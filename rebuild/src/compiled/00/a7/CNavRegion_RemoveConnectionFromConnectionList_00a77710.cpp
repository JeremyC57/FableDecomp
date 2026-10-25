// CNavRegion::RemoveConnectionFromConnectionList(const CNavRegionConnection* connection) -- find the region's counted connection entry for `connection` and erase it (releasing the counted reference); nothing happens when it is not listed
// Leaf: ?RemoveConnectionFromConnectionList@CNavRegion@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A77710, 101 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: FableWin.pdb `?RemoveConnectionFromConnectionList@CNavRegion@@AAEXPBVCNavRegionConnection@@@Z`; callers
//   CNavRegionConnection's release 0x00A778B0 and CNavRegion::RemoveConnectionToRegion 0x00A78060, each calling it on both
//   regions of a connection whose count dropped to zero. The catalog label `remove_if<CTCCreatureModeManager::CModeBase::
//   CIsType>` is propagated.
// Callees: the count block's DeleteFunc, operator delete @ 0x00BFE9BC (the count block), free @ 0x00BFEA14 (the node).
// Layout: Ego_r.pdb CNavRegion { RegionID +0, Connections +4 (retail 4-byte STLport list), MostRecentSearchID +8 };
//   node { next, prev, CCountedPointer<CNavRegionConnection> { Data +8, Info +0xc } }.
// Shape: the CTCEntityEvents::RemoveEvent twin (0x008112D0): a for loop with the erase and an early `return` inside.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

struct CCPPointerInfo {                                     // Ego_r.pdb
    unsigned long RefCount;                                 // +0x0
    void (__fastcall* DeleteFunc)(void* data);              // +0x4
    void* Data;                                             // +0x8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    T* operator->() const { return Data; }
    bool operator==(const CCountedPointer& other) const { return Data == other.Data; }
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Tp* operator->() const { return &((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    void remove(const _Tp& __val)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__val == *__first)
                erase(__first);
            __first = __next;
        }
    }
    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first))
                erase(__first);
            __first = __next;
        }
    }
};
}

class CNavRegion;
class CNavRegionConnection {                                // Ego_r.pdb, 12 bytes
public:
    CNavRegion* FirstRegion;                                // +0x0
    CNavRegion* SecondRegion;                               // +0x4
    long        NumberOfConnections;                        // +0x8
};

class CNavRegion {
public:
    typedef _STL::list<CCountedPointer<CNavRegionConnection> > CConnectionList;
    long            RegionID;                               // +0x0
    CConnectionList Connections;                            // +0x4
    long            MostRecentSearchID;                     // +0x8
private:
    void RemoveConnectionFromConnectionList(const CNavRegionConnection* connection);   // private in FableWin.pdb (`AAE`)
};

void CNavRegion::RemoveConnectionFromConnectionList(const CNavRegionConnection* connection)
{
    for (CConnectionList::iterator it = Connections.begin(); it != Connections.end(); ++it) {
        if (it->Data == connection) {
            Connections.erase(it);
            return;
        }
    }
}