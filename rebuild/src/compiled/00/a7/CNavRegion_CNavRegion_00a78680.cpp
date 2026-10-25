// CNavRegion::CNavRegion(long mapIndex, long internalIndex) -- build a navigation region: an empty connection list (one malloc'd sentinel node), RegionID = mapIndex << 16 ^ (internalIndex & 0xFFFF), MostRecentSearchID = 0
// Leaf: ??0CNavRegion@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A78680, 62 bytes (+ int3 padding to 64), thiscall `ret 8`, returns this.
// Identity: Ego_r.pdb `??0CNavRegion@@QAE@JJ@Z` (CNavRegion(long, long)) and the CNavRegion layout (RegionID +0,
//   Connections +4 -- an STLport list<CCountedPointer<CNavRegionConnection> >, 4 bytes in retail -- MostRecentSearchID
//   +8; the landed 0x00A786C0 SetAsConnectedToRegion uses the same layout). The 16-byte list node is the two links plus
//   the 8-byte counted pointer. GetMapIndex/GetInternalIndex/GetMapIndexFromRegionID (PDB) read the packed ID back.
//   No direct caller (the region list is built through an inlined copy elsewhere); catalog "PackedKeyList_Construct" is
//   descriptive.
// Callees: malloc 0x00BFEA0E (allocator<_List_node>::allocate(1)).
// Shape: the list member's STLport ctor runs first: `_M_node(a, 0)` (zero store), then the sentinel built in a local --
//   allocate(1), next/prev pointing at itself, THEN published to `_M_node._M_data` (the member-first spelling re-reads
//   the member after the store; under /Oa it stores the member before the links). RegionID and MostRecentSearchID are
//   body assignments (stored after the list).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

struct SCountBlock;
class CNavRegionConnection;

template <class T> class CCountedPointer {
public:
    T*           Data;                                      // +0
    SCountBlock* Info;                                      // +4
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    _List_base(const _Alloc&) : _M_node(_Node_allocator_type(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
};
}

class CNavRegion {
public:
    long RegionID;                                                      // +0
    std::list<CCountedPointer<CNavRegionConnection> > Connections;     // +4
    long MostRecentSearchID;                                            // +8
    CNavRegion(long mapIndex, long internalIndex);
};

CNavRegion::CNavRegion(long mapIndex, long internalIndex)
{
    RegionID = (mapIndex << 16) ^ (internalIndex & 0xFFFF);
    MostRecentSearchID = 0;
}