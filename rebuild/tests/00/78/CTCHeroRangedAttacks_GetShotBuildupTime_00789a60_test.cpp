// Behaviour test for 0x00789A60 CTCHeroRangedAttacks::GetShotBuildupTime (links the REAL row object).
// Fixtures (raw native layout): a 0x4C-byte component image, AimingTimer +0x18 (the only field the row reads); the protected static
//   CMainGameComponent::ConstantFPS (retail 0x01375550) defined here.
// Reference model (independent): the quotient of the two integers in double precision (the CRT's default x87 precision control),
//   rounded once to float; the component image is not written.
// Checks: hand-written timer / rate pairs (incl. negative timers, the -1 "not aiming" value, large values) + 20000 seeded pairs,
//   bit patterns compared.
#include <stdio.h>
#include <string.h>

class CMainGameComponent
{
public:
    static void Set(long fps) { ConstantFPS = fps; }
protected:
    static long ConstantFPS;
};
long CMainGameComponent::ConstantFPS = 30;
class CTCHeroRangedAttacks
{
public:
    float GetShotBuildupTime() const;
};

typedef unsigned long UL;
static UL g_seed = 0x789A60u;
static UL Rnd() { g_seed = g_seed * 1664525u + 1013904223u; return g_seed >> 8; }
static int g_fail, g_cases;
static unsigned char g_obj[0x4C], g_ref[0x4C];
static UL Bits(float f) { UL u; memcpy(&u, &f, 4); return u; }

static void Run(long timer, long fps)
{
    ++g_cases;
    for (int i = 0; i < 0x4C; ++i) g_obj[i] = (unsigned char)Rnd();
    memcpy(g_obj + 0x18, &timer, 4);
    memcpy(g_ref, g_obj, sizeof(g_obj));
    CMainGameComponent::Set(fps);
    volatile double num = (double)timer; volatile double den = (double)fps;
    volatile float want = (float)(num / den);
    volatile float got = ((const CTCHeroRangedAttacks*)g_obj)->GetShotBuildupTime();
    if (Bits(got) != Bits(want) || memcmp(g_ref, g_obj, sizeof(g_obj)) != 0)
    {
        if (g_fail < 5) printf("  case %d: %ld / %ld -> %08lX, want %08lX\n", g_cases, timer, fps, Bits(got), Bits(want));
        ++g_fail;
    }
}

int main()
{
    static const long pairs[][2] = {
        { 45, 30 }, { 0, 30 }, { 30, 60 }, { 1, 30 }, { 29, 30 }, { -1, 30 }, { -45, 30 }, { 7, 15 }, { 100, 15 }, { 1, 3 }, { 2, 3 },
        { 0x7FFFFFFF, 30 }, { (long)0x80000000, 30 }, { 16777217, 3 }, { 16777219, 16777217 }, { 1, 0x7FFFFFFF }, { 15, 15 }, { 1001, 1000 }
    };
    for (int i = 0; i < (int)(sizeof(pairs) / sizeof(pairs[0])); ++i) Run(pairs[i][0], pairs[i][1]);
    for (int n = 0; n < 20000; ++n)
    {
        long t = (long)((Rnd() << 8) ^ Rnd()), f = (long)((Rnd() << 8) ^ Rnd());
        if (n & 1) { t = (long)(Rnd() % 4000) - 1; f = (long)(Rnd() % 120) + 1; }
        if (f == 0) f = 30;
        Run(t, f);
    }
    if (g_fail) { printf("00789a60_TEST FAIL (%d of %d cases)\n", g_fail, g_cases); return 1; }
    printf("00789a60_TEST PASS (%d cases)\n", g_cases);
    return 0;
}