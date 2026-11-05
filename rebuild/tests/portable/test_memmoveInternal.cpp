// test_memmoveInternal.cpp -- dual-path test of strmbase/memmoveInternal.h: both copy orders on overlapping and disjoint
// ranges, every count 0..40 and offsets 0..7, against memmove-free references and (dual) the twins.
#include <stdio.h>
#include <string.h>
#include "strmbase/memmoveInternal.h"
#include "fable_test_mode.h"
static void Forward(void* dst, const void* src, unsigned int count) {
    STRMBASE_MEMMOVE_FORWARD(dst, src, count);
}
static void Backward(void* dst, const void* src, unsigned int count) {
    STRMBASE_MEMMOVE_BACKWARD(dst, src, count);
}
static void Fill(unsigned char* b) { for (int i = 0; i < 96; ++i) b[i] = (unsigned char)(i * 37 + 11); }
int main()
{
    int bad = 0;
    unsigned char a[96], r[96];
    for (unsigned int count = 0; count <= 40; ++count)
        for (int d = 0; d < 8; ++d)
            for (int s = 0; s < 8; ++s) {
                // forward: dst below src (or disjoint) -- a lowest-first byte copy is the reference
                Fill(a); Fill(r);
                Forward(a + d, a + 16 + s, count);
                for (unsigned int i = 0; i < count; ++i) r[d + i] = r[16 + s + i];
                if (memcmp(a, r, 96)) ++bad;
                // backward: dst above src -- a highest-first byte copy is the reference
                Fill(a); Fill(r);
                Backward(a + 16 + d, a + s, count);
                for (unsigned int i = count; i; --i) r[16 + d + i - 1] = r[s + i - 1];
                if (memcmp(a, r, 96)) ++bad;
#ifdef FABLE_TEST_DUAL
                unsigned char t[96];
                Fill(a); Fill(t); Forward(a + d, a + 8 + s, count); fable_portable::MemmoveForward(t + d, t + 8 + s, count);
                if (memcmp(a, t, 96)) ++bad;
                Fill(a); Fill(t); Backward(a + 8 + d, a + s, count); fable_portable::MemmoveBackward(t + 8 + d, t + s, count);
                if (memcmp(a, t, 96)) ++bad;
#endif
            }
    printf("test_memmoveInternal %s: %s\n", kFableTestMode, bad ? "FAIL" : "PASS");
    return bad ? 1 : 0;
}
