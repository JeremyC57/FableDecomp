// test_IsCpuidSupported.cpp -- dual-path test of libpk/IsCpuidSupported.h: the EFLAGS.ID probe and the twin agree (every
// x86 / x86-64 test host implements CPUID), and a set result stays set.
#include <stdio.h>
#include "libpk/IsCpuidSupported.h"
#include "fable_test_mode.h"
static long Probe(long start) { long supported = start;
    LIBPK_EFLAGS_ID_PROBE(supported);
return supported; }
int main()
{
    int bad = 0;
    long got = Probe(0);
    if (Probe(1) != 1) ++bad;
#if defined(_M_IX86) || defined(__i386__) || defined(__x86_64__) || defined(_M_X64)
    if (got != 1) ++bad;
#endif
#ifdef FABLE_TEST_DUAL
    long twin = 0;
    fable_portable::EflagsIdProbe(twin);
    if (twin != got) { printf("  asm %ld, twin %ld\n", got, twin); ++bad; }
#endif
    printf("test_IsCpuidSupported %s: %s (%ld)\n", kFableTestMode, bad ? "FAIL" : "PASS", got);
    return bad ? 1 : 0;
}
