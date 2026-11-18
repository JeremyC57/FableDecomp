// Behaviour test for 0x006D63F0 CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete(): the row's defines,
// includes and body verbatim over the shared headers, run on zeroed object images (no constructor runs): the component image holds
// ExperienceMultiplierParam (+0x30) and the PDef pointer (+0x54) to a def image holding ExperienceMultiplierParamPerUnitComboMultiplier
// (+0x64). Cases (exact binary fractions): -1/2+1 = 0.5 -> 0.25 (kills a dropped +1); 5/2+1 = 3.5 -> 0.5/2 = 0.25; 1/4+1 = 1.25 -> 0.25/4 = 0.0625 (division by the def value,
// not multiplication, and the +1 offset); a whole level (4/2+1 = 3) -> 0; negative params whose level truncates toward zero
// (-3/2+1 = -0.5 -> diff -0.5) or below (-5/2+1 = -1.5 -> diff -0.5) clamp to 0; two images with different defs (the receiver's own
// def and param are read).
#include <stdio.h>
#include <string.h>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCHEROEXPERIENCE
#define FABLE_RETAIL_CTCHEROEXPERIENCE_FRACTIONCOMPLETE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroExperience.h"
#include "engine/CHeroExperienceDef.h"

float CTCHeroExperience::GetExperiencePickupMultiplierCurrentLevelFractionComplete() const
{
    float param_per_level = PDef->ExperienceMultiplierParamPerUnitComboMultiplier;
    float multiplier = ExperienceMultiplierParam / param_per_level + 1.0f;
    float diff = multiplier - (long)multiplier;
    if (diff < 0.0f)
        diff = 0.0f;
    diff /= param_per_level;
    return diff;
}

struct Image { unsigned char b[0x60]; };
struct DefImage { unsigned char b[0xB4]; };
static void make(Image& c, DefImage& d, float param, float per_level)
{
    memset(&c, 0, sizeof c); memset(&d, 0, sizeof d);
    memcpy(c.b + 0x30, &param, 4);
    void* pd = &d; memcpy(c.b + 0x54, &pd, 4);
    memcpy(d.b + 0x64, &per_level, 4);
}
static float run(const Image& c) { return reinterpret_cast<const CTCHeroExperience*>(&c)->GetExperiencePickupMultiplierCurrentLevelFractionComplete(); }
static int fail(int n) { printf("006d63f0_TEST FAIL %d\n", n); return 1; }

int main()
{
    if (sizeof(CTCHeroExperience) != 0x60 || sizeof(CHeroExperienceDef) != 0xB4) return fail(0);
    Image a, b; DefImage da, db;
    make(a, da, 5.0f, 2.0f);
    if (run(a) != 0.25f) return fail(1);
    make(b, db, 1.0f, 4.0f);
    if (run(b) != 0.0625f) return fail(2);
    if (run(a) != 0.25f) return fail(3);                       // each receiver reads its own param / def
    make(a, da, 4.0f, 2.0f);
    if (run(a) != 0.0f) return fail(4);                        // whole level
    make(a, da, -3.0f, 2.0f);
    if (run(a) != 0.0f) return fail(5);                        // -0.5: truncates to 0, diff -0.5 clamps
    make(a, da, -5.0f, 2.0f);
    if (run(a) != 0.0f) return fail(6);                        // -1.5: truncates to -1, diff -0.5 clamps
    make(a, da, -1.0f, 2.0f);
    if (run(a) != 0.25f) return fail(8);                       // -0.5 + 1 = 0.5 -> 0.25 (0 without the +1)
    make(a, da, 7.0f, 4.0f);
    if (run(a) != 0.1875f) return fail(7);                     // 2.75 -> 0.75 / 4
    // (audit fix) exact boundary just above the clamp: param 2^-23 / per-level 1 -> multiplier 1 + 2^-23, diff 2^-23 is kept (kills any
    // clamp threshold above 0); a multiplier past the 16-bit range (70000.5) truncates as a long; a NaN param stays NaN: retail's
    // `test ah, 5 ; jp` (0x006D641D/0x006D6420) skips the clamp when the compare is unordered (kills `!(diff >= 0.0f)`).
    unsigned long bits = 0x34000000;                           // 2^-23
    float tiny; memcpy(&tiny, &bits, 4);
    make(a, da, tiny, 1.0f);
    if (run(a) != tiny) return fail(9);
    make(a, da, 69999.5f, 1.0f);
    if (run(a) != 0.5f) return fail(10);
    bits = 0x7FC00000;                                         // quiet NaN
    float nan; memcpy(&nan, &bits, 4);
    make(a, da, nan, 2.0f);
    float r = run(a);
    if (r == r) return fail(11);
    printf("006d63f0_TEST PASS\n");
    return 0;
}
