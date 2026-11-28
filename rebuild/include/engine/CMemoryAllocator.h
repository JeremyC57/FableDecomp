#ifndef FABLE_ENGINE_CMEMORYALLOCATOR_H
#define FABLE_ENGINE_CMEMORYALLOCATOR_H
// Declaration-only API projection of the original class: no object construction, sizeof, or field access. This is not a storage-layout definition.
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_ALIGNED
#include "rebuild_abi.h"
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATOR
extern "C" __declspec(dllimport) int __stdcall VirtualFree(void* address, unsigned long size, unsigned long freeType);   // [0x0143FD40]
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
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_SYNCHRONISATION_PASS30
#include "engine/Win32Types.h"
// In FableWin and Ego_r lib_memoryallocator.obj, the lock is a file-level CriticalSection.
extern CRITICAL_SECTION CriticalSection;
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
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_SYNCHRONISATION_PASS30
 // ClearPerformanceStats 0x009F26B0 expands both synchronization members in retail.
 static void __fastcall EnterSynchronisation() { EnterCriticalSection(&CriticalSection); }
 static void __fastcall LeaveSynchronisation() { LeaveCriticalSection(&CriticalSection); }
#else
 static void __fastcall EnterSynchronisation();
 static void __fastcall LeaveSynchronisation();
#endif
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
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_GETALLOCATIONSIZE
 // NF161 opt-in (q4196, rows 0x00A67F60 / 0x00A682E0 / 0x00A68370): Ego_r.pdb / FableWin.pdb ?GetAllocationSize@CMemoryAllocator@@QAEKPAX@Z
 // (public, landed 0x009F2390). Declaration only.
 unsigned long GetAllocationSize(void* block);                           // 0x009F2390
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_INTERNALVALIDATEAREA
 // NF161 opt-in (q4196, rows 0x00A65330 / 0x00A66260): Ego_r.pdb / FableWin.pdb ?InternalValidateArea@CMemoryAllocator@@QAEXPAX@Z (public),
 // retail 0x009F1F40 (empty `ret 4` in release); Ego_r donor order GetNoAllocations / SetCurrentTrackingID / GetCurrentTrackingID /
 // InternalValidateArea / InternalValidate = retail 0x009F1F10 / 1F20 / 1F30 / 1F40 / 1F50 (bodies: +0x40c read, +0x414 byte store
 // `ret 4`, +0x414 movzx read, `ret 4`, `ret`). Declaration only.
 void InternalValidateArea(void* block);                                 // 0x009F1F40
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_SAVESTACKMEMORYTRACKERLOG
 // NF161 opt-in (q4196, row 0x00415E85 GFLogMemoryUse): Ego_r.pdb / FableWin.pdb ?SaveStackMemoryTrackerLog@CMemoryAllocator@@QAEXPBD@Z
 // (public), retail 0x009F2080 (empty `ret 4` in release); Ego_r donor order Leave / PeekAllocatorMemoryControlled /
 // PeekAllocatorMemoryAllocated / IsAllocatorFixedSize / SaveStackMemoryTrackerLog = retail 0x009F2010 / 2020 / 2040 / 2060 / 2080.
 // Declaration only.
 void SaveStackMemoryTrackerLog(const char* file_name);                  // 0x009F2080
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_CONSOLIDATEFREEAREAS
 // NF161 opt-in (q4196, row 0x004189C2 CMainGameComponent::Run): Ego_r.pdb / FableWin.pdb ?ConsolidateFreeAreas@CMemoryAllocator@@QAEXXZ
 // (public, retail 0x009F2660). Declaration only.
 void ConsolidateFreeAreas();                                            // 0x009F2660
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_CLEARPERFORMANCESTATS
 // NF161 opt-in (q4196, row 0x004189C2 CMainGameComponent::Run): Ego_r.pdb / FableWin.pdb ?ClearPerformanceStats@CMemoryAllocator@@QAEXXZ
 // (public, retail 0x009F26B0; the landed binder's spelling). Declaration only.
 void ClearPerformanceStats();                                           // 0x009F26B0
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_NEWFRAME
 // NF161 opt-in (q4196, row 0x00416047): Ego_r.pdb / FableWin.pdb ?NewFrame@CMemoryAllocator@@QAEXXZ (public), retail 0x009F26D0 (Ego_r donor
 // order ClearPerformanceStats / NewFrame = retail 0x009F26B0 / 0x009F26D0, both the bare Enter/LeaveSynchronisation pair in release).
 // Declaration only.
 void NewFrame();                                                        // 0x009F26D0
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_ALIGNED
 // Opt-in (chunk 892, row 0x00B482A0): Ego_r.pdb ?AllocateAligned@CMemoryAllocator@@SIPAXKJ@Z (0x009F2BE0, landed) and
 // ?DeallocateAligned@CMemoryAllocator@@SIXPAX@Z (0x009F2B20, landed), public static __fastcall. Declarations only.
 static void* FABLE_FASTCALL AllocateAligned(unsigned long size,long alignment);
 static void FABLE_FASTCALL DeallocateAligned(void* p);
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
 // (inline here: the retail callers delete the allocator in place). The destroy-when-empty flag (0x013CAA94) has no PDB data name: the
 // spelling of the landed CMemoryAllocator::DeallocateAligned 0x009F2B20. The body is that row's Deallocate.
 unsigned long GetNoAllocations();                                       // 0x009F1F10
 // Deallocate / Destroy: plain inline (NF145, EXTRA_RULES 2026-11-08 preference 2): the users whose retail expands both past VC7.1's
 // default inline threshold carry a per-row `// Flags: /d2inlT114` (sweep: naming/2026-11-20_agentNF145/NOTES.md), not a source-level force.
 static inline void __fastcall Deallocate(void* block) {
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
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATOR
 // NF145 opt-in (with _DEALLOCATE; row 0x0122A9A0, the atexit destructor of Deallocator in CMemoryAllocator.cpp): that TU holds the
 // class operator delete's body (VirtualFree MEM_RELEASE, out-of-line copy 0x009F1F90), and retail expands it there.
 static void __fastcall operator delete(void* p) { VirtualFree(p, 0, 0x8000 /* MEM_RELEASE */); }
#else
 static void __fastcall operator delete(void* p);                         // 0x009F1F90
#endif
 void InternalDeallocate(void* block);                                    // 0x009F2920
 static inline void __fastcall Destroy(){delete PMemoryAllocator;PMemoryAllocator=0;}
public:
#endif
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATOR
 // NF145 opt-in (row 0x0122A9A0): the static teardown object. FableWin.pdb ??1CDeallocator@CMemoryAllocator@@QAE@XZ and
 // "`dynamic atexit destructor for 'CMemoryAllocator::Deallocator''"; the class is empty (its initializer 0x01224240 only registers
 // the atexit thunk). Class key and the member's access as the landed row had them (no PDB data record).
 class CDeallocator {
 public:
  ~CDeallocator();
 };
 static CDeallocator Deallocator;
#endif
};
#ifdef FABLE_RETAIL_CMEMORYALLOCATOR_CTOR
#ifndef FABLE_RETAIL_CMEMORYALLOCATOR_SAVEBINARY_HOST_PASS23
typedef char CMemoryAllocator_size_is_0x49c[sizeof(CMemoryAllocator) == 0x49c ? 1 : -1];
#endif
#endif
#endif
