// std::_Rb_tree<unsigned __int64, pair<const unsigned __int64, CCombatantInfo> >::erase(const key_type& key) -- STLport erase by key: count the equal_range with distance, erase that range, return the count
// Grade: DIFFER(83v83) -- Rb_tree erase(key) speed TU: one store/load pair scheduled differently (erase's two by-value iterators); 9 forms tried
// Leaf: ?erase@?$_Rb_tree@_KU?$pair@$$CB_KVCCombatantInfo@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F50B0, 83 bytes, thiscall, `ret 4`.
// Identity (probable): its callers CCombatManager::DeregisterAttackingCombatant / DeregisterAttackedCombatant (landed) erase a creature's UID from Combatants -- Ego_r.pdb CCombatManager::Combatants is map<unsigned __int64, CCombatantInfo> (stand-in ?erase@CCombatantMap@@QAEXAB_K@Z).
// Callees: equal_range(k) 0x006F27B0 (pair through the hidden pointer), __distance(first, last, const input_iterator_tag&)
//   0x006F0350 (cdecl, out of line), erase(iterator, iterator) 0x006F4E40.
// Shape (family K009, representative 0x0051B880): the landed G053 _Rb_tree::erase(const key_type&) 0x0042CDE0 in a speed TU (no
//   frame): the distance call pops 8 bytes and its third slot carries erase's first iterator.
class CCombatantInfo;
namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Tp> struct less { };
template <class _Pair> struct _Select1st { };
template <class _Tp> struct _Identity { };
template <class _Tp, class _Alloc = allocator<_Tp> > class list;
template <class _Tp> struct _Nonconst_traits { };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
template <class _Iterator> struct iterator_traits {
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::difference_type difference_type;
};
template <class _InputIterator>
typename iterator_traits<_InputIterator>::difference_type __cdecl
__distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&);   // 0x006F0350
template <class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type __cdecl
distance(const _InputIterator& __first, const _InputIterator& __last)
{
    return __distance(__first, __last, typename iterator_traits<_InputIterator>::iterator_category());
}

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr __cdecl _M_increment(_Base_ptr);          // 0x004292C0
};
typedef _Rb_global<bool> _Rb_global_inst;
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef int difference_type;
    _Base_ptr _M_node;
    bool operator==(const _Rb_tree_base_iterator& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Rb_tree_base_iterator& __y) const { return _M_node != __y._M_node; }
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Base_ptr __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
};
template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Key key_type;
    typedef unsigned int size_type;
    typedef _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> > iterator;
    unsigned int _M_node_count;
    _Compare _M_key_compare;
    pair<iterator, iterator> equal_range(const key_type& __x);   // 0x006F27B0 (own COMDAT)
    void erase(iterator __first, iterator __last);              // 0x006F4E40 (own COMDAT)
    size_type erase(const key_type& __x)
    {
        pair<iterator, iterator> __p = equal_range(__x);
        size_type __n = distance(__p.first, __p.second);
        erase(__p.first, __p.second);
        return __n;
    }
};
typedef unsigned __int64 _TreeKey;
typedef pair<const unsigned __int64, CCombatantInfo> _TreeValue;
template _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey> >::size_type
    _Rb_tree<_TreeKey, _TreeValue, _Select1st<_TreeValue>, less<_TreeKey> >::erase(const _TreeKey&);
}
