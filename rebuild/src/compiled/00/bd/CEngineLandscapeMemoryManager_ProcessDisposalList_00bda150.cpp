// CEngineLandscapeMemoryManager::ProcessDisposalList() -- advance delayed resource destruction one generation: with delayed destruction off, free every disposal list; then destroy and free every texture, vertex buffer and index buffer queued in the generation after the current one
// Leaf: ?ProcessDisposalList@CEngineLandscapeMemoryManager@@AAEXXZ
// Flags: /d2inlT114
// Inline-budget: /d2inlT114 (EXTRA_RULES 2026-11-08 preference 2; NF276 D4 S3) -- the owner 0x00BD9B90's own CompilerFlags, so this TU's
//   pick-any FreeVirtual copy (and the inlined CMemoryAllocator::Deallocate) come out as the owner's 97-byte body (pk EQ 0x00BD9B90; at the
//   default threshold 68 bytes, NEQ); the row's own leaf is unchanged at T114 (one retail TU, one setting).
// Inline: the header inlines CMemoryAllocator::Deallocate and Destroy (engine/CMemoryAllocator.h opt-in
//   FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE) are plain inline; VC7.1 expands both here at its default threshold (NF145 sweep:
//   MATCH with no inline control; the nf140 FABLE_FORCEINLINE is gone from the header).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDA150, 514 bytes, thiscall, plain `ret`. Speed TU (no pragma).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?ProcessDisposalList@CEngineLandscapeMemoryManager@@AAEXXZ (private; Ego_r
//   009C2A30 between EnablePoolAllocation 009C29D0 and AllocateIndexBuffer 009C2B20 -- retail has EnablePoolAllocation
//   0x00BDA070, this body, AllocateIndexBuffer 0x00BDA360). Callers are the class's three Allocate* members (0x00BDA360,
//   0x00BDA460, 0x00BDA560), which the landed sources spell `ProcessDisposalLists` (NAME_FIXES: the PDB spelling is singular).
//   Catalog "_global::CDrawResourcePool_FlushCycle" is descriptive and wrong (class member).
// Callees: FreeDisposalList 0x00BD9CE0 (out of line), CTexture::Uninitialise 0x009F9F70, the virtual destructors of
//   CVertexBufferWin32 / CIndexBuffer (slot 0, flag 0), CMemoryAllocator::EnterSynchronisation 0x009F2000 / CMemoryAllocator::LeaveSynchronisation 0x009F2010,
//   CMemoryAllocator::InternalDeallocate 0x009F2920 / GetAllocationCount 0x009F1F10 / ~CMemoryAllocator 0x009F2310 /
//   operator delete 0x009F1F90 (the inlined FreeVirtual 0x00BD9B90), free 0x00BFEA14 (list node).
// Shape: `if (!EnableDelayedResourceDestruction) FreeDisposalList(); long index = (CurrentDisposalListIndex + 1) % 3;` then
//   the three `while (list[index].size() != 0) { DeleteX(front); pop_front(); }` loops of the landed FreeDisposalList
//   0x00BD9CE0 (same TU model: the private DeleteTextureBlockWhileBusy / DeleteVertexBufferBlockWhileBusy /
//   DeleteIndexBufferBlockWhileBusy and FreeVirtual auto-inlined).
// Names (NF267, 2026-11-22): the three destroy-now helpers were spelled with the public DeleteTexture / DeleteVertexBuffer /
//   DeleteIndexBuffer names; those are the queue functions 0x00BDA110 / 0x00BDA0D0 / 0x00BDA090 (public, called only from other
//   classes). The destroy-now bodies are the private ...BlockWhileBusy trio (Ego_r.pdb / FableWin.pdb / Ego_d.pdb, `AAE`): retail
//   0x00BD9C00 / 0x00BD9C20 / 0x00BD9CC0 (out-of-line copies with no caller; VB / IB for the first two from Ego_r order only).
// Fidelity: scaffolding, not retail (NF274, D4): the retail TU-mates FreeVirtual 0x00BD9B90 and the BlockWhileBusy trio are defined `inline`
//   here and in 0x00BD9CE0, so both objects emit them pick-any (one definition; at HEAD both defined them strongly: 4 LNK4006); bytes unchanged.
// NF140 (q4197 / q4198): the allocation is the inline public static CMemoryAllocator::Allocate(size, flags) of engine/CMemoryAllocator.h
// (Ego_r.pdb / FableWin.pdb ?Allocate@CMemoryAllocator@@SIPAXKK@Z, inline in release): ?Create@CMemoryAllocator@@KIXXZ 0x009F2870, the protected
// statics ?EnterSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2000 / ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2010 (both PDBs),
// PMemoryAllocator 0x013CAA90 and InternalAllocate 0x009F2A40, as the landed 0x009A3640 binds them.
// NF140 (q4197 / q4198): the frees are the inline public static CMemoryAllocator::Deallocate(block) (FableWin.pdb ?Deallocate@CMemoryAllocator@@SIXPAX@Z,
// out of line at 0x009A4350; engine/CMemoryAllocator.h opt-in FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE, the body of the landed 0x009F2B20).
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#include "engine/CMemoryAllocator.h"
extern "C" void __cdecl free(void* p);                     // 0x00BFEA14

namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
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
template <class _Tp> struct _Nonconst_traits { };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _InputIterator> inline unsigned int distance(_InputIterator __first, _InputIterator __last)
{
    unsigned int __n = 0;
    while (__first != __last) { ++__first; ++__n; }
    return __n;
}
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    unsigned int size() { return distance(begin(), end()); }
    _Tp& front() { return *begin(); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
    void pop_front() { erase(begin()); }
};
}   // namespace std

class CTexture {                                           // non-polymorphic (no RTTI vtable); ~CTexture is inline
public:
    void Uninitialise();                                   // 0x009F9F70
    ~CTexture() { Uninitialise(); }
};
class CVertexBufferWin32 {                                 // vtable 0x0129BA20 (slot 0 ??_G 0x009E2890)
public:
    virtual ~CVertexBufferWin32();
};
class CIndexBuffer {                                       // vtable 0x0129EF2C (slot 0 ??_G 0x00AC2070)
public:
    virtual ~CIndexBuffer();
};
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
    void FreeVirtual(void* block);                         // 0x00BD9B90
private:
    void DeleteIndexBufferBlockWhileBusy(CIndexBuffer* buffer);         // 0x00BD9C20 (Ego_r order)
    void DeleteVertexBufferBlockWhileBusy(CVertexBufferWin32* buffer);  // 0x00BD9C00 (Ego_r order)
    void DeleteTextureBlockWhileBusy(CTexture* texture);                // 0x00BD9CC0
public:
    void FreeDisposalList();                               // 0x00BD9CE0
private:
    void ProcessDisposalList();                            // 0x00BDA150 (this row)
    virtual bool HandleFailedAllocation(unsigned long size);
    virtual void RelocateData(long id, void* from, void* to, long size);
    virtual bool CanRelocateData(long id) const;
};

inline void CEngineLandscapeMemoryManager::FreeVirtual(void* block)
{
    if (block) {
        CMemoryAllocator::Deallocate(block);
    }
}

inline void CEngineLandscapeMemoryManager::DeleteIndexBufferBlockWhileBusy(CIndexBuffer* buffer)
{
    buffer->~CIndexBuffer();
    FreeVirtual(buffer);
}

inline void CEngineLandscapeMemoryManager::DeleteVertexBufferBlockWhileBusy(CVertexBufferWin32* buffer)
{
    buffer->~CVertexBufferWin32();
    FreeVirtual(buffer);
}

inline void CEngineLandscapeMemoryManager::DeleteTextureBlockWhileBusy(CTexture* texture)
{
    texture->~CTexture();
    FreeVirtual(texture);
}

void CEngineLandscapeMemoryManager::ProcessDisposalList()
{
    if (!EnableDelayedResourceDestruction)
        FreeDisposalList();
    long index = (CurrentDisposalListIndex + 1) % 3;
    while (TextureDisposalList[index].size() != 0) {
        DeleteTextureBlockWhileBusy(TextureDisposalList[index].front());
        TextureDisposalList[index].pop_front();
    }
    while (VertexBufferDisposalList[index].size() != 0) {
        DeleteVertexBufferBlockWhileBusy(VertexBufferDisposalList[index].front());
        VertexBufferDisposalList[index].pop_front();
    }
    while (IndexBufferDisposalList[index].size() != 0) {
        DeleteIndexBufferBlockWhileBusy(IndexBufferDisposalList[index].front());
        IndexBufferDisposalList[index].pop_front();
    }
}