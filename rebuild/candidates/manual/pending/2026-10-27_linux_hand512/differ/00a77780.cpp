// CNavRegion::IsConnectedToRegion(const CNavRegion& region) const -- breadth-first search over the region-connection graph: true when region is this region (same RegionID) or reachable through connections; each search stamps visited regions with a fresh search id
// Grade: DIFFER(302v298) -- prologue shrink-wrap: VC defers the ebx/ebp/esi pushes past the RegionID early return (its own pop/ret), while retail pushes all four registers at entry and shares the mov al,1 epilogue (3 grades + 1 QFE probe)
// Leaf: ?IsConnectedToRegion@CNavRegion@@QBE_NABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A77780, 298 bytes, thiscall `ret 4`.
// Identity (proven): Ego_r.pdb has exactly one `bool CNavRegion::X(const CNavRegion&) const` -- IsConnectedToRegion; the
//   body compares RegionID first and walks Connections (list<CCountedPointer<CNavRegionConnection>>, Ego_r.pdb), each
//   connection's other side found through the inline CNavRegionConnection::GetRegionConnectedTo. Its caller
//   CNavigatorAStar::CanNavigateToPos 0x00A1BCA0 (landed) binds it under an invented pointer spelling IsConnectedTo.
// Layout (retail, STLport list = 4 bytes): RegionID +0x0, Connections +0x4, MostRecentSearchID +0x8 (Ego_r.pdb +0x10 with
//   the 12-byte Dinkumware list).
// Callees: malloc 0x00BFEA0E (the open list's sentinel and nodes), free 0x00BFEA14. Global: the search-id counter
//   0x013D2D28 (descriptive name).
// Shape: STLport 4.6 list<const CNavRegion*> verbatim (sentinel node allocated by the ctor, push_back = insert(end()),
//   pop_front = erase(begin()), the destructor's clear loop + sentinel free duplicated on both exits).
extern "C" void* __cdecl malloc(unsigned int size);        // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

extern long NavRegionSearchID_013d2d28;                    // 0x013D2D28

namespace std {
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return (_Tp*)malloc(__n * sizeof(_Tp)); }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp> _Node;
    _Node* _M_node;
    list()
    {
        _M_node = allocator<_Node>().allocate(1);
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    ~list()
    {
        clear();
        allocator<_Node>().deallocate(_M_node, 1);
    }
    void clear()
    {
        _Node* __cur = (_Node*)_M_node->_M_next;
        while (__cur != _M_node) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            allocator<_Node>().deallocate(__tmp, 1);
        }
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    bool empty() const { return _M_node->_M_next == _M_node; }
    _Tp& front() { return ((_Node*)_M_node->_M_next)->_M_data; }
    void push_back(const _Tp& __x)
    {
        _Node* __tmp = allocator<_Node>().allocate(1);
        new (&__tmp->_M_data) _Tp(__x);
        _List_node_base* __n = _M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
    }
    void pop_front()
    {
        _List_node_base* __pos = _M_node->_M_next;
        _List_node_base* __next = __pos->_M_next;
        _List_node_base* __prev = __pos->_M_prev;
        __prev->_M_next = __next;
        __next->_M_prev = __prev;
        allocator<_Node>().deallocate((_Node*)__pos, 1);
    }
};
}

template <class T> class CCountedPointer {
public:
    T* Data;                                               // +0x0
    void* Info;                                            // +0x4
    T* operator->() const { return Data; }
};

class CNavRegion;
class CNavRegionConnection {                               // 12 bytes (Ego_r.pdb)
public:
    CNavRegion* FirstRegion;                               // +0x0
    CNavRegion* SecondRegion;                              // +0x4
    long        NumberOfConnections;                       // +0x8
    CNavRegion& GetRegionConnectedTo(const CNavRegion& region) const
    {
        if (&region == FirstRegion)
            return *SecondRegion;
        return *FirstRegion;
    }
};

class CNavRegion {
public:
    long RegionID;                                                         // +0x0
    std::list<CCountedPointer<CNavRegionConnection> > Connections;         // +0x4
    mutable long MostRecentSearchID;                                       // +0x8
    bool IsConnectedToRegion(const CNavRegion& region) const;
};

bool CNavRegion::IsConnectedToRegion(const CNavRegion& region) const
{
    if (region.RegionID == RegionID)
        return true;
    MostRecentSearchID = ++NavRegionSearchID_013d2d28;
    std::list<const CNavRegion*> open;
    open.push_back(this);
    while (!open.empty()) {
        const CNavRegion* current = open.front();
        for (std::list<CCountedPointer<CNavRegionConnection> >::_Node* it = (std::list<CCountedPointer<CNavRegionConnection> >::_Node*)current->Connections._M_node->_M_next;
             it != current->Connections._M_node;
             it = (std::list<CCountedPointer<CNavRegionConnection> >::_Node*)it->_M_next) {
            const CNavRegion* other = &it->_M_data->GetRegionConnectedTo(*current);
            if (other == &region)
                return true;
            if (other->MostRecentSearchID != NavRegionSearchID_013d2d28) {
                other->MostRecentSearchID = NavRegionSearchID_013d2d28;
                open.push_back(other);
            }
        }
        open.pop_front();
    }
    return false;
}
