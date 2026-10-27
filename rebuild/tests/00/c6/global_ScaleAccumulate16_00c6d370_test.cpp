#include <cstdio>
// behaviour test of ScaleAccumulate16 (0x00c6d370): dst[i] += (high word of src[i] * gain) * 2 over count blocks of eight 16-bit samples (SSE2)
#ifdef _MSC_VER
#define ALIGN16 __declspec(align(16))
typedef long long32;
void __cdecl ScaleAccumulate16(const short* src, unsigned long count, long* dst, short gain)
{
    __declspec(align(16)) short gains[8];
    gains[0] = gain; gains[1] = gain; gains[2] = gain; gains[3] = gain;
    gains[4] = gain; gains[5] = gain; gains[6] = gain; gains[7] = gain;
    __asm {
        mov     ecx, count
        mov     eax, src
        mov     edx, dst
        movdqa  xmm7, gains
        align   16
    next:
        movdqu  xmm0, [eax]
        pmulhw  xmm0, xmm7
        pxor    xmm1, xmm1
        pxor    xmm2, xmm2
        punpcklwd xmm1, xmm0
        punpckhwd xmm2, xmm0
        psrad   xmm1, 15
        psrad   xmm2, 15
        paddd   xmm1, [edx]
        paddd   xmm2, [edx + 10h]
        movdqa  [edx], xmm1
        movdqa  [edx + 10h], xmm2
        add     eax, 10h
        add     edx, 20h
        dec     ecx
        jne     next
    }
}
#else
#define ALIGN16 __attribute__((aligned(16)))
typedef int long32;
void ScaleAccumulate16(const short* src, unsigned long count, long32* dst, short gain)
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
    ScaleAccumulate16(src, 3, dst, 16384);                    // gain 0.5 in Q15
    bool ok = dst[24] == 99;
    for (int i = 0; i < 24; ++i) {
        long32 high = ((long32)src[i] * 16384) >> 16;
        long32 expect = 5 + high * 2;
        ok = ok && dst[i] == expect;
    }
    // (the kernel counts down with `dec ; jne`: a zero count is never passed by the mixer)
    std::printf(ok ? "00c6d370_TEST PASS\n" : "00c6d370_TEST FAIL\n");
    return ok ? 0 : 1;
}