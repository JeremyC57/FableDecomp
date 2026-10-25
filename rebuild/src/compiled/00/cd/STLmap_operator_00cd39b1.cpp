// std::map::operator[](const CCharString& k) -- STLport subscript of map<CCharString, unsigned long> (size TU): lower_bound, insert (k, 0) when absent, return the value
// Leaf: ??A?$map
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CD39B1, 96 bytes, __fastcall `ret 4`, frame-pointer size TU.
// Identity: only caller CRegion::SetMiniMapRegionExitTextOffsetX (0x00CBFAB8) on the global map @ 0x0143E910; the
//   manifest type map<CCharString, unsigned long> agrees. Byte twin of the landed 0x0042D131 (same template, other TU).
// Callees: _M_lower_bound @ 0x00CD2884 (node pointer in eax), less<CCharString>::operator() @ 0x0042AAB3 (landed; its
//   `this` is the key_comp() temporary homed in the high byte of the dead `k` slot), CCharString copy ctor @ 0x0099EC30
//   / dtor @ 0x0099EAE0, insert(iterator, const value_type&) @ 0x00CD367C (iterator built in its slot, result through
//   the dead `k` slot).
// Shape: the landed 0x0042D131 source verbatim with this TU's tree helpers: STLport 4.x operator[] under
//   optimize("s", on); the functor body is a visible noinline COMDAT that never touches `this` (declared only, the
//   temporary takes its own frame word).
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
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;        // 0x00CD2884
    iterator insert(iterator position, const value_type& v);     // 0x00CD367C
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