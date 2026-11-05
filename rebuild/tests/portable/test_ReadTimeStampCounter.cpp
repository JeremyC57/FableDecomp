// test_ReadTimeStampCounter.cpp -- dual-path test of libpk/ReadTimeStampCounter.h: the function and the macro read the
// same monotonic counter as the portable twin (interleaved reads stay in order).
#include <stdio.h>
#include "libpk/ReadTimeStampCounter.h"
#include "fable_test_mode.h"
#ifdef _MSC_VER
typedef unsigned __int64 tsc_t;
#else
typedef unsigned long long tsc_t;
#endif
static tsc_t ByMacro()
{
    tsc_t counter;
    LIBPK_READ_TIME_STAMP_COUNTER(counter);
    return counter;
}
int main()
{
    int bad = 0;
    for (int i = 0; i < 1000; ++i) {
        tsc_t a = (tsc_t)ReadTimeStampCounter();
        tsc_t b = ByMacro();
#ifdef FABLE_TEST_DUAL
        tsc_t c = (tsc_t)fable_portable::ReadTimeStampCounter();
#else
        tsc_t c = b;
#endif
        tsc_t d = (tsc_t)ReadTimeStampCounter();
        if (!(a <= b && b <= c && c <= d)) ++bad;
    }
    printf("test_ReadTimeStampCounter %s: %s (1000 interleaved reads, %d out of order)\n", kFableTestMode, bad ? "FAIL" : "PASS", bad);
    return bad ? 1 : 0;
}
