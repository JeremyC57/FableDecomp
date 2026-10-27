// AddFloat128BlockToInt(const float* src, long* dst) -- libpk mixer: convert 128 floats to 32-bit integers (cvtps2dq, rounding mode forced to nearest through MXCSR) and add them into dst, restoring the caller's rounding mode afterwards
// Leaf: ?AddFloat128BlockToInt@@
// Retail-asm: 0x00C5D2E4..0x00C5D364 `stmxcsr [ebp-4] ; mov esi,[ebp-4] ; stmxcsr [ebp-4] ; mov eax,[ebp-4] ; and eax,0FFFF9FFFh ; mov [ebp-4],eax ; ldmxcsr [ebp-4] ; and esi,6000h ; mov ecx,8 ; mov eax,[ebp+8] ; mov edx,[ebp+0Ch] ; cvtps2dq xmm0,[eax] .. cvtps2dq xmm3,[eax+30h] ; paddd xmm0,[edx] .. ; movdqa [edx],xmm0 .. ; add eax,40h ; add edx,40h ; dec ecx ; jne ; stmxcsr [ebp-4] ; mov ecx,[ebp-4] ; and ecx,0FFFF9FFFh ; or ecx,esi ; mov [ebp-4],ecx ; ldmxcsr [ebp-4]` -- a hand SSE2 kernel (19 instructions, the loop jne, `align 16` on the loop head: rasm1 family (f)) between single-instruction `__asm stmxcsr csr` / `__asm ldmxcsr csr` statements over one named local; the rounding-control arithmetic (`and eax,0FFFF9FFFh`, `and esi,6000h`, `or ecx,esi`) is C++ on that local and on `saved`, which the compiler keeps in esi (its `push esi` follows the first stmxcsr). `_mm_getcsr`/`_mm_setcsr` spell the save as register copies with a different frame (rasm1 probe); the stmxcsr-twice idiom is hand written.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D2E0, 141 bytes (+3 int3), __cdecl, plain `ret`; called from 0x00C5D3DA and
//   0x00C5D49A (the __stdcall mixer wrappers). The catalog name "Math_AccumulateFloatArray" is descriptive.
#pragma optimize("y", off)
void __cdecl AddFloat128BlockToInt(const float* src, long* dst)
{
    unsigned long csr;
    unsigned long saved;
    __asm stmxcsr csr
    saved = csr & 0x6000;                 // the caller's rounding control, kept in esi by the compiler (live across the asm blocks)
    __asm {
        stmxcsr csr
        mov     eax, csr
        and     eax, 0FFFF9FFFh           // round to nearest for cvtps2dq
        mov     csr, eax
        ldmxcsr csr
    }
    __asm {
        mov     ecx, 8
        mov     eax, src
        mov     edx, dst
        align   16
    next:
        cvtps2dq xmm0, [eax]
        cvtps2dq xmm1, [eax + 10h]
        cvtps2dq xmm2, [eax + 20h]
        cvtps2dq xmm3, [eax + 30h]
        paddd   xmm0, [edx]
        paddd   xmm1, [edx + 10h]
        paddd   xmm2, [edx + 20h]
        paddd   xmm3, [edx + 30h]
        movdqa  [edx], xmm0
        movdqa  [edx + 10h], xmm1
        movdqa  [edx + 20h], xmm2
        movdqa  [edx + 30h], xmm3
        add     eax, 40h
        add     edx, 40h
        dec     ecx
        jne     next
    }
    __asm stmxcsr csr
    csr &= 0xFFFF9FFF;                    // restore the caller's rounding control
    csr |= saved;
    __asm ldmxcsr csr
}
