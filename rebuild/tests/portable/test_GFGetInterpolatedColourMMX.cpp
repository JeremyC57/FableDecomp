// test_GFGetInterpolatedColourMMX.cpp -- dual-path test of engine/GFGetInterpolatedColourMMX.h (GF_MMX_INTERPOLATE_COLOUR):
// every weight 0..256 (and two out-of-range weights that wrap the 16-bit products) over a set of colour pairs, against the
// per-channel formula and (dual) the twin.
#include <stdio.h>
#include <string.h>
#include "engine/GFGetInterpolatedColourMMX.h"
#include "fable_test_mode.h"
struct Colour { unsigned char B, G, R, A; };
static Colour Blend(Colour a, Colour b, unsigned long weight)
{
    Colour blend;
    unsigned long inverse;
    unsigned long zero;
    zero = 0;
    inverse = 256 - weight;
    GF_MMX_INTERPOLATE_COLOUR(a, b, inverse, weight, zero, blend);
    return blend;
}
int main()
{
    int bad = 0, n = 0;
    static const Colour cs[] = { { 0, 0, 0, 0 }, { 255, 255, 255, 255 }, { 1, 2, 3, 4 }, { 200, 17, 99, 128 }, { 255, 0, 128, 7 } };
    for (int i = 0; i < 5; ++i) for (int j = 0; j < 5; ++j) for (unsigned long w = 0; w <= 258; ++w, ++n) {
        unsigned long weight = w == 257 ? 300ul : (w == 258 ? 0x10010ul : w);
        Colour got = Blend(cs[i], cs[j], weight);
        unsigned int inv = (unsigned int)(256 - weight) & 0xFFFFu, wt = (unsigned int)weight & 0xFFFFu;
        const unsigned char* pa = &cs[i].B; const unsigned char* pb = &cs[j].B; const unsigned char* pg = &got.B;
        for (int c = 0; c < 4; ++c) {
            unsigned int want = ((((pa[c] * inv) & 0xFFFFu) + ((pb[c] * wt) & 0xFFFFu)) & 0xFFFFu) >> 8;
            if (pg[c] != want) { ++bad; break; }
        }
#ifdef FABLE_TEST_DUAL
        Colour q; fable_portable::InterpolateColourMMX(&cs[i], &cs[j], 256 - weight, weight, &q);
        if (memcmp(&q, &got, 4)) ++bad;
#endif
    }
    printf("test_GFGetInterpolatedColourMMX %s: %s (%d blends)\n", kFableTestMode, bad ? "FAIL" : "PASS", n);
    return bad ? 1 : 0;
}
