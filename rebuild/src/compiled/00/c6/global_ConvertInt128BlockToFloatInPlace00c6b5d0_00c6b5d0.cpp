// ConvertInt128BlockToFloatInPlace_00c6b5d0(float* block) -- libpk mixer: convert a block of 128 32-bit integers to floats in place, with SSE2 (cvtdq2ps, 8 x 4 per loop), else SSE+MMX (cvtpi2ps, 16 x 4, then emms), else the x87 fild/fstp loop; detects the CPU first when that has not been done
// Leaf: ?ConvertInt128BlockToFloatInPlace_00c6b5d0@@YAXPAM@Z
// Retail-asm: 0x00C6B5EA..0x00C6B622 `mov ecx,8 ; mov eax,[ebp+8] ; jmp ; cvtdq2ps xmm0,[eax] .. cvtdq2ps xmm3,[eax+30h] ; movaps [eax],xmm0 .. movaps [eax+30h],xmm3 ; add eax,40h ; dec ecx ; jne` and 0x00C6B62F..0x00C6B664 `mov ecx,10h ; mov eax,[ebp+8] ; jmp ; cvtpi2ps xmm0,[eax] .. cvtpi2ps xmm3,[eax+18h] ; movlps [eax],xmm0 .. movlps [eax+18h],xmm3 ; add eax,20h ; dec ecx ; jne ; emms` -- two hand SIMD conversion loops with `align 16` loop heads (the jmp-over filler), the in-place twin of the landed retail_asm ConvertInt128BlockToFloat 0x00C6B3A0 (same CPU flags, same detection call, same loops with one pointer); VC7.1 cannot emit cvtdq2ps/cvtpi2ps/movaps/movlps/emms from plain C++ and the intrinsic spellings allocate registers differently (rasm1 probe of the twin). The CPU-flag tests, the detection call and the scalar x87 fallback loop are C++.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C6B5D0, 219 bytes, __cdecl, plain `ret`, ebp frame (the asm blocks). Globals: the
//   CPU-detected flag 0x0143BC78, SSE2 flag 0x0143BC7D, SSE flag 0x0143BC7C; callee: the CPU detection 0x00C52E40.
// Name: descriptive, after the landed twin. The catalog's "Math_ConvertFloatsToIntegers" has the direction backwards, and the landed
//   caller CDecodedStream::PumpBlock 0x00C546E0 declares this address as `ResetDecoder(void*)` -- both are invented; the body only
//   converts its block in place (the caller passes the decoder's 128-sample block at +0x21C).
extern bool g_cpuDetected;                                  // 0x0143BC78
extern bool g_cpuHasSSE;                                    // 0x0143BC7C
extern bool g_cpuHasSSE2;                                   // 0x0143BC7D
void __cdecl DetectCpu();                                   // 0x00C52E40
#pragma optimize("y", off)
void __cdecl ConvertInt128BlockToFloatInPlace_00c6b5d0(float* block)
{
    if (!g_cpuDetected)
        DetectCpu();
    if (g_cpuHasSSE2) {
        __asm {
            mov     ecx, 8
            mov     eax, block
            align   16
        next2:
            cvtdq2ps xmm0, [eax]
            cvtdq2ps xmm1, [eax + 10h]
            cvtdq2ps xmm2, [eax + 20h]
            cvtdq2ps xmm3, [eax + 30h]
            movaps  [eax], xmm0
            movaps  [eax + 10h], xmm1
            movaps  [eax + 20h], xmm2
            movaps  [eax + 30h], xmm3
            add     eax, 40h
            dec     ecx
            jne     next2
        }
    } else if (g_cpuHasSSE) {
        __asm {
            mov     ecx, 16
            mov     eax, block
            align   16
        next1:
            cvtpi2ps xmm0, [eax]
            cvtpi2ps xmm1, [eax + 8]
            cvtpi2ps xmm2, [eax + 10h]
            cvtpi2ps xmm3, [eax + 18h]
            movlps  [eax], xmm0
            movlps  [eax + 8], xmm1
            movlps  [eax + 10h], xmm2
            movlps  [eax + 18h], xmm3
            add     eax, 20h
            dec     ecx
            jne     next1
            emms
        }
    } else {
        for (long i = 0; i < 128; ++i)          // the compiler unrolls it eight times (16 iterations in ecx)
            block[i] = (float)((long*)block)[i];
    }
}