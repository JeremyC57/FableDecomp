// CNavigatorManager::GetQuadTreeMemoryUsage(unsigned long& a, unsigned long& b, unsigned long& c, unsigned long& d) -- total memory of every loaded navigation quad tree, each tree also adding to the four per-kind counters
// Leaf: ?GetQuadTreeMemoryUsage@CNavigatorManager@@QBEKAAK000@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A15100, 63 bytes, `ret 10h`. Name: Ego_r/FableWin PDB
//   ?GetQuadTreeMemoryUsage@CNavigatorManager@@QBEKAAK000@Z (the catalog row agrees).
// Callee: CNavQuadTree::GetMemoryUsage(ulong&, ulong&, ulong&, ulong&) const @ 0x00A77E80, on each map's PQuadTree.
// Layout: retail CNavigatorManager (engine/CNavigatorManager.h): QuadTrees +0x24 (STLport list, one node pointer);
//   CNavQuadTreeMap (Ego_r.pdb): PQuadTree (CCountedPointer, 8 bytes) +0, Index +8, MapArea +0xC, SectionName.
// Shape: a const_iterator walk over the real STLport 4.6 list, the end node re-read every pass.
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator_base() {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) {}
    _List_iterator() {}
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) {}
    reference operator*() const { return ((_Node*)_M_node)->_M_data; }
    pointer operator->() const { return &(operator*()); }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : protected _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_iterator<_Tp, _Const_traits<_Tp> > const_iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    const_iterator begin() const { return const_iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
};
}   // namespace std

class CNavQuadTree {
public:
    unsigned long GetMemoryUsage(unsigned long& a, unsigned long& b, unsigned long& c, unsigned long& d) const;   // 0x00A77E80
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
};
class CNavQuadTreeMap {
public:
    CCountedPointer<CNavQuadTree> PQuadTree;   // +0x00
    long                          Index;       // +0x08
};

// Layout: local model at the engine/CNavigatorManager.h offsets; the header carries QuadTrees as opaque bytes
//   (`unsigned char QuadTrees[0x4]`), so the real std::list member is declared here (NOTES.md "Header debt").
class CNavigatorManager {
public:
    void*         __vftable;                              // +0x00
    unsigned char _pad04[0x18];
    void*         Seed;                                   // +0x1c
    long          NumberOfNodeExaminationsLeftThisFrame;  // +0x20
    std::list<CNavQuadTreeMap> QuadTrees;                 // +0x24
    unsigned long GetQuadTreeMemoryUsage(unsigned long& a, unsigned long& b, unsigned long& c, unsigned long& d) const;
};

unsigned long CNavigatorManager::GetQuadTreeMemoryUsage(unsigned long& a, unsigned long& b, unsigned long& c, unsigned long& d) const
{
    unsigned long total = 0;
    for (std::list<CNavQuadTreeMap>::const_iterator it = QuadTrees.begin(); it != QuadTrees.end(); ++it)
        total += it->PQuadTree->GetMemoryUsage(a, b, c, d);
    return total;
}