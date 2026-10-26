// std::map<CCharString, CCountedPointer<NUISystem::CComponent> >::operator[](const CCharString& k) -- STLport subscript: lower_bound, insert (k, null counted pointer) when absent, return the value
// Leaf: ??A?$map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0042D45D, length 118 bytes, `ret 4` after `leave`. The catalog's
// "CPersistTraits_Transfer_And_Construct" is a propagated label. Callers: NUISystem::CManager::CreateMenuEntryComponent
// (0x004284D4, 0x004284F3, 0x00428511, 0x00428671), which store `CCountedPointer<CComponent>(CreateComponent(...))` under a
// name -- the PDB's map<CCharString, CCountedPointer<NUISystem::CComponent> > (CManager::BastardChildren) value type.
// Body (STLport 4.x verbatim, the same template as the landed map<CCharString, unsigned long>::operator[] 0x0042D131):
// i = lower_bound(k) (_M_lower_bound @ 0x0042AE58); if (i == end() || key_comp()(k, (*i).first))
// i = insert(i, value_type(k, T())); return (*i).second (node +0x14).
// Callees: less<CCharString>::operator() @ 0x0042AAB3 (out of line; its `this` is the key_comp() temporary homed in the
// high byte of the dead `k` slot, `lea ecx,[ebp+0Bh]`), pair<const CCharString, CCountedPointer<CComponent> >::pair
// @ 0x00429F9C (out of line here: CCharString copy 0x0099EC30 + CCountedPointer copy 0x00428928), insert(iterator,
// const value_type&) @ 0x0042D2A9 (iterator built in its argument slot, result returned through the dead `k` slot),
// ~CCountedPointer @ 0x004291DE (pair.second, then the T() temporary), ~CCharString @ 0x0099EAE0 (pair.first).
// Shape: frame-pointer size TU; the T() temporary is the zeroed 8-byte counted pointer at [ebp-8] (`and dword ptr`),
// the value_type temporary sits at [ebp-14h]; the less<CCharString> functor body is a same-TU noinline COMDAT as in
// the landed sibling.
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
    pair(const T1& a, const T2& b);                      // 0x00429F9C for pair<const CCharString, CCountedPointer<CComponent> >
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
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;        // 0x0042AE58
    iterator insert(iterator position, const value_type& v);     // 0x0042D2A9
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
}
namespace NUISystem { class CComponent; }
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;                                // +0x00
    CCPPointerInfo* Info;                                // +0x04
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other);       // 0x00428928
    ~CCountedPointer();                                  // 0x004291DE
};
template CCountedPointer<NUISystem::CComponent>& std::map<CCharString, CCountedPointer<NUISystem::CComponent> >::operator[](const CCharString&);