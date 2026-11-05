// memmoveInternal(void* dst, const void* src, unsigned int count) -- DirectShow wxutil.cpp overlap-safe memory move: forward dword+byte copy, or a backward byte copy when dst overlaps the tail of src
// Grade: RELOCATION_MATCH -- retail_asm through the shared helper strmbase/memmoveInternal.h (2026-11-05 asm migration, docs/pipeline/ASM_RULES.md); no inline asm in this row
// Leaf: ?memmoveInternal@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0310, 81 bytes (+ int3 padding). __stdcall (`ret 0Ch`) with an
// ebp frame (the inline-assembly blocks force it). No direct callers: wxutil.h's inline CopyMemory replacement
// reaches it through the strmbase sample-copy paths. The manifest label "MemMove" is descriptive.
// Retail-asm: 0x00CB0331 `std` / 0x00CB0353 `rep movsd` -- the DX9 SDK wxutil.cpp _X86_ blocks; VC7.1 has no std/cld/rep movs intrinsic.
// Helper (2026-11-05 asm migration): the retail inline-asm block is the shared strmbase/memmoveInternal.h (byte-matching path + FABLE_PORTABLE twin);
//   the row has no asm of its own. C++/intrinsic attempts: the batch ATTEMPTS.tsv, family STD_REP_MOVS.
// Retail-asm evidence: `std` (0x00CB0331) and `cld` (0x00CB033A, 0x00CB034A) have no VC7.1 intrinsic (the
// __movsb family arrived in VC8), and the DX9 SDK wxutil.cpp writes this function as `_asm { ... }` blocks under
// `#ifdef _X86_`. dst stays in eax from the overlap test to the return because the blocks only touch
// esi/edi/ecx/edx, so retail emits no `mov eax,[ebp+8]` for `return ret;`.

#include "strmbase/memmoveInternal.h"
void* __stdcall memmoveInternal(void* dst, const void* src, unsigned int count)
{
    void* ret = dst;
    if (dst <= src || (char*)dst >= ((char*)src + count)) {
        // Non-Overlapping Buffers: copy from lower addresses to higher addresses
        STRMBASE_MEMMOVE_FORWARD(dst, src, count);
    }
    else {
        // Overlapping Buffers: copy from higher addresses to lower addresses
        STRMBASE_MEMMOVE_BACKWARD(dst, src, count);
    }

    return ret;
}