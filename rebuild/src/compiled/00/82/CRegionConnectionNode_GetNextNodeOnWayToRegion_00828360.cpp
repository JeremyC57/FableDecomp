// CRegionConnectionNode::GetNextNodeOnWayToRegion(long toRegion, const CRegionConnectionGraph& graph) const -- breadth-first search over the region connection graph from this node: returns the first region to step into on the way to toRegion (toRegion itself when it is a direct neighbour, -1 when this is the target or it cannot be reached)
// Leaf: ?GetNextNodeOnWayToRegion@CRegionConnectionNode
// Retail: Fable.exe (TLC, VC7.1) @ 0x00828360, 555 bytes, thiscall `ret 8`.
// Identity: Ego_r.pdb ?GetNextNodeOnWayToRegion@CRegionConnectionNode@@QBEJJABVCRegionConnectionGraph@@@Z (the debug
//   build's 0x00731060); the only caller is CRegionConnectionGraph::GetNextRegionOnWayToRegion 0x00828590 (landed). The
//   catalog's "Navigation_SortRegionConnectionTree" is descriptive.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (the STLport list's node allocator), CVectorMap<long,
//   CCountedPointer<CRegionConnectionNode> >::LowerBound(const long&) 0x00827B60, the list destructor 0x00827E40 (out of
//   line at the first-loop return only).
// Globals: the search stamp 0x013847BC (referenced by this function only). It must have external linkage: as a
//   file-scope `static` VC7.1 re-reads it at every use of the second loop and keeps the index in ebp; as an ordinary
//   global it is promoted to ebp across the inner loop (re-read after push_back's malloc) and the index spills.
// Layout: Ego_r.pdb CRegionConnectionNode with the retail 12-byte STLport vector: RegionID +0, Connections +4,
//   MostRecentSearchID +0x10, FirstNodeInSearch +0x14 (the landed SetAsConnectedToRegion 0x008285D0 model); the graph
//   is the landed 0x00828590 CVectorMap model (first/last +0/+4, 12-byte {key, CCountedPointer} entries).
// Shape: a local STLport list<const CRegionConnectionNode*> as the BFS queue (the sentinel built in a register and
//   stored last), the graph's private inline GetRegionNode (find + `(it == end || key < it->first) ? end : it` + the
//   counted pointer's object), push_back = insert(end(), x) through create_node (placement null test), pop_front =
//   erase(begin()). LowerBound must be a same-TU body (noinline, the real binary-search shape): VC7.1 then knows the
//   call writes no memory and keeps the key and the search stamp in registers across it (an extern declaration
//   re-reads the key from its frame slot). The STLport ~_List_base (clear + `if (p) free(p)`) goes out of line at the
//   first-loop return only (0x00827E40) and inline at the other two exits, where the sentinel's null test drops.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14
inline void* operator new(unsigned int, void* where) { return where; }

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    const _Tp& operator[](unsigned int __n) const { return *(_M_start + __n); }
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
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base(const _List_iterator_base& __x) : _M_node(__x._M_node) { }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator<_Tp>& __x) : _List_iterator_base(__x) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;
    _List_base(const allocator<_Node>& __a) : _M_node(__a, (_List_node_base*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    ~_List_base()
    {
        clear();
        _M_node.deallocate((_Node*)_M_node._M_data, 1);
    }
    void clear()
    {
        _Node* __cur = (_Node*)_M_node._M_data->_M_next;
        while (__cur != _M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _M_node.deallocate(__tmp, 1);
        }
        _M_node._M_data->_M_next = _M_node._M_data;
        _M_node._M_data->_M_prev = _M_node._M_data;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    list() : _List_base<_Tp, _Alloc>(allocator<_Node>()) { }
    iterator begin() { return iterator((_Node*)this->_M_node._M_data->_M_next); }
    iterator end() { return iterator((_Node*)this->_M_node._M_data); }
    bool empty() const { return this->_M_node._M_data->_M_next == this->_M_node._M_data; }
    _Tp& front() { return *begin(); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = this->_M_node.allocate(1);
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
    void pop_front() { erase(begin()); }
};
}

class CRegionConnectionNode;

template <class T> struct CCountedPointer {
    T*    Object;                                       // +0x00
    void* PBlock;                                       // +0x04
};

struct SRegionNodeEntry {                               // pair<long, CCountedPointer<CRegionConnectionNode> >
    long                                    first;      // +0x00
    CCountedPointer<CRegionConnectionNode>  second;     // +0x04
};

struct CRegionNodeMap {                                 // CVectorMap<long, CCountedPointer<CRegionConnectionNode> >
    SRegionNodeEntry* m_first;                          // +0x00
    SRegionNodeEntry* m_last;                           // +0x04
    SRegionNodeEntry* m_end;                            // +0x08
    SRegionNodeEntry* LowerBound(const long& key) const;   // 0x00827B60
    SRegionNodeEntry* Find(const long& key) const
    {
        SRegionNodeEntry* it = LowerBound(key);
        return (it == m_last || key < it->first) ? m_last : it;
    }
};

#pragma auto_inline(off)
SRegionNodeEntry* CRegionNodeMap::LowerBound(const long& key) const   // 0x00827B60
{
    SRegionNodeEntry* first = m_first;
    int len = m_last - m_first;
    while (len > 0) {
        int half = len / 2;
        SRegionNodeEntry* middle = first + half;
        if (middle->first < key) {
            first = middle + 1;
            len = len - half - 1;
        }
        else
            len = half;
    }
    return first;
}
#pragma auto_inline(on)

class CRegionConnectionGraph {
public:
    CRegionNodeMap GraphNodes;                          // +0x00
    const CRegionConnectionNode* GetRegionNode(long region) const
    {
        SRegionNodeEntry* it = GraphNodes.Find(region);
        if (it == GraphNodes.m_last)
            return 0;
        return it->second.Object;
    }
};

class CRegionConnectionNode {
public:
    long              RegionID;                         // +0x00
    std::vector<long> Connections;                      // +0x04
    mutable long      MostRecentSearchID;               // +0x10
    mutable long      FirstNodeInSearch;                // +0x14
    long GetNextNodeOnWayToRegion(long toRegion, const CRegionConnectionGraph& graph) const;
};

long g_RegionSearchID;                                 // 0x013847BC

long CRegionConnectionNode::GetNextNodeOnWayToRegion(long toRegion, const CRegionConnectionGraph& graph) const
{
    if (RegionID == toRegion)
        return -1;
    ++g_RegionSearchID;
    std::list<const CRegionConnectionNode*> queue;
    MostRecentSearchID = g_RegionSearchID;
    FirstNodeInSearch = 0;
    for (unsigned int i = 0; i < Connections.size(); ++i) {
        const CRegionConnectionNode* node = graph.GetRegionNode(Connections[i]);
        if (node != 0) {
            if (node->RegionID == toRegion)
                return toRegion;
            node->MostRecentSearchID = g_RegionSearchID;
            node->FirstNodeInSearch = node->RegionID;
            queue.push_back(node);
        }
    }
    while (!queue.empty()) {
        const CRegionConnectionNode* cur = queue.front();
        for (unsigned int i = 0; i < cur->Connections.size(); ++i) {
            const CRegionConnectionNode* node = graph.GetRegionNode(cur->Connections[i]);
            if (node != 0 && node->MostRecentSearchID != g_RegionSearchID) {
                if (node->RegionID == toRegion)
                    return cur->FirstNodeInSearch;
                node->MostRecentSearchID = g_RegionSearchID;
                node->FirstNodeInSearch = cur->FirstNodeInSearch;
                queue.push_back(node);
            }
        }
        queue.pop_front();
    }
    return -1;
}