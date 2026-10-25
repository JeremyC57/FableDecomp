// std::_Rb_tree::erase(const key_type& x) -- (map<void*, CRGBColour>) STLport 4.6 erase-by-key: equal_range, count the range with the out-of-line __distance, erase it, return the count
// Grade: DIFFER(83v83) -- scheduling only: building erase's by-value iterator arguments, retail loads p.first (`mov ecx,[esp+10h]`) before storing p.second into the leftover distance slot (`mov [eax],edx`); VC7.1 here stores first. Same with the QFE-4035 compiler.
// Leaf: ?erase@?$_Rb_tree@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C4430, 83 bytes (+ int3 padding), speed TU, thiscall `ret 4`.
// Callees: equal_range(const key&) 0x004C4080 (pair through a hidden pointer), __distance(const it&, const it&,
//   const input_iterator_tag&) 0x004C1D40 (__cdecl `_STLP_CALL`; the bidirectional tag temporary homed in the dead x
//   argument slot; only 8 bytes popped, the third slot becomes erase's `last`), erase(iterator, iterator) 0x004C3F90.
// Identity: _Rb_tree<void*, pair<void* const, CRGBColour> >::erase(void* const&), i.e. map<void*, CRGBColour> -- proven;
//   evidence: its direct caller 0x004BFAE0 (catalogue "CParticleSystem::Kill", propagated) takes one pointer argument, erases
//   it from +0x18 = CTCGraphicAppearance::TintColours (map<void*, CRGBColour>; MeshEffects +0x0C, both 12-byte STLport maps,
//   as the landed CTCGraphicAppearance rows lay them out), then flags the appearance for a resend: Ego_r.pdb
//   CTCGraphicAppearance::RemoveTintColour(void*); 0x004C4CC0 (`jmp`) is map::erase(const key&); the PDB name sets hold the
//   map<void*, CRGBColour> erase. Manifest label "Entity_Destroy_Wrapper" is propagated/behavioural.
// Tried: STLport iterator (base _M_node, assignment in the copy ctor); a flat iterator with an init-list copy ctor;
//   (grade 4, probe/g005v4) the G053 pair without a user copy constructor plus a same-TU noinline __distance body: the same
//   two-instruction swap. 4 grades used.
namespace std {
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __o) : first(__o.first), second(__o.second) { }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; };
struct _Rb_tree_base_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    _Rb_tree_node_base* _M_node;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
};

template <class _InputIterator>
int __cdecl __distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&);   // 0x004C1D40

template <class _InputIterator>
inline int __cdecl distance(const _InputIterator& __first, const _InputIterator& __last)
{
    return __distance(__first, __last, bidirectional_iterator_tag());
}

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Key key_type;
    typedef unsigned int size_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;

    _STLP_alloc_proxy<_Link_type, _Rb_tree_node<_Value>, allocator<_Rb_tree_node<_Value> > > _M_header;   // +0x00
    size_type _M_node_count;                                            // +0x04
    _Compare _M_key_compare;                                            // +0x08

    pair<iterator, iterator> equal_range(const key_type& __x);          // 0x004C4080
    void erase(iterator __first, iterator __last);                      // 0x004C3F90
    size_type erase(const key_type& __x);
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
typename _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::size_type
_Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::erase(const key_type& __x)
{
    pair<iterator, iterator> __p = equal_range(__x);
    size_type __n = distance(__p.first, __p.second);
    erase(__p.first, __p.second);
    return __n;
}
}
class CRGBColour { public: unsigned char B, G, R, A; };          // 4 bytes
typedef std::pair<void* const, CRGBColour> TValue;
template unsigned int std::_Rb_tree<void*, TValue, std::_Select1st<TValue>, std::less<void*>, std::allocator<TValue> >::erase(void* const&);
