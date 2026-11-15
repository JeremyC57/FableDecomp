// test_MixKernelsSIMD_copy64.cpp -- dual-path test of LIBPK_MMX_COPY64 (libpk/MixKernelsSIMD.h, agentC42): 64 bytes from source to
// history, nothing else written, against memcpy and (dual) the twin.
#include <stdio.h>
#include <string.h>
#include "libpk/MixKernelsSIMD.h"
#include "fable_test_mode.h"
static void Kernel(unsigned char* buffer)
{
    unsigned char* source = buffer + 0x100;
    unsigned char* history = buffer;
    LIBPK_MMX_COPY64(source, history);
}
int main()
{
    int bad = 0;
    static unsigned char a[0x180], r[0x180];
    for (int i = 0; i < 0x180; ++i) a[i] = r[i] = (unsigned char)(i * 7 + 3);
    memcpy(r, r + 0x100, 0x40);
    Kernel(a);
    if (memcmp(a, r, sizeof a)) { printf("  copy differs\n"); ++bad; }
#ifdef FABLE_TEST_DUAL
    static unsigned char q[0x180];
    for (int i = 0; i < 0x180; ++i) q[i] = (unsigned char)(i * 7 + 3);
    fable_portable::CopyBytes(q, q + 0x100, 0x40);
    if (memcmp(a, q, sizeof a)) { printf("  twin differs\n"); ++bad; }
#endif
    printf("test_MixKernelsSIMD_copy64 %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
