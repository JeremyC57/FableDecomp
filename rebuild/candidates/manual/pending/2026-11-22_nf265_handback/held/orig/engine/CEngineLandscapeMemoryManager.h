#ifdef FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#ifndef FABLE_ENGINE_CENGINELANDSCAPEMEMORYMANAGER_H
#define FABLE_ENGINE_CENGINELANDSCAPEMEMORYMANAGER_H
// Retail opt-in (chunk 779, row 0x00BEAE50): RTTI .?AVCEngineLandscapeMemoryManager@@ : CFailedAllocationHandler@0, CMovableResource@4
// (ghidra_out/rtti_bases.tsv); the Ego_r.pdb members follow the two vptrs (VirtualMemPool +8 ... EnableDelayedResourceDestruction +0x40,
// STLport lists 4 bytes each: retail sizeof 0x44). Member functions from the Ego_r.pdb name set, bound by landed rows:
// AllocVirtual 0x00BD9AF0 (0x00BEA7E0), FreeVirtual 0x00BD9B90 (0x00BE8E60), AllocateVertexBuffer 0x00BDA3D0 and AllocateIndexBuffer
// 0x00BDA360 (0x00BE3660). The row reaches the manager through CEngineLandscapeRenderer::MemoryManager (+0x6B0 of the global 0x01436EA8).
#include <stddef.h>
#include "rebuild_abi.h"
#ifndef FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CMOVABLERESOURCE
#endif
#include "engine/CFailedAllocationHandler.h"
#include "engine/CMovableResource.h"
#include "stlport/fable_containers.h"
#include "engine/EVertexType.h"
class CMovableResourceMemoryPool;
class CXboxBufferMemoryPool;
class CTexture;
class CVertexBufferWin32;
class CIndexBuffer;
enum EIndexBufferFormat;                                                 // MS forward enum
enum EPrimitiveType;                                                     // MS forward enum
class CEngineLandscapeMemoryManager : public CFailedAllocationHandler, public CMovableResource {
public:
#ifdef FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_PROCESSDISPOSALLIST
    // Retail opt-in (NF149, row 0x00BDA3D0): Ego_r.pdb / FableWin.pdb ?ProcessDisposalList@CEngineLandscapeMemoryManager@@AAEXXZ (retail 0x00BDA150,
    // private; AllocateVertexBuffer / AllocateIndexBuffer call it first).
private:
    void ProcessDisposalList();                                          // 0x00BDA150
public:
#endif
#ifdef FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_CTOR
    // Retail opt-in (wfE7, row 0x00B69000): Ego_r.pdb ??0CEngineLandscapeMemoryManager@@QAE@ABVCEngineMemoryInit@@@Z (retail 0x00BD9FD0,
    // `new` of 0x44 bytes in the CEngineLandscapeRenderer ctor). Declaration only.
    CEngineLandscapeMemoryManager(const class CEngineMemoryInit& init);  // 0x00BD9FD0
    virtual ~CEngineLandscapeMemoryManager();                            // ??1CEngineLandscapeMemoryManager@@UAE@XZ
private:
    // Ego_r.pdb private overrides (EAE / EBE) of the two bases' virtuals.
    virtual bool HandleFailedAllocation(unsigned long size);             // ?HandleFailedAllocation@CEngineLandscapeMemoryManager@@EAE_NK@Z
    virtual void RelocateData(long, void*, void*, long);                 // ?RelocateData@CEngineLandscapeMemoryManager@@EAEXJPAX0J@Z
    virtual bool CanRelocateData(long) const;                            // ?CanRelocateData@CEngineLandscapeMemoryManager@@EBE_NJ@Z
public:
#endif
    void* AllocVirtual(long size, CMovableResource* owner);              // 0x00BD9AF0 ?AllocVirtual@CEngineLandscapeMemoryManager@@QAEPAXJPAVCMovableResource@@@Z
    void FreeVirtual(void* block);                                       // 0x00BD9B90 ?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z
    CIndexBuffer* AllocateIndexBuffer(CMovableResource* owner, long count, unsigned long usage, EIndexBufferFormat format, EPrimitiveType type);   // 0x00BDA360
    CVertexBufferWin32* AllocateVertexBuffer(CMovableResource* owner, EVertexType type, long count, long stride, unsigned long usage);   // 0x00BDA3D0 ?AllocateVertexBuffer@CEngineLandscapeMemoryManager@@QAEPAVCVertexBufferWin32@@PAVCMovableResource@@W4EVertexType@@JJK@Z
    CMovableResourceMemoryPool*     VirtualMemPool;                      // +0x08
    CXboxBufferMemoryPool*          PhysicalMemPool;                     // +0x0c
    std::list<CTexture*>            TextureDisposalList[3];              // +0x10
    std::list<CVertexBufferWin32*>  VertexBufferDisposalList[3];         // +0x1c
    std::list<CIndexBuffer*>        IndexBufferDisposalList[3];          // +0x28
    long                            CurrentDisposalListIndex;            // +0x34
    bool                            InsideDefragment;                    // +0x38
    bool                            PoolAllocationEnabled;               // +0x39
    long                            AllocationCount;                     // +0x3c
    bool                            EnableDelayedResourceDestruction;    // +0x40
};
FABLE_STATIC_ASSERT_AT(42, sizeof(CEngineLandscapeMemoryManager) == 0x44);
FABLE_STATIC_ASSERT_AT(43, offsetof(CEngineLandscapeMemoryManager, VirtualMemPool) == 0x8);
FABLE_STATIC_ASSERT_AT(44, offsetof(CEngineLandscapeMemoryManager, CurrentDisposalListIndex) == 0x34);
FABLE_STATIC_ASSERT_AT(45, offsetof(CEngineLandscapeMemoryManager, EnableDelayedResourceDestruction) == 0x40);
#endif // FABLE_ENGINE_CENGINELANDSCAPEMEMORYMANAGER_H
#else // flattened Ego_r layout (the landed default, unchanged)
#ifndef FABLE_ENGINE_CENGINELANDSCAPEMEMORYMANAGER_H
#define FABLE_ENGINE_CENGINELANDSCAPEMEMORYMANAGER_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineLandscapeMemoryManager = 0x8c.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineLandscapeMemoryManager = 0x44.
// Retail evidence: 3 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CMovableResourceMemoryPool;
class CXboxBufferMemoryPool;

