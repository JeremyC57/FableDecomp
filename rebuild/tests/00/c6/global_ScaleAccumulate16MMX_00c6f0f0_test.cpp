#include <cstdio>
// behaviour test of ScaleAccumulate16MMX (0x00c6f0f0): dst[i] += (high word of src[i] * gain) * 2 over count blocks of eight 16-bit samples (MMX)
#ifdef _MSC_VER
#define ALIGN16 __declspec(align(16))
typedef long long32;
void __cdecl ScaleAccumulate16MMX(const short* src, unsigned long count, long* dst, short gain)
{
    __declspec(align(16)) short gains[4];
    gains[0] = gain; gains[1] = gain; gains[2] = gain; gains[3] = gain;
    __asm {
        mov     ecx, count
        mov     eax, src
        mov     edx, dst
        movq    mm7, gains
        align   16
    next:
        movq    mm0, [eax]
        pmulhw  mm0, mm7
        pxor    mm1, mm1
        pxor    mm2, mm2
        punpcklwd mm1, mm0
        punpckhwd mm2, mm0
        psrad   mm1, 15
        psrad   mm2, 15
        paddd   mm1, [edx]
        paddd   mm2, [edx + 8]
        movq    [edx], mm1
        movq    [edx + 8], mm2
        movq    mm3, [eax + 8]
        pmulhw  mm3, mm7
        pxor    mm4, mm4
        pxor    mm5, mm5
        punpcklwd mm4, mm3
        punpckhwd mm5, mm3
        psrad   mm4, 15
        psrad   mm5, 15
        paddd   mm4, [edx + 10h]
        paddd   mm5, [edx + 18h]
        movq    [edx + 10h], mm4
        add     eax, 10h
        movq    [edx + 18h], mm5
        add     edx, 20h
        dec     ecx
        jne     next
        emms
    }
}
#else
#define ALIGN16 __attribute__((aligned(16)))
typedef int long32;
void ScaleAccumulate16MMX(const short* src, unsigned long count, long32* dst, short gain)
{
    for (unsigned long b = 0; b < count; ++b)
        for (int i = 0; i < 8; ++i) {
            short high = (short)(((long32)src[b * 8 + i] * gain) >> 16);       // pmulhw
            dst[b * 8 + i] += ((long32)high << 16) >> 15;                      // unpack into the high word, arithmetic >> 15
        }
}
#endif
int main()
{
    ALIGN16 short src[24]; ALIGN16 long32 dst[25];
    for (int i = 0; i < 24; ++i) { src[i] = (short)(i * 1000 - 12000); dst[i] = 5; }
    dst[24] = 99;
    ScaleAccumulate16MMX(src, 3, dst, 16384);                    // gain 0.5 in Q15
    bool ok = dst[24] == 99;
    for (int i = 0; i < 24; ++i) {
        long32 high = ((long32)src[i] * 16384) >> 16;
        long32 expect = 5 + high * 2;
        ok = ok && dst[i] == expect;
    }
    // (the kernel counts down with `dec ; jne`: a zero count is never passed by the mixer)
    std::printf(ok ? "00c6f0f0_TEST PASS\n" : "00c6f0f0_TEST FAIL\n");
    return ok ? 0 : 1;
}