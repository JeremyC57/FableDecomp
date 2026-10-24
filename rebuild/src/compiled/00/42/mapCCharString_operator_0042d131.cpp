// std::map<CCharString, unsigned long>::operator[](const CCharString& k) -- STLport subscript: lower_bound, insert (k, 0) when absent, return the value
// Retail 0x0042D131, 96 bytes, `ret 4` (catalog: map<CCharString,unsigned_long>::operator[]).
// Body (STLport 4.x verbatim): i = lower_bound(k) (_M_lower_bound @ 0x0042AD34, node pointer in eax);
// if (i == end() || key_comp()(k, (*i).first)) i = insert(i, value_type(k, unsigned long()));
// return (*i).second (node +0x14).
// Callees: less<CCharString>::operator() @ 0x0042AAB3 (out of line here; its `this` is the key_comp()
// temporary, homed in the high byte of the dead `k` slot: `lea ecx,[ebp+0Bh]`), CCharString copy ctor @
// 0x0099EC30 and dtor @ 0x0099EAE0 for the value_type temporary in the frame, insert(iterator, const
// value_type&) @ 0x0042CDA6 (iterator argument built in its slot by the STLport iterator copy ctor, result
// returned through the dead `k` slot).
// Shape: same TU and template as the landed map<CComponent*, long>::operator[] 0x0042D1B7 (frame-pointer size
// TU); the CCharString key makes the comparison an out-of-line functor call and the temporary pair a
// constructed / destroyed object. The functor temporary overlays the still-live `k` slot only when
// less<CCharString>::operator() -- a COMDAT of this same TU in retail (0x0042AAB3, landed) -- is a visible
// noinline body that never touches `this`; declared only, the temporary takes its own frame word (0xC v 8).
// Leaf: ??A?$map
#pragma optimize("s", on)

template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;     // 0x00429950
};

class CCharString
{
public:
    CBasicString<char>* PData;                           // +0x00
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    __forceinline bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};

namespace std {
template <class T> struct less {
    __declspec(noinline) bool operator()(const T& x, const T& y) const { return x < y; }   // 0x0042AAB3 for CCharString
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    bool operator==(const _Rb_tree_base_iterator& y) const { return _M_node == y._M_node; }
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return static_cast<_Rb_tree_node<V>*>(_M_node)->_M_value_field; }
};
template <class K, class T, class C = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;
    unsigned int        _M_node_count;
    C                   _M_key_compare;
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;        // 0x0042AD34
    iterator insert(iterator position, const value_type& v);     // 0x0042CDA6
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    T& operator[](const K& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};
template unsigned long& map<CCharString, unsigned long>::operator[](const CCharString&);
}