#pragma pack(push, 1)
class CEngineLandscapeMemoryManager {
public:
    void*                       __vftable;                   // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char               _pad_0x04[0x4];              // +0x04
    CMovableResourceMemoryPool* VirtualMemPool;              // +0x08
    CXboxBufferMemoryPool*      PhysicalMemPool;             // +0x0c
    unsigned char               TextureDisposalList[0xc];    // +0x10 list<CTexture_*,std::allocator<CTexture_*>_>[3]
    unsigned char               VertexBufferDisposalList[0xc];  // +0x1c list<CVertexBufferWin32_*,std::allocator<CVertexBufferWin32_*>_>[3]
    unsigned char               IndexBufferDisposalList[0xc];  // +0x28 list<CIndexBuffer_*,std::allocator<CIndexBuffer_*>_>[3]
    long                        CurrentDisposalListIndex;    // +0x34
    bool                        InsideDefragment;            // +0x38
    bool                        PoolAllocationEnabled;       // +0x39
    unsigned char               _pad_0x3a[0x2];              // +0x3a
    long                        AllocationCount;             // +0x3c
    bool                        EnableDelayedResourceDestruction; // +0x40
    unsigned char               _pad_0x41[0x3];              // +0x41
};
#pragma pack(pop)

FABLE_STATIC_ASSERT_AT(82, sizeof(CEngineLandscapeMemoryManager) == 0x44);
FABLE_STATIC_ASSERT_AT(83, offsetof(CEngineLandscapeMemoryManager, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(84, offsetof(CEngineLandscapeMemoryManager, VirtualMemPool) == 0x8);
FABLE_STATIC_ASSERT_AT(85, offsetof(CEngineLandscapeMemoryManager, PhysicalMemPool) == 0xc);
FABLE_STATIC_ASSERT_AT(86, offsetof(CEngineLandscapeMemoryManager, TextureDisposalList) == 0x10);
FABLE_STATIC_ASSERT_AT(87, offsetof(CEngineLandscapeMemoryManager, VertexBufferDisposalList) == 0x1c);
FABLE_STATIC_ASSERT_AT(88, offsetof(CEngineLandscapeMemoryManager, IndexBufferDisposalList) == 0x28);
FABLE_STATIC_ASSERT_AT(89, offsetof(CEngineLandscapeMemoryManager, CurrentDisposalListIndex) == 0x34);
FABLE_STATIC_ASSERT_AT(90, offsetof(CEngineLandscapeMemoryManager, InsideDefragment) == 0x38);
FABLE_STATIC_ASSERT_AT(91, offsetof(CEngineLandscapeMemoryManager, PoolAllocationEnabled) == 0x39);
FABLE_STATIC_ASSERT_AT(92, offsetof(CEngineLandscapeMemoryManager, AllocationCount) == 0x3c);
FABLE_STATIC_ASSERT_AT(93, offsetof(CEngineLandscapeMemoryManager, EnableDelayedResourceDestruction) == 0x40);

#endif // FABLE_ENGINE_CENGINELANDSCAPEMEMORYMANAGER_H

#endif // FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
