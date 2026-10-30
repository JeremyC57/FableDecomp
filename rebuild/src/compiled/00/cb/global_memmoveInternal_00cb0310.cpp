// memmoveInternal(void* dst, const void* src, unsigned int count) -- DirectShow wxutil.cpp overlap-safe memory move: forward dword+byte copy, or a backward byte copy when dst overlaps the tail of src
// Grade: MATCH retry1 (retail_asm) -- the DX9 SDK wxutil.cpp _X86_ body: two __asm blocks (forward rep movsd/movsb, backward std/rep movsb/cld)
// Leaf: ?memmoveInternal@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0310, 81 bytes (+ int3 padding). __stdcall (`ret 0Ch`) with an
// ebp frame (the inline-assembly blocks force it). No direct callers: wxutil.h's inline CopyMemory replacement
// reaches it through the strmbase sample-copy paths. The manifest label "MemMove" is descriptive.
// Retail-asm: 0x00CB0331 `std` / 0x00CB0353 `rep movsd` -- the DX9 SDK wxutil.cpp _X86_ blocks; VC7.1 has no std/cld/rep movs intrinsic.
// Retail-asm evidence: `std` (0x00CB0331) and `cld` (0x00CB033A, 0x00CB034A) have no VC7.1 intrinsic (the
// __movsb family arrived in VC8), and the DX9 SDK wxutil.cpp writes this function as `_asm { ... }` blocks under
// `#ifdef _X86_`. dst stays in eax from the overlap test to the return because the blocks only touch
// esi/edi/ecx/edx, so retail emits no `mov eax,[ebp+8]` for `return ret;`.
#define MEMORY_ALIGNMENT_LOG2 2
#define MEMORY_ALIGNMENT_MASK 3

void* __stdcall memmoveInternal(void* dst, const void* src, unsigned int count)
{
    void* ret = dst;

    if (dst <= src || (char*)dst >= ((char*)src + count)) {
        // Non-Overlapping Buffers: copy from lower addresses to higher addresses
        __asm {
            mov     esi,src
            mov     edi,dst
            mov     ecx,count
            cld
            mov     edx,ecx
            and     edx,MEMORY_ALIGNMENT_MASK
            shr     ecx,MEMORY_ALIGNMENT_LOG2
            rep     movsd
            or      ecx,edx
            jz      memmove_done
            rep     movsb
memmove_done:
        }
    }
    else {
        // Overlapping Buffers: copy from higher addresses to lower addresses
        __asm {
            mov     esi,src
            mov     edi,dst
            mov     ecx,count
            std
            add     esi,ecx
            add     edi,ecx
            dec     esi
            dec     edi
            rep     movsb
            cld
        }
    }

    return ret;
}