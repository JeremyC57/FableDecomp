// memmoveInternal(void* dst, const void* src, unsigned int count) -- DirectShow wxutil.cpp overlap-safe memory move: forward dword+byte copy, or a backward byte copy when dst overlaps the tail of src
// Grade: DIFFER(73v81) -- retail_asm candidate: the original body is two inline-assembly blocks (`cld ; rep movsd ; rep movsb` forward, `std ; rep movsb ; cld` backward) that VC7.1 C++ cannot emit
// Leaf: ?memmoveInternal@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0310, 81 bytes (+ int3 padding). __stdcall (`ret 0Ch`) with an
// ebp frame (the inline-assembly blocks force it). No direct callers: wxutil.h's inline CopyMemory replacement
// reaches it through the strmbase sample-copy paths. The manifest label "MemMove" is descriptive.
// Retail-asm evidence: `std` (0x00CB0331) and `cld` (0x00CB033A, 0x00CB034A) have no VC7.1 intrinsic (the
// __movsb family arrived in VC8), and the DX9 SDK wxutil.cpp writes this function as `_asm { ... }` blocks under
// `#ifdef _X86_`. dst stays in eax from the overlap test to the return because the blocks only touch
// esi/edi/ecx/edx, so retail emits no `mov eax,[ebp+8]` for `return ret;`.
// The body below is a C++ reference with the same behaviour, not the retail formulation.
#include <string.h>

void* __stdcall memmoveInternal(void* dst, const void* src, unsigned int count)
{
    void* ret = dst;

    if (dst <= src || (char*)dst >= ((char*)src + count)) {
        // Non-Overlapping Buffers: copy from lower addresses to higher addresses
        memcpy(dst, src, count);
    }
    else {
        // Overlapping Buffers: copy from higher addresses to lower addresses
        char* d = (char*)dst + count - 1;
        const char* s = (const char*)src + count - 1;
        while (count--)
            *d-- = *s--;
    }

    return ret;
}
