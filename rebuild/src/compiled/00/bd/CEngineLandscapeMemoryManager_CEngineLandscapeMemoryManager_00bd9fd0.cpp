// CEngineLandscapeMemoryManager::CEngineLandscapeMemoryManager(const CEngineMemoryInit& init) -- construct the landscape memory manager: its two base subobjects, the three-deep texture / vertex-buffer / index-buffer disposal lists (each STLport list with a self-linked sentinel), and cleared defragment, allocation and delayed-destruction state
// Leaf: ??0CEngineLandscapeMemoryManager
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD9FD0, 151 bytes, thiscall, `ret 4`, returns this.
// Identity: Ego_r.pdb ??0CEngineLandscapeMemoryManager@@QAE@ABVCEngineMemoryInit@@@Z; RTTI of vtables 0x012A7CBC (+0) and
//   0x012A7CA8 (+4): CEngineLandscapeMemoryManager : CFailedAllocationHandler (+0), CMovableResource (+4). One caller
//   0x00B6B07D. The init argument is not read in this build. Catalog "ParticleSpriteGroup_Construct" is wrong.
// Callees: malloc @ 0x00BFEA0E (each list sentinel, STLport node allocator).
// Layout: Ego_r.pdb CEngineLandscapeMemoryManager with retail 4-byte STLport lists (PDB 12-byte lists): VirtualMemPool +8,
//   PhysicalMemPool +0xC (not set here), TextureDisposalList[3] +0x10, VertexBufferDisposalList[3] +0x1C,
//   IndexBufferDisposalList[3] +0x28, CurrentDisposalListIndex +0x34, InsideDefragment +0x38, PoolAllocationEnabled
//   +0x39, AllocationCount +0x3C, EnableDelayedResourceDestruction +0x40. engine/CEngineLandscapeMemoryManager.h carries
//   the Ego_r (0x24-byte list array) offsets, so the class is a local model.
// Shape: CMovableResource has a user constructor (its vtable 0x0129B860 is stored, then overwritten); the
//   CFailedAllocationHandler base has only the implicit one, whose vtable store VC7.1 drops. Each list array is an inline
//   three-step loop of the STLport _List_base constructor (zero store, malloc(12), self-link, publish).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;                      // +0x00
    _List_node_base* _M_prev;                      // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                                   // +0x08
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef allocator<_Node> _Node_allocator_type;
    _List_base(const allocator_type&) : _M_node(_Node_allocator_type(), (_Node*)0)
    {
        _Node* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x00 the sentinel
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type()) : _List_base<_Tp, _Alloc>(__a) { }
};

}   // namespace std

class CEngineMemoryInit;
class CTexture;
class CVertexBufferWin32;
class CIndexBuffer;
class CMovableResourceMemoryPool;
class CXboxBufferMemoryPool;

class CFailedAllocationHandler {                   // vtable 0x0129B874
public:
    virtual ~CFailedAllocationHandler() { }
    virtual bool HandleFailedAllocation(unsigned long size) = 0;
};

class CMovableResource {                           // vtable 0x0129B860
public:
    CMovableResource() { }
    virtual ~CMovableResource() { }
    virtual void RelocateData(long offset, void* oldBase, void* newBase, long size) = 0;
    virtual bool CanRelocateData(long offset) const;                     // 0x009D50F0
    virtual void v003();                                                 // 0x009D5100
};

class CEngineLandscapeMemoryManager : public CFailedAllocationHandler, public CMovableResource {
public:
    CEngineLandscapeMemoryManager(const CEngineMemoryInit& init);
    virtual ~CEngineLandscapeMemoryManager();

    CMovableResourceMemoryPool*         VirtualMemPool;                  // +0x08
    CXboxBufferMemoryPool*              PhysicalMemPool;                 // +0x0c
    std::list<CTexture*>               TextureDisposalList[3];          // +0x10
    std::list<CVertexBufferWin32*>     VertexBufferDisposalList[3];     // +0x1c
    std::list<CIndexBuffer*>           IndexBufferDisposalList[3];      // +0x28
    long                                CurrentDisposalListIndex;        // +0x34
    bool                                InsideDefragment;                // +0x38
    bool                                PoolAllocationEnabled;           // +0x39
    long                                AllocationCount;                 // +0x3c
    bool                                EnableDelayedResourceDestruction; // +0x40
private:
    virtual bool HandleFailedAllocation(unsigned long size);             // 0x00BD9B30
    virtual void RelocateData(long offset, void* oldBase, void* newBase, long size);   // 0x00BD9B70
    virtual bool CanRelocateData(long offset) const;                     // 0x00BD9B80
};

CEngineLandscapeMemoryManager::CEngineLandscapeMemoryManager(const CEngineMemoryInit& init)
{
    InsideDefragment = false;
    PoolAllocationEnabled = false;
    AllocationCount = 0;
    EnableDelayedResourceDestruction = false;
    CurrentDisposalListIndex = 0;
}