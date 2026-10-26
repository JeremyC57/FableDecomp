// std::_Rb_tree<NUISystem::CComponent*, pair<NUISystem::CComponent* const, NUISystem::CComponent*>, _Select1st<...>,
//   less<NUISystem::CComponent*>, allocator<...> >::equal_range(const key_type&) -- the [lower_bound, upper_bound) pair for a key
// Leaf: ?equal_range@?$_Rb_tree@PAVCComponent@NUISystem@@
// Retail: Fable.exe @ 0x0042C058, 42 bytes, `ret 8` (hidden pair return slot + key). The body calls _M_upper_bound
// (0x0042AEB5) then _M_lower_bound (0x0042AE90) -- right-to-left argument evaluation of `pair(lower_bound(k),
// upper_bound(k))` -- and stores them as {first, second}. Both helpers compare the key unsigned (`jb` / `jae`) at node +0x10.
// Identity: the tree behind CTCInventoryTrade::ItemBuyLists / ItemSellLists (struct_layouts_egor: map<NUISystem::CComponent*,
// NUISystem::CComponent*>, retail +0x200 / +0x20C). The only caller is erase(const key_type&) 0x0042CDE0 (equal_range, the
// out-of-line distance 0x0042A8BD, then erase(first, last) 0x0042B854, whose whole-range path calls the landed clear
// 0x0042ADE5 of this tree); 0x0042CDE0 in turn is called only by the two thunks 0x0042D437 (this+0x20C) and 0x0042D44A
// (this+0x200). The real map<unsigned long, long> (CVertexGroupBlock::GroupToIndexMap) has its own equal_range 0x005D8676
// and bounds 0x005D8446 / 0x005D84F2, so this tree is not that one. Earlier landed as _Rb_tree<unsigned long, pair<const
// unsigned long, long> > (and the catalog's CBankFile::CompareDataOffset::operator() before that); same bytes.
// Opt: size TU (unaligned entry, `push dword ptr [esp+10h]`). Shape: STLport's _Rb_tree_iterator has a user copy
// constructor (its converting ctor from iterator); without it VC7.1 moves `pop edi` ahead of both pair stores.
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }

namespace std {
template <class T1, class T2> struct pair {
    typedef T1 first_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) {}
};
template <class T> struct less {
    bool operator()(const T& x, const T& y) const { return x < y; }
};
template <class T> class allocator {};
template <class V> struct _Select1st {
    const typename V::first_type& operator()(const V& x) const { return x.first; }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& it) : _M_node(it._M_node) {}
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_iterator<V> iterator;
    _Base_ptr _M_header;                                 // +0x00
    size_t    _M_node_count;                             // +0x04
    _Base_ptr _M_lower_bound(const K& k) const;          // 0x0042AE90
    _Base_ptr _M_upper_bound(const K& k) const;          // 0x0042AEB5
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator upper_bound(const K& k) { return iterator(_M_upper_bound(k)); }
    pair<iterator, iterator> equal_range(const K& k)
    {
        return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
    }
};
}
typedef std::pair<NUISystem::CComponent* const, NUISystem::CComponent*> CComponentPair;
typedef std::_Rb_tree<NUISystem::CComponent*, CComponentPair, std::_Select1st<CComponentPair>, std::less<NUISystem::CComponent*>,
                      std::allocator<CComponentPair> > CComponentMapTree;
template std::pair<CComponentMapTree::iterator, CComponentMapTree::iterator> CComponentMapTree::equal_range(NUISystem::CComponent* const&);
