#if defined(FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL_DEFRAG)
#ifndef FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H
#define FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H
// Retail opt-in (chunk 1022, row 0x00A02DF0 PartialDefragment): the real class -- CBaseClassNonCopyable base (vtable 0x0129C964,
// RTTI CHandleBasedMemoryPool : CBaseClassNonCopyable : CBaseClass), Ego_r.pdb member names, the nested CHeader (Ego_r, 32 bytes)
// and CHandleInfo (4-byte union), and retail's 12-byte STLport CArray, so Name +0x448 and CriticalSection +0x44C (0x464 bytes;
// the landed 0x00A02D20 / 0x00A02DB0 / 0x00A030D0 / 0x00A03180 rows measure the same offsets).
// Member functions: Ego_r.pdb / FableWin.pdb spellings; the bodies retail keeps in this TU and that callers need visible
// (FindLargestFreeArea 0x00A01F50, GetListIndex, BeginCritical / EndCritical) are given inline-declared below.
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CArray.h"
#include "engine/Win32Types.h"
#include "fable_string.h"
class CFailedAllocationHandler;
extern "C" __declspec(dllimport) int __stdcall TryEnterCriticalSection(CRITICAL_SECTION* section);   // IAT 0x0143FD30 (winbase.h)
class CHandleBasedMemoryPool : public CBaseClassNonCopyable {
public:
    class CHeader {                                          // Ego_r.pdb CHandleBasedMemoryPool::CHeader (32 bytes)
    public:
        CHeader();
        void*          Resource;                             // +0x00
        long           Handle;                               // +0x04
        long           Size;                                 // +0x08
        short          Padding;                              // +0x0c
        unsigned char  Status;                               // +0x0e ('N' free, 'T' taken)
        unsigned char  Locked;                               // +0x0f
        CHeader**      RefAvailable;                         // +0x10
        CHeader*       NextAvailable;                        // +0x14
        CHeader*       PrevContiguous;                       // +0x18
        CHeader*       NextContiguous;                       // +0x1c
    };
    class CHandleInfo {                                      // Ego_r.pdb: 4 bytes (a union: the area, or the next free slot)
    public:
        CHandleInfo();
        union { CHeader* Header; long NextFree; };
    };
    class CScopedCritical {                                  // Ego_r.pdb CHandleBasedMemoryPool::CScopedCritical (8 bytes)
    public:
        CScopedCritical(CHandleBasedMemoryPool& owner, bool tryOnly)    // FableWin ??0CScopedCritical@CHandleBasedMemoryPool@@QAE@AAV1@_N@Z
            : Owner(&owner) { Success = owner.BeginCritical(tryOnly); }
        ~CScopedCritical()                                   // ??1CScopedCritical@CHandleBasedMemoryPool@@QAE@XZ (0x00A03530 out of line)
        {
            if (Success)
                Owner->EndCritical();
        }
        CHandleBasedMemoryPool* Owner;                       // +0x00
        bool                    Success;                     // +0x04
    };
    friend class CScopedCritical;
    CHandleBasedMemoryPool();
    virtual ~CHandleBasedMemoryPool();
    bool PartialDefragment(long size);                       // 0x00A02DF0 (?PartialDefragment@CHandleBasedMemoryPool@@QAE_NJ@Z)
protected:
    virtual void MoveResource(void*, long, void*, void*, unsigned long);
    virtual bool CanMoveResource(void*, long);
    virtual void ValidateResource(void*);
private:
    bool BeginCritical(bool tryOnly)                         // ?BeginCritical@CHandleBasedMemoryPool@@AAE_N_N@Z (out of line 0x00A02140, landed; inline in Allocate / PartialDefragment)
    {
        if (!MultiThreaded)
            return true;
        if (tryOnly) {
            if (!TryEnterCriticalSection(&CriticalSection))
                return false;
        } else {
            EnterCriticalSection(&CriticalSection);
        }
        return true;
    }
    void EndCritical()                                       // ?EndCritical@CHandleBasedMemoryPool@@AAEXXZ
    {
        if (MultiThreaded)
            LeaveCriticalSection(&CriticalSection);
    }
    // ?GetListIndex@CHandleBasedMemoryPool@@ABEKK@Z (FableWin 0x0319C460; inline in retail): one free list per 512 bytes, 256 lists.
    // The ternary is retail's spelling: PartialDefragment 0x00A02DF0 threads its 0xFF arm past the loop test (0x00A02F43), which the
    // if-assignment form does not; FindLargestFreeArea 0x00A01F50 keeps the select either way.
    unsigned long GetListIndex(unsigned long size) const
    {
        return (size >> 9) < 0xff ? (size >> 9) : 0xff;
    }
    void CombineAreas(CHeader* first, CHeader* second);      // 0x00A01E40 ?CombineAreas@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@0@Z
    CHeader* FindFreeArea(long size);                        // 0x00A01ED0 ?FindFreeArea@CHandleBasedMemoryPool@@AAEPAVCHeader@1@J@Z
    void CombineAdjacentAreas(CHeader*& area);               // 0x00A02470 ?CombineAdjacentAreas@CHandleBasedMemoryPool@@AAEXAAPAVCHeader@1@@Z
    void MoveArea(CHeader* from, CHeader* to);               // 0x00A028F0 ?MoveArea@CHandleBasedMemoryPool@@AAEXPAVCHeader@1@0@Z
    CHeader* FindLargestFreeArea();                          // 0x00A01F50 ?FindLargestFreeArea@CHandleBasedMemoryPool@@AAEPAVCHeader@1@XZ
    void UpdateLargestFreeArea();                            // 0x00A01F80 ?UpdateLargestFreeArea@CHandleBasedMemoryPool@@AAEXXZ
    bool TestIfAreaIsGrowable(CHeader* area, long size);     // 0x00A02020 ?TestIfAreaIsGrowable@CHandleBasedMemoryPool@@AAE_NPAVCHeader@1@J@Z
    bool GrowFreeArea(CHeader*& area);                       // 0x00A02C00 ?GrowFreeArea@CHandleBasedMemoryPool@@AAE_NAAPAVCHeader@1@@Z
public:
    void*                     Buffer;                        // +0x04
    void*                     UnalignedBuffer;               // +0x08
    long                      PoolSize;                      // +0x0c
    long                      AllocatedSize;                 // +0x10
    long                      NumAllocations;                // +0x14
    long                      LockCount;                     // +0x18
    long                      LargestFreeArea;               // +0x1c
    unsigned long             MemoryFlags;                   // +0x20
    bool                      MultiThreaded;                 // +0x24
    bool                      ShutdownFlag;                  // +0x25
    bool                      EnableDefrag;                  // +0x26
    bool                      EnableFullDefragIfPartialFails; // +0x27
    unsigned long             MaxFreeSizeBeforeDefrag;       // +0x28
    unsigned long             MaxFreeSizeBeforeFailure;      // +0x2c
    long                      MaxBlocksToTryOnPartialDefrag; // +0x30
    CFailedAllocationHandler* FailureHandler;                // +0x34
    CHeader*                  FreeLists[256];                // +0x38
    CArray<CHandleInfo>       Handles;                       // +0x438 (retail STLport CArray, 12 bytes)
    long                      FirstUnusedHandle;             // +0x444
    CCharString               Name;                          // +0x448
    _RTL_CRITICAL_SECTION     CriticalSection;               // +0x44c
};
FABLE_STATIC_ASSERT(sizeof(CHandleBasedMemoryPool) == 0x464);
#endif
#elif defined(FABLE_RETAIL_CHANDLEBASEDMEMORYPOOL)
#ifndef FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H
#define FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H
#include "rebuild_abi.h"
#include "engine/CBaseClassNonCopyable.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
class C3DVector;class CQuaternion;class CMatrix3x4;class C3DAnimFile;class CDataInputStream;class CDataOutputStream;class CChunkedFileLoader;class CChunkedFileSaver;class CChunkedFileSuperChunk;class CSimpleMemoryPool;class C3DAnimFileSequenceChunk;class C3DAnimFileObjectChunk;class C3DAnimFileRootChunk;class C3DAnimFileHelpersChunk;class C3DAnimFileMovementVectorChunk;class C3DAnimFileBoneMaskChunk;class C3DAnimationSequenceData;class C3DAnimationSequence;class CMovableResourceMemoryPool;class CMovableResource;
// Layout: original raw PDB fields and bases; retail sizes reconciled by native accesses and 12-byte vectors.
class CHandleBasedMemoryPool:public CBaseClassNonCopyable{public:
CHandleBasedMemoryPool(const CHandleBasedMemoryPool&);
CHandleBasedMemoryPool();
virtual ~CHandleBasedMemoryPool();
#ifdef FABLE_RETAIL_C3DANIMATIONINFO_BUILD
// Retail opt-in (Codex pass19): original Ego_r/FableWin types and donor calls; native animation rows 00a24d70/00a26ad0. See NAMING_EVIDENCE.md.
void Defragment();
#endif
void * GetPointerFromHandle(long);
protected:
virtual void MoveResource(void *,long,void *,void *,unsigned long);
virtual bool CanMoveResource(void *,long);
virtual void ValidateResource(void *);
private:unsigned char Buffer[0x460];};
FABLE_STATIC_ASSERT(sizeof(CHandleBasedMemoryPool)==0x464);
#endif
#else
#ifndef FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H
#define FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CHandleBasedMemoryPool = 0x468.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CHandleBasedMemoryPool = 0x464.
// Retail evidence: 1 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCharStringData;
class CFailedAllocationHandler;
struct CHeader;
struct _RTL_CRITICAL_SECTION_DEBUG;

#pragma pack(push, 1)
class CHandleBasedMemoryPool {
public:
    void*                        __vftable;                  // +0x00 vptr, or first dword of a flattened base subobject
    void*                        Buffer;                     // +0x04
    void*                        UnalignedBuffer;            // +0x08
    long                         PoolSize;                   // +0x0c
    long                         AllocatedSize;              // +0x10
    long                         NumAllocations;             // +0x14
    long                         LockCount;                  // +0x18
    long                         LargestFreeArea;            // +0x1c
    unsigned long                MemoryFlags;                // +0x20
    bool                         MultiThreaded;              // +0x24
    bool                         ShutdownFlag;               // +0x25
    bool                         EnableDefrag;               // +0x26
    bool                         EnableFullDefragIfPartialFails; // +0x27
    unsigned long                MaxFreeSizeBeforeDefrag;    // +0x28
    unsigned long                MaxFreeSizeBeforeFailure;   // +0x2c
    long                         MaxBlocksToTryOnPartialDefrag; // +0x30
    CFailedAllocationHandler*    FailureHandler;             // +0x34
    CHeader*                     FreeLists[256];             // +0x38
    unsigned char                Handles[0xc];               // +0x438 CArray<CHandleBasedMemoryPool::CHandleInfo>
    long                         FirstUnusedHandle;          // +0x444
    CCharStringData*             Name_PStringData;           // +0x448 CCharString::PStringData
    _RTL_CRITICAL_SECTION_DEBUG* CriticalSection_DebugInfo;  // +0x44c _RTL_CRITICAL_SECTION::DebugInfo
    long                         CriticalSection_LockCount;  // +0x450 _RTL_CRITICAL_SECTION::LockCount
    long                         CriticalSection_RecursionCount; // +0x454 _RTL_CRITICAL_SECTION::RecursionCount
    void*                        CriticalSection_OwningThread; // +0x458 _RTL_CRITICAL_SECTION::OwningThread
    void*                        CriticalSection_LockSemaphore; // +0x45c _RTL_CRITICAL_SECTION::LockSemaphore
    unsigned long                CriticalSection_SpinCount;  // +0x460 _RTL_CRITICAL_SECTION::SpinCount
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CHandleBasedMemoryPool) == 0x464);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, Buffer) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, UnalignedBuffer) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, PoolSize) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, AllocatedSize) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, NumAllocations) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, LockCount) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, LargestFreeArea) == 0x1c);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, MemoryFlags) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, MultiThreaded) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, ShutdownFlag) == 0x25);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, EnableDefrag) == 0x26);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, EnableFullDefragIfPartialFails) == 0x27);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, MaxFreeSizeBeforeDefrag) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, MaxFreeSizeBeforeFailure) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, MaxBlocksToTryOnPartialDefrag) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, FailureHandler) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, FreeLists) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, Handles) == 0x438);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, FirstUnusedHandle) == 0x444);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, Name_PStringData) == 0x448);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, CriticalSection_DebugInfo) == 0x44c);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, CriticalSection_LockCount) == 0x450);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, CriticalSection_RecursionCount) == 0x454);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, CriticalSection_OwningThread) == 0x458);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, CriticalSection_LockSemaphore) == 0x45c);
FABLE_STATIC_ASSERT(offsetof(CHandleBasedMemoryPool, CriticalSection_SpinCount) == 0x460);

#endif // FABLE_ENGINE_CHANDLEBASEDMEMORYPOOL_H

#endif
