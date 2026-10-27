#include <cstdio>
// behaviour test of AddInt128Block_00c5d4b0 (0x00c5d4b0): dst[i] += src[i] for 128 packed 32-bit integers (SSE2 paddd, eight blocks of four aligned 16-byte rows)
#ifdef _MSC_VER
#define ALIGN16 __declspec(align(16))
void __stdcall AddInt128Block_00c5d4b0(const long* src, long* dst)
{
    __asm {
        mov     ecx, 8
        mov     eax, src
        mov     edx, dst
        align   16
    next:
        movdqa  xmm0, [eax]
        movdqa  xmm1, [eax + 10h]
        movdqa  xmm2, [eax + 20h]
        movdqa  xmm3, [eax + 30h]
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
}
#else
#define ALIGN16 __attribute__((aligned(16)))
typedef int long32;
void AddInt128Block_00c5d4b0(const long32* src, long32* dst) { for (int i = 0; i < 128; ++i) dst[i] += src[i]; }
#endif
int main()
{
#ifdef _MSC_VER
    ALIGN16 long src[128]; ALIGN16 long dst[129];
#else
    ALIGN16 long32 src[128]; ALIGN16 long32 dst[129];
#endif
    for (int i = 0; i < 128; ++i) { src[i] = i * 3 - 100; dst[i] = 1000 - i; }
    dst[128] = 77;
    AddInt128Block_00c5d4b0(src, dst);
    bool ok = dst[128] == 77;
    for (int i = 0; i < 128; ++i) ok = ok && dst[i] == 1000 - i + i * 3 - 100 && src[i] == i * 3 - 100;
    std::printf(ok ? "00c5d4b0_TEST PASS\n" : "00c5d4b0_TEST FAIL\n");
    return ok ? 0 : 1;
}