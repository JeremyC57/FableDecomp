#ifndef FABLE_ENGINE_CMEMORYALLOCATOR_H
#define FABLE_ENGINE_CMEMORYALLOCATOR_H
// Declaration-only API projection of the original class: no object construction, sizeof, or field access. This is not a storage-layout definition.
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_ALIGNED
#include "rebuild_abi.h"
#endif
#if defined(FABLE_RETAIL_CMEMORYALLOCATOR_ALLOCATE_FORCEINLINE) || defined(FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE)
#include "fable_compiler.h"
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_CTOR
// Retail opt-in (wf14f, row 0x009F20B0): with this macro the class carries its Ego_r.pdb storage (0x49c bytes), the
// allocator-info records and the protected constructor, so the constructor row can be written against this header.
class CMemoryAllocatorBase;
struct CAllocatorInfo {                          // Ego_r.pdb: 28 bytes
    long                  MaxAllocSize;          // +0x00
    long                  MinChunkSize;          // +0x04
    long                  InitialSize;           // +0x08
    long                  Alignment;             // +0x0c
    unsigned long         Flags;                 // +0x10 allocator type index (0..14)
    unsigned long         InternalFlags;         // +0x14 bit 1: fixed-size allocator
    CMemoryAllocatorBase* PAllocator;            // +0x18
};
struct CAllocSizeToAllocator {                   // Ego_r.pdb: 8 bytes
    CMemoryAllocatorBase* PAllocator;            // +0x00
    unsigned char         ID;                    // +0x04
};
// The allocator-info table (retail 0x01396868, 30 records). No PDB name was found for it: descriptive name, the same
// spelling the landed CMemoryAllocator rows use.
extern CAllocatorInfo g_AllocatorInfo[];
#endif
class CMemoryAllocator {
protected:
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_CTOR
 unsigned long          MemoryAllocatedPerTrackingID[256];   // +0x000
 unsigned long          MemoryAllocated;                     // +0x400
 void**                 MemoryAuditBuffer;                   // +0x404
 void*                  Heap;                                // +0x408
 unsigned long          NoAllocations;                       // +0x40c
 unsigned long          NoAllocators;                        // +0x410
 unsigned char          CurrentTrackingID;                   // +0x414
 bool                   AllocatorExists[15];                 // +0x415
 long                   MaxFixedAllocationSizes[15];         // +0x424
 CAllocSizeToAllocator* AllocSizeToAllocator[15];            // +0x460
 // Ego_r.pdb ??0CMemoryAllocator@@IAE@PAX@Z (retail 0x009F20B0).
 CMemoryAllocator(void* heap);
#endif
 static CMemoryAllocator* PMemoryAllocator;
 static void __fastcall Create();
 static void __fastcall EnterSynchronisation();
 static void __fastcall LeaveSynchronisation();
 void* InternalAllocate(long,unsigned long,bool);
public:
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_SAVEBINARY_PASS23
 // Both exact original PDB PUBLICs; actual debug caller confirms these two statistics.
 unsigned long GetTotalMemoryAllocated();
 unsigned long GetNoAllocatedAreas();
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_VARIABLESIZE
 // Original FableWin Get018a4e40; OnFailedAllocation030bf3a0.
 static CMemoryAllocator* FABLE_FASTCALL Get(){return PMemoryAllocator;}
 void OnFailedAllocation();
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_ALIGNED
 // Opt-in (chunk 892, row 0x00B482A0): Ego_r.pdb ?AllocateAligned@CMemoryAllocator@@SIPAXKJ@Z (0x009F2BE0, landed) and
 // ?DeallocateAligned@CMemoryAllocator@@SIXPAX@Z (0x009F2B20, landed), public static __fastcall. Declarations only.
 static void* FABLE_FASTCALL AllocateAligned(unsigned long size,long alignment);
 static void FABLE_FASTCALL DeallocateAligned(void* p);
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_ALLOCATE_FORCEINLINE
 // NF140 opt-in (q4197 row 0x009D52D0 CBankFileAsyncData::SetupBuffer): retail expands Allocate in place past VC7.1's
 // inline budget in that EH function; the landed row spelled the body as a FABLE_FORCEINLINE helper. Inline control only (EXTRA_RULES
 // 2026-11-08); no declaration changes.
 FABLE_FORCEINLINE
#endif
 static void* __fastcall Allocate(unsigned long size,unsigned long flags) {
  if(!PMemoryAllocator)Create();
  EnterSynchronisation();
  void* p=PMemoryAllocator->InternalAllocate(size,flags,false);
  LeaveSynchronisation();return p;
 }
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
 // NF140 opt-in (q4197 / q4198, the deallocating binders of 0x009F2000 / 0x009F2010): the inline public static
 // ?Deallocate@CMemoryAllocator@@SIXPAX@Z (FableWin.pdb 0x02CB6E30; no Ego_r copy: inline in release) and what it reaches, spellings
 // from Ego_r.pdb / FableWin.pdb: ?GetNoAllocations@CMemoryAllocator@@QAEKXZ (0x009F1F10), ??1CMemoryAllocator@@IAE@XZ (0x009F2310),
 // ??3CMemoryAllocator@@KIXPAX@Z (0x009F1F90), ?InternalDeallocate@CMemoryAllocator@@IAEXPAX@Z (0x009F2920), ?Destroy@CMemoryAllocator@@KIXXZ
 // (forced inline here: the retail callers delete the allocator in place). The destroy-when-empty flag (0x013CAA94) has no PDB data name: the
 // spelling of the landed CMemoryAllocator::DeallocateAligned 0x009F2B20. The body is that row's Deallocate.
 unsigned long GetNoAllocations();                                       // 0x009F1F10
 // Deallocate: FABLE_FORCEINLINE as in the landed 0x009F2B20 (with Destroy expanded, a plain inline is called out of line: 0x00BD9B90 DIFFER 16v97).
 static FABLE_FORCEINLINE void __fastcall Deallocate(void* block) {
  EnterSynchronisation();
  if(block&&PMemoryAllocator){
   PMemoryAllocator->InternalDeallocate(block);
   if(DestroyWhenEmpty&&PMemoryAllocator->GetNoAllocations()==0)Destroy();
  }
  LeaveSynchronisation();
 }
protected:
 static bool DestroyWhenEmpty;                                            // 0x013CAA94
 ~CMemoryAllocator();                                                     // 0x009F2310
 static void __fastcall operator delete(void* p);                         // 0x009F1F90
 void InternalDeallocate(void* block);                                    // 0x009F2920
 // Destroy: FABLE_FORCEINLINE as in the landed 0x009F2B20 (a plain inline is called out of line from Deallocate: 0x00BD9B90 DIFFER 68v97).
 static FABLE_FORCEINLINE void __fastcall Destroy(){delete PMemoryAllocator;PMemoryAllocator=0;}
public:
#endif
};
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_CTOR
#ifndef FABLE_RETAIL_CMEMORYALLOCATOR_SAVEBINARY_HOST_PASS23
typedef char CMemoryAllocator_size_is_0x49c[sizeof(CMemoryAllocator) == 0x49c ? 1 : -1];
#endif
#endif
#endif
