// CMemoryAllocator::DestroyDeallocator() -- the dynamic atexit destructor for the static `CMemoryAllocator::Deallocator`: at exit flag the allocator as shutting down and, when nothing is left allocated, destroy the global allocator and release its pages
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0122A9A0, 61 bytes (+3 int3), in `.text$yd`; its dynamic initializer
//   @ 0x01224240 only registers it with atexit (CDeallocator is empty). VC7.1 names the pair `$E1` / `$E2` in the TU.
// Identity: FableWin.pdb lists "`dynamic atexit destructor for 'CMemoryAllocator::Deallocator''" after the console's
//   OPEN_BRACKET / CLOSE_BRACKET / SEPARATOR strings, as retail's thunk follows three CCharString thunks
//   (0x0122A8D0..0x0122A8F0); `??1CDeallocator@CMemoryAllocator@@QAE@XZ`, `?Destroy@CMemoryAllocator@@KIXXZ` and
//   `??3CMemoryAllocator@@KIXPAX@Z` are its PDB members. The same teardown sits in the header's inline CMemoryAllocator::Deallocate.
// NF145 (q11809): on engine/CMemoryAllocator.h (opt-ins _CTOR for the storage, _DEALLOCATE for the inline Destroy, ~CMemoryAllocator,
//   operator delete and the destroy-when-empty flag, _DEALLOCATOR for CDeallocator / Deallocator and operator delete's body, which this
//   TU holds). The globals take the header spellings ?PMemoryAllocator@CMemoryAllocator@@1PAV1@A (0x013CAA90) and
//   ?DestroyWhenEmpty@CMemoryAllocator@@1_NA (0x013CAA94), which every other landed binder uses; the row's old PAllocator /
//   ShuttingDown had no source. The count read at +0x40C is the header's NoAllocations (Ego_r.pdb storage; GetNoAllocations
//   0x009F1F10 reads the same word), read directly: which accessor retail's source called here is not proven.
// Callees: ~CMemoryAllocator @ 0x009F2310, VirtualFree (KERNEL32 import [0x0143FD40]) through the class's operator delete.
// Shape: ~CDeallocator -> Destroy() -> `delete PMemoryAllocator` all inline: the null test of the delete folds into the outer
//   one (the pointer stays in ecx, then esi), the flag store does not force a reload, and the class operator delete is
//   `VirtualFree(p, 0, MEM_RELEASE)`.
#define FABLE_RETAIL_CMEMORYALLOCATOR_CTOR
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATOR
#include "engine/CMemoryAllocator.h"

CMemoryAllocator::CDeallocator::~CDeallocator()
{
    if (PMemoryAllocator) {
        DestroyWhenEmpty = true;
        if (PMemoryAllocator->NoAllocations == 0)
            Destroy();
    }
}

CMemoryAllocator::CDeallocator CMemoryAllocator::Deallocator;
