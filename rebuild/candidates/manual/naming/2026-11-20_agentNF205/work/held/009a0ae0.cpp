// CMemoryAllocator::`scalar deleting destructor'(unsigned int flags) -- run ~CMemoryAllocator, then (flags & 1) release the block through the class operator delete
// Leaf: ??_GCMemoryAllocator@@IAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009A0AE0, 25 bytes, thiscall, `ret 4`: push esi ; mov esi,ecx ; call ~CMemoryAllocator 0x009F2310 ;
//   test [esp+8],1 ; je ; mov ecx,esi ; call operator delete 0x009F1F90 ; mov eax,esi ; pop esi.
// Identity: the compiler-generated scalar deleting destructor of CMemoryAllocator (protected destructor, so `IAE`), the helper VC7.1 emits
//   for `delete PMemoryAllocator` in the inline CMemoryAllocator::Destroy (engine/CMemoryAllocator.h); the landed Deallocator teardown
//   0x0122A9A0 emits the same COMDAT ??_GCMemoryAllocator@@IAEPAXI@Z. Callees: ~CMemoryAllocator 0x009F2310 (Ego_r + FableWin
//   ??1CMemoryAllocator@@IAE@XZ), ??3CMemoryAllocator@@KIXPAX@Z 0x009F1F90.
// Fidelity: scaffolding, not retail. The body is compiler-generated; this row reaches it through the inline Destroy from an
//   anonymous-namespace derived class (the members are protected: nf86 / nf88 form) so VC7.1 emits the pick-any COMDAT.
// Ported (NF205, 2026-11-20, q11879): was an old row with no Leaf line defining the free stand-in
//   CMemoryAllocator_scalar_deleting_destructor over a local CMemoryAllocator model (callees ?Destroy@ / ?FreeThis@ stand-ins).
#define FABLE_RETAIL_CMEMORYALLOCATOR_CTOR
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#include "engine/CMemoryAllocator.h"

namespace {
struct CMemoryAllocatorDestroyReference : CMemoryAllocator {
    static void Reference();
};
void CMemoryAllocatorDestroyReference::Reference()
{
    CMemoryAllocator::Destroy();
}
}
