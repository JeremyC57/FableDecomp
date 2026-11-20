// CMemoryAllocator::operator delete(void* p) -- the class operator delete: release the whole block with VirtualFree(p, 0, MEM_RELEASE)
// Leaf: ??3CMemoryAllocator@@KIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F1F90, 15 bytes, __fastcall (p in ecx), plain `ret`.
// Identity: Ego_r.pdb + FableWin.pdb ??3CMemoryAllocator@@KIXPAX@Z (protected static __fastcall; NF140 notes); the body is the
//   class operator delete that the CMemoryAllocator.cpp TU expands in its Deallocator teardown 0x0122A9A0 (NF145, engine/CMemoryAllocator.h
//   FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATOR), and the deallocating users (0x0122A9A0, 0x009A4350, 0x009C6020 ... ) bind this spelling.
// Fidelity: scaffolding, not retail. The body is the header inline (one definition, as landed 0x0122A9A0 emits it); this row takes its
//   address from an anonymous-namespace derived class (the member is protected: nf86 / nf88 form) so VC7.1 emits the pick-any COMDAT.
// Ported (NF205, 2026-11-20, q11879): was a free stand-in ?operator_delete@@YIXPAX@Z (no landed row binds it).
#define FABLE_RETAIL_CMEMORYALLOCATOR_CTOR
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATOR
#include "engine/CMemoryAllocator.h"

namespace {
struct CMemoryAllocatorDeleteReference : CMemoryAllocator {
    static void (__fastcall* Reference())(void*);
};
void (__fastcall* CMemoryAllocatorDeleteReference::Reference())(void*)
{
    return &CMemoryAllocator::operator delete;
}
}
