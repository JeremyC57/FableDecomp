// std::_Rb_tree::erase(const key_type& __x) -- (set<long>) STLport erase by key: count the equal_range with distance, erase that range, return the count
// Grade: DIFFER(83v83) -- DIFFER(83v83): one swapped pair -- retail loads the first by-value iterator (mov ecx,[esp+10h]) before storing the second into its argument slot (mov [eax],edx); every model stores first
// Leaf: ?erase@?$_Rb_tree@JJU?$_Identity@J@std@@U?$less@J@2@V?$allocator@J@2@@std@@QAEIABJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00639590, 83 bytes (+ int3 padding), thiscall, `ret 4`; /O2 TU (no frame pointer).
// Identity (proven): std::_Rb_tree<long, long, ...>::erase(const key_type&); evidence: CGameTimeManager::SetTimeAsStopped 0x006BBE30 erases from this+0x2C (Ego_r.pdb CGameTimeManager::StopTimeIndexes, set<long>, +0x2C) and CTCVillage::RemoveCrime 0x0065C146 from this+0x64 (Ego_r.pdb CTCVillage::PlayersCrimes, set<long>, +0xDC, after fifteen 12-byte PDB lists that are 4 bytes in retail); the other callers are 0x00638300 (this+0x20) and the one-jmp set::erase(key) wrapper 0x00639720.
// Label: the manifest's "Crime_RemoveEntry" is propagated / behavioural (family J084: five tree erase-by-key bodies that differ only in
//   their three callees; retail has no identical-code folding).
// Callees: this tree's equal_range(const key_type&) 0x00639250 (pair returned through the hidden pointer), __distance(const iterator&,
//   const iterator&, const input_iterator_tag&) 0x00638C30 (cdecl, out of line), erase(iterator, iterator) 0x00638EE0.
// Shape: agent951 G053 (0x0042CDE0, a size TU) under /O2: STLport 4.6 _tree.h verbatim; the bidirectional tag temporary lives in
//   the dead key argument slot (a user constructor, so nothing is stored), and only 8 bytes of the distance call are popped -- the
//   third slot is reused for the first by-value iterator of erase(first, last).
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
__distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&);   // 0x00638C30 (own COMDAT)
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
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef int difference_type;
    _Base_ptr _M_node;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Base_ptr __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
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
    pair<iterator, iterator> equal_range(const key_type& __x);   // 0x00639250 (own COMDAT)
    void erase(iterator __first, iterator __last);              // 0x00638EE0 (own COMDAT)
    size_type erase(const key_type& __x)
    {
        pair<iterator, iterator> __p = equal_range(__x);
        size_type __n = distance(__p.first, __p.second);
        erase(__p.first, __p.second);
        return __n;
    }
};
typedef long TKey_00639590;
template _Rb_tree<TKey_00639590, long, _Identity<long>, less<long> >::size_type
    _Rb_tree<TKey_00639590, long, _Identity<long>, less<long> >::erase(const TKey_00639590&);
}   // namespace std
