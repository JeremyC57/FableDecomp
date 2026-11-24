// CEngineLandscapeMemoryManager::FreeVirtual(void* block) -- returns a block to the global CMemoryAllocator under the allocator lock and, when the free-when-empty flag is set and nothing is left allocated, destroys the global allocator
// Flags: /d2inlT114
// Leaf: ?FreeVirtual@CEngineLandscapeMemoryManager@@QAEXPAX@Z
// Inline-budget: /d2inlT114 (EXTRA_RULES 2026-11-08 preference 2; replaces the nf140 FABLE_FORCEINLINE in the header) -- retail
//   expands the header inlines CMemoryAllocator::Deallocate and Destroy (engine/CMemoryAllocator.h opt-in
//   FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE, plain inline) here; at the default threshold both stay calls (DIFFER 68v97).
//   Sweep NF145: T74..T113 all DIFFER, T114 is the smallest match (T114..T2000 match on 0x00BD9B90; /d2inlS85 also matches there).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BD9B90, length 97 bytes (`ret 4`; `this` unused).
// Callees: GFEnterAllocatorLock @ 0x009F2000 / GFLeaveAllocatorLock @ 0x009F2010, CMemoryAllocator::InternalDeallocate @ 0x009F2920,
// the allocator's allocation-count query @ 0x009F1F10 (manifest "CWorld::DrawGetEnvironment", propagated), ~CMemoryAllocator
// @ 0x009F2310 and the class `operator delete` @ 0x009F1F90 (`__fastcall`).
// Globals: the global allocator pointer @ 0x013CAA90 (as the landed AllocVirtual 0x00BD9AF0) and the byte flag @ 0x013CAA94.
// Shape: `delete g` through a class `static void __fastcall operator delete(void*)` (GOTCHAS 24h: `mov ecx,[g] ; test ; mov esi,ecx ;
// call dtor ; mov ecx,esi ; call`), the global re-read after each call, then the explicit null store.
// NF140 (q4197 / q4198): the free is the inline public static CMemoryAllocator::Deallocate(block) of engine/CMemoryAllocator.h
// (FableWin.pdb ?Deallocate@CMemoryAllocator@@SIXPAX@Z; opt-in FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE): EnterSynchronisation 0x009F2000 /
// LeaveSynchronisation 0x009F2010 (both PDBs ...@@KIXXZ), InternalDeallocate, GetNoAllocations, the inline Destroy (~CMemoryAllocator +
// operator delete), PMemoryAllocator 0x013CAA90; the outer null test is the caller's (the inner one folds).
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#include "engine/CMemoryAllocator.h"


class CEngineLandscapeMemoryManager { public:
    void FreeVirtual(void* block);
};

inline void CEngineLandscapeMemoryManager::FreeVirtual(void* block)
{
    if (block)
        CMemoryAllocator::Deallocate(block);
}

namespace {
// Fidelity: scaffolding, not retail (NF276, D4; the nf86 / nf88 form): FreeVirtual is defined `inline` and its address taken here, so this
// object emits it as a pick-any COMDAT (bytes unchanged) like its TU-mate copies in 0x00BDA150 / 0x00BD9CE0, instead of a strong definition
// that every link meets with LNK4006 against those copies. Internal to this TU; never called.
void (CEngineLandscapeMemoryManager::*ReferenceFreeVirtual())(void*)
{
    return &CEngineLandscapeMemoryManager::FreeVirtual;
}
}
