// CEngineLandscapeMemoryManager::~CEngineLandscapeMemoryManager() -- free the delayed-disposal lists, destroy the three per-frame disposal list arrays (index, vertex, texture buffers) and fall through the inline CMovableResource / CFailedAllocationHandler base destructors
// Leaf: ??1CEngineLandscapeMemoryManager@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BD9ED0, 245 bytes, thiscall, plain `ret`.
// Identity (proven): it stores vtables 0x012A7CBC / 0x012A7CA8 (RTTI .?AVCEngineLandscapeMemoryManager@@, bases
//   CFailedAllocationHandler @0 and CMovableResource @4) and ends storing the bases' 0x0129B874 / 0x0129B860; Ego_r.pdb
//   `??1CEngineLandscapeMemoryManager@@UAE@XZ` follows FreeDisposalList in donor order as retail 0x00BD9ED0 follows the
//   callee 0x00BD9CE0; its only caller 0x00BDAF63 is the class's ??_G. The catalog's "ParticleSpriteGroup_Destroy" is
//   wrong (propagated).
// Callees: FreeDisposalList() @ 0x00BD9CE0 (catalog "TexturePool_Clear", Ego_r ?FreeDisposalList@...@@QAEXXZ),
//   free @ 0x00BFEA14 (STLport list nodes and sentinels).
// Layout (local model; the engine header flattens both bases into `__vftable` + pad and keeps the list arrays opaque):
//   VirtualMemPool +8, PhysicalMemPool +0xC, TextureDisposalList[3] +0x10, VertexBufferDisposalList[3] +0x1C,
//   IndexBufferDisposalList[3] +0x28 (STLport lists, one sentinel pointer each), CurrentDisposalListIndex +0x34,
//   InsideDefragment +0x38, PoolAllocationEnabled +0x39, AllocationCount +0x3C, EnableDelayedResourceDestruction +0x40.
// Shape: the body is the one call; the implicit member destruction runs each array backwards (counted `dec ebx` loop
//   of three) through STLport 4.6 ~_List_base (clear() inline: free each node, relink the sentinel to itself, then
//   deallocate the sentinel behind its null test); the base destructors are inline empty virtual ones (vtable stores).
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14

namespace std {

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
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

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
    ~_List_base()
    {
        clear();
        _M_node.deallocate(_M_node._M_data, 1);
    }
    void clear()
    {
        _Node* __cur = (_Node*)this->_M_node._M_data->_M_next;
        while (__cur != this->_M_node._M_data) {
            _Node* __tmp = __cur;
            __cur = (_Node*)__cur->_M_next;
            _Destroy(&__tmp->_M_data);
            this->_M_node.deallocate(__tmp, 1);
        }
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> { };

}   // namespace std

class CTexture;
class CVertexBufferWin32;
class CIndexBuffer;
class CMovableResourceMemoryPool;
class CXboxBufferMemoryPool;

class CFailedAllocationHandler {                           // vtable 0x0129B874
public:
    virtual ~CFailedAllocationHandler() { }
    virtual bool HandleFailedAllocation(unsigned long size) = 0;
};

class CMovableResource {                                   // vtable 0x0129B860
public:
    virtual ~CMovableResource() { }
    virtual void RelocateData(long id, void* from, void* to, long size) = 0;
    virtual bool CanRelocateData(long id) const = 0;
    virtual unsigned long Verify() const;
};

class CEngineLandscapeMemoryManager : public CFailedAllocationHandler, public CMovableResource {
public:
    CMovableResourceMemoryPool* VirtualMemPool;            // +0x08
    CXboxBufferMemoryPool*      PhysicalMemPool;           // +0x0C
    std::list<CTexture*>           TextureDisposalList[3];       // +0x10
    std::list<CVertexBufferWin32*> VertexBufferDisposalList[3];  // +0x1C
    std::list<CIndexBuffer*>       IndexBufferDisposalList[3];   // +0x28
    long                        CurrentDisposalListIndex;  // +0x34
    bool                        InsideDefragment;          // +0x38
    bool                        PoolAllocationEnabled;     // +0x39
    long                        AllocationCount;           // +0x3C
    bool                        EnableDelayedResourceDestruction;   // +0x40
    virtual ~CEngineLandscapeMemoryManager();
    void FreeDisposalList();                               // 0x00BD9CE0
private:
    virtual bool HandleFailedAllocation(unsigned long size);
    virtual void RelocateData(long id, void* from, void* to, long size);
    virtual bool CanRelocateData(long id) const;
};

CEngineLandscapeMemoryManager::~CEngineLandscapeMemoryManager()
{
    FreeDisposalList();
}