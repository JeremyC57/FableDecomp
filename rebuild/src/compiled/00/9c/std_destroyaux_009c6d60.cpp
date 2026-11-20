// std::__destroy_aux(CFileInstaller::CBufferChunk* first, CFileInstaller::CBufferChunk* last, const __false_type&) -- run the inlined ~CBufferChunk over [first, last): give each chunk's memory back to the global allocator
// Inline: the header inlines CMemoryAllocator::Deallocate and Destroy (engine/CMemoryAllocator.h opt-in
//   FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE) are plain inline; VC7.1 expands both here at its default threshold (NF145 sweep:
//   MATCH with no inline control; the nf140 FABLE_FORCEINLINE is gone from the header).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C6D60, length 123 bytes (padded to 128).
// Leaf: __destroy_aux
// Label: the catalog "_Destroy_range<std::allocator<CFileInstaller::CBufferChunk>>" names the element correctly: the
// stride is 8 = Ego_r.pdb CBufferChunk {Mem +0, Used +4}; as an STLport out-of-line body it is `__destroy_aux(first,
// last, const __false_type&)` (nine callers in the CFileInstaller code, 0x009C52E7 ...).
// Convention: LTCG register form -- first in ecx, last in edx, the tag pointer on the stack (`ret 4`).
// Callees: CMemoryAllocator::EnterSynchronisation @ 0x009F2000 / CMemoryAllocator::LeaveSynchronisation @ 0x009F2010, CMemoryAllocator::InternalDeallocate
//          @ 0x009F2920 / GetAllocationCount @ 0x009F1F10 / ~CMemoryAllocator @ 0x009F2310 / operator delete @ 0x009F1F90.
// Globals: the global allocator @ 0x013CAA90 and its destroy-when-empty flag @ 0x013CAA94.
// Shape: the chunk destructor tests Mem and calls the landed GFFreeFromGlobalAllocator body (0x009A4350) inline, whose
// own null test survives (`test esi,esi` twice) because it sits after the lock call.
#include "fable_compiler.h"
// NF140 (q4197 / q4198): the allocation is the inline public static CMemoryAllocator::Allocate(size, flags) of engine/CMemoryAllocator.h
// (Ego_r.pdb / FableWin.pdb ?Allocate@CMemoryAllocator@@SIPAXKK@Z, inline in release): ?Create@CMemoryAllocator@@KIXXZ 0x009F2870, the protected
// statics ?EnterSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2000 / ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ 0x009F2010 (both PDBs),
// PMemoryAllocator 0x013CAA90 and InternalAllocate 0x009F2A40, as the landed 0x009A3640 binds them.
// NF140 (q4197 / q4198): the frees are the inline public static CMemoryAllocator::Deallocate(block) (FableWin.pdb ?Deallocate@CMemoryAllocator@@SIXPAX@Z,
// out of line at 0x009A4350; engine/CMemoryAllocator.h opt-in FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE, the body of the landed 0x009F2B20).
#define FABLE_RETAIL_CMEMORYALLOCATOR_DEALLOCATE
#include "engine/CMemoryAllocator.h"
class CFileInstaller { public:
    class CBufferChunk { public:                          // Ego_r.pdb, 8 bytes
        unsigned char* Mem;                        // +0x00
        bool           Used;                       // +0x04
        ~CBufferChunk()
        {
            if (Mem)
                CMemoryAllocator::Deallocate(Mem);
        }
    };
};
namespace std {
struct __false_type { };
template <class T> inline void _Destroy(T* p) { p->~T(); }
void __fastcall __destroy_aux(CFileInstaller::CBufferChunk* first, CFileInstaller::CBufferChunk* last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
}