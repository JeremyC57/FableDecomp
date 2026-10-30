// ConvertFloat128BlockToInt(long* dst, const float* src) -- libpk mixer: round 128 floats from src to 32-bit integers in dst, with SSE2 (cvtps2dq, 8 x 4 per loop) or SSE+MMX (cvtps2pi, 16 x 2 x 4, then emms) under a forced round-to-nearest MXCSR (saved and restored), else the scalar FtoL loop; detects the CPU first when that has not been done
// Grade: DIFFER(315v315) -- scalar fallback only: retail loads the pushed float into ecx (`mov ecx,[esi+edx] ; push ecx`), VC7.1 picks eax; every other byte (both retail asm blocks, frame, flag tests) matches. Tried: unsigned and int index, *dst++/*src++ count loop (316v315), FtoL body after the caller (still keeps edx, still eax). Next: a float local / register-pressure variant of the call argument, or the 0xC6B250 HasSSE getter in the TU; grade retail_asm once it matches.
// Leaf: ?ConvertFloat128BlockToInt@@YAXPAJPBM@Z
// Retail-asm: 0x00C6B27C..0x00C6B2F1 `stmxcsr [ebp-4] ; mov esi,[ebp-4] ; stmxcsr ; and eax,0FFFF9FFFh ; ldmxcsr [ebp-4] ; and esi,6000h ; mov ecx,8 ; mov eax,[ebp+0Ch] ; mov edx,[ebp+8] ; jmp ; cvtps2dq xmm0,[eax] .. cvtps2dq xmm3,[eax+30h] ; movdqa [edx],xmm0 .. movdqa [edx+30h],xmm3 ; add eax,40h ; add edx,40h ; dec ecx ; jne ; stmxcsr .. or ecx,esi .. ldmxcsr` and 0x00C6B303..0x00C6B36A the same frame around `cvtps2pi mm0,[eax] .. cvtps2pi mm3,[eax+18h] ; movq [edx],mm0 .. movq [edx+18h],mm3 ; add eax,20h ; add edx,20h ; dec ecx ; jne ; emms` -- two hand SIMD rounding loops with `align 16` loop heads (the jmp-over `lea esp,[esp]` filler and the 1-byte nop) inside an MXCSR rounding-mode save / round-to-nearest / restore through the one frame local; VC7.1 C++ has no stmxcsr/ldmxcsr/cvtps2dq/movdqa/cvtps2pi/movq-mm/emms form, and the xmmintrin/emmintrin intrinsic spellings allocate their own registers and temporaries (rasm1 probe of the landed twin 0x00C6B3A0). The CPU-flag tests, the detection call and the scalar fallback loop are C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6B260, 315 bytes, __cdecl (dst [ebp+8], src [ebp+0Ch]), plain `ret` (three
//   epilogues), ebp frame with one local (the MXCSR word at [ebp-4]); esi holds the saved rounding bits in the asm blocks.
//   Globals: the CPU-detected flag 0x0143BC78, SSE2 flag 0x0143BC7D, SSE flag 0x0143BC7C; callees: the CPU detection
//   0x00C52E40 (`?DetectCpu@@YAXXZ`), FtoL 0x00C6B240 (`_FtoL`, same TU).
// Identity: the out-of-place twin of the landed retail_asm ConvertFloat128BlockToIntInPlace 0x00C6B490 (same flags, same
//   detection call, same MXCSR frame, same FtoL fallback) and the float-to-int counterpart of ConvertInt128BlockToFloat
//   0x00C6B3A0 (same (dst, src) argument order: dst [ebp+8] in edx, src [ebp+0Ch] in eax). No direct caller (a
//   function-pointer table entry, like its siblings). Name descriptive (libpk has no PDB); the manifest's
//   "FloatToLongConvert_Vectorized" is descriptive too.
// Shape: FtoL's body (the landed retail_asm 0x00C6B240) sits in this TU, so VC7.1 knows it leaves edx alone and keeps the
//   dst pointer in edx across the call, with src addressed as `[esi+edx]` (esi = src - dst) and the trip count in edi.
//   `optimize("y", off)` keeps the ebp frame (as in the twins).
extern bool g_cpuDetected;                                  // 0x0143BC78
extern bool g_cpuHasSSE;                                    // 0x0143BC7C
extern bool g_cpuHasSSE2;                                   // 0x0143BC7D
void __cdecl DetectCpu();                                   // 0x00C52E40
extern "C" long __cdecl FtoL(float f)                       // 0x00C6B240 (same TU)
{
    long result;
    __asm fld f
    __asm fistp result
    return result;
}
#pragma optimize("y", off)
void __cdecl ConvertFloat128BlockToInt(long* dst, const float* src)
{
    unsigned long csr;
    if (!g_cpuDetected)
        DetectCpu();
    if (g_cpuHasSSE2) {
        __asm {
            stmxcsr csr
            mov     esi, csr
            stmxcsr csr
            mov     eax, csr
            and     eax, 0FFFF9FFFh
            mov     csr, eax
            ldmxcsr csr
            and     esi, 6000h
            mov     ecx, 8
            mov     eax, src
            mov     edx, dst
            align   16
        next2:
            cvtps2dq xmm0, [eax]
            cvtps2dq xmm1, [eax + 10h]
            cvtps2dq xmm2, [eax + 20h]
            cvtps2dq xmm3, [eax + 30h]
            movdqa  [edx], xmm0
            movdqa  [edx + 10h], xmm1
            movdqa  [edx + 20h], xmm2
            movdqa  [edx + 30h], xmm3
            add     eax, 40h
            add     edx, 40h
            dec     ecx
            jne     next2
            stmxcsr csr
            mov     ecx, csr
            and     ecx, 0FFFF9FFFh
            or      ecx, esi
            mov     csr, ecx
            ldmxcsr csr
        }
    } else if (g_cpuHasSSE) {
        __asm {
            stmxcsr csr
            mov     esi, csr
            stmxcsr csr
            mov     edx, csr
            and     edx, 0FFFF9FFFh
            mov     csr, edx
            ldmxcsr csr
            and     esi, 6000h
            mov     ecx, 16
            mov     eax, src
            mov     edx, dst
            align   16
        next1:
            cvtps2pi mm0, [eax]
            cvtps2pi mm1, [eax + 8]
            cvtps2pi mm2, [eax + 10h]
            cvtps2pi mm3, [eax + 18h]
            movq    [edx], mm0
            movq    [edx + 8], mm1
            movq    [edx + 10h], mm2
            movq    [edx + 18h], mm3
            add     eax, 20h
            add     edx, 20h
            dec     ecx
            jne     next1
            emms
            stmxcsr csr
            mov     eax, csr
            and     eax, 0FFFF9FFFh
            or      eax, esi
            mov     csr, eax
            ldmxcsr csr
        }
    } else {
        for (unsigned long i = 0; i < 128; ++i)
            dst[i] = FtoL(src[i]);
    }
}
