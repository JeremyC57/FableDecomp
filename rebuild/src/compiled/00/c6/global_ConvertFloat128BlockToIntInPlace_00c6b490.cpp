// ConvertFloat128BlockToIntInPlace(float* block) -- libpk mixer: round a block of 128 floats to 32-bit integers in place, with SSE2 (cvtps2dq, 8 x 4 per loop) or SSE+MMX (cvtps2pi, 16 x 2 x 4, then emms) under a forced round-to-nearest MXCSR (saved and restored), else the scalar FtoL loop; detects the CPU first when that has not been done
// Leaf: ?ConvertFloat128BlockToIntInPlace@@YAXPAM@Z
// Retail-asm: 0x00C6B4AB..0x00C6B522 `stmxcsr [ebp-4] .. ldmxcsr [ebp-4] ; and edx,6000h ; mov ecx,8 ; mov eax,[ebp+8] ; jmp ; cvtps2dq xmm0,[eax] .. cvtps2dq xmm3,[eax+30h] ; movdqa [eax],xmm0 .. movdqa [eax+30h],xmm3 ; add eax,40h ; dec ecx ; jne ; stmxcsr .. or ecx,edx .. ldmxcsr` and 0x00C6B52F..0x00C6B598 the same frame around `cvtps2pi mm0,[eax] .. cvtps2pi mm3,[eax+18h] ; movq [eax],mm0 .. movq [eax+18h],mm3 ; add eax,20h ; dec ecx ; jne ; emms` -- two hand SIMD rounding loops with `align 16` loop heads (the jmp-over `lea esp,[esp]` filler) inside an MXCSR rounding-mode save / round-to-nearest / restore through the one frame local; VC7.1 C++ has no stmxcsr/ldmxcsr/cvtps2dq/movdqa/cvtps2pi/movq-mm/emms form, and the xmmintrin/emmintrin intrinsic spellings allocate their own registers and temporaries (rasm1 probe of the landed twin 0x00C6B3A0). The CPU-flag tests, the detection call and the scalar fallback loop are C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6B490, 307 bytes, __cdecl, plain `ret` (three epilogues), ebp frame with one local
//   (the MXCSR word at [ebp-4]). Globals: the CPU-detected flag 0x0143BC78, SSE2 flag 0x0143BC7D, SSE flag 0x0143BC7C;
//   callees: the CPU detection 0x00C52E40, FtoL 0x00C6B240.
// Identity: the float-to-int counterpart of the landed retail_asm twins ConvertInt128BlockToFloat 0x00C6B3A0 /
//   ConvertInt128BlockToFloatInPlace_00c6b5d0 (same flags, same detection call, same loop frames; this one converts the
//   other way and switches the MXCSR rounding mode to nearest around the SSE loops). No caller references it directly
//   (a function-pointer table entry, like its siblings). Name descriptive (libpk has no PDB); the manifest's
//   "FloatArrayRound_CPU" is descriptive too.
// Shape: FtoL's body (the landed retail_asm 0x00C6B240) sits in this TU, so VC7.1 knows it leaves edx alone and keeps the
//   loop index in edx across the call (`xor edx,edx ... call ; mov [esi+edx*4],eax ; inc edx ; cmp edx,80h ; jb`); the
//   unsigned index gives retail's `jb`. `optimize("y", off)` keeps the ebp frame (as in the twins).
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
void __cdecl ConvertFloat128BlockToIntInPlace(float* block)
{
    unsigned long csr;
    if (!g_cpuDetected)
        DetectCpu();
    if (g_cpuHasSSE2) {
        __asm {
            stmxcsr csr
            mov     edx, csr
            stmxcsr csr
            mov     eax, csr
            and     eax, 0FFFF9FFFh
            mov     csr, eax
            ldmxcsr csr
            and     edx, 6000h
            mov     ecx, 8
            mov     eax, block
            align   16
        next2:
            cvtps2dq xmm0, [eax]
            cvtps2dq xmm1, [eax + 10h]
            cvtps2dq xmm2, [eax + 20h]
            cvtps2dq xmm3, [eax + 30h]
            movdqa  [eax], xmm0
            movdqa  [eax + 10h], xmm1
            movdqa  [eax + 20h], xmm2
            movdqa  [eax + 30h], xmm3
            add     eax, 40h
            dec     ecx
            jne     next2
            stmxcsr csr
            mov     ecx, csr
            and     ecx, 0FFFF9FFFh
            or      ecx, edx
            mov     csr, ecx
            ldmxcsr csr
        }
    } else if (g_cpuHasSSE) {
        __asm {
            stmxcsr csr
            mov     edx, csr
            stmxcsr csr
            mov     eax, csr
            and     eax, 0FFFF9FFFh
            mov     csr, eax
            ldmxcsr csr
            and     edx, 6000h
            mov     ecx, 16
            mov     eax, block
            align   16
        next1:
            cvtps2pi mm0, [eax]
            cvtps2pi mm1, [eax + 8]
            cvtps2pi mm2, [eax + 10h]
            cvtps2pi mm3, [eax + 18h]
            movq    [eax], mm0
            movq    [eax + 8], mm1
            movq    [eax + 10h], mm2
            movq    [eax + 18h], mm3
            add     eax, 20h
            dec     ecx
            jne     next1
            emms
            stmxcsr csr
            mov     ecx, csr
            and     ecx, 0FFFF9FFFh
            or      ecx, edx
            mov     csr, ecx
            ldmxcsr csr
        }
    } else {
        for (unsigned long i = 0; i < 128; ++i)
            ((long*)block)[i] = FtoL(block[i]);
    }
}