// Test: GFInitMaths -- the reciprocal-square-root mantissa table (entry 64 forced to 0xFF), 1024 cosines over a full
// turn with entry 1024 repeating entry 0, and 1025 acos / asin samples over [-1, 1] each followed by a zero guard.
#include <cstdio>
#include <cmath>
static unsigned long g_InvSqrtMantissaTable[128];
static float g_CosTable[1026];
static float g_ACosTable[1028];
static float g_ASinTable[1028];
static void GFInitMaths()
{
    unsigned long i;
    union { unsigned long I; float F; } in, out;
    unsigned long* entry = g_InvSqrtMantissaTable;
    for (i = 0; i < 128; ++i) {
        in.I = (i | 0x1F80) << 17;
        out.F = (float)(1.0 / sqrt(in.F));
        *entry++ = ((out.I + 0x2000) >> 15) & 0xFF;
    }
    g_InvSqrtMantissaTable[64] = 0xFF;
    for (i = 0; i < 1024; ++i)
        g_CosTable[i] = (float)cos((float)(i * 2) * 0.0030679617f);
    g_CosTable[1024] = g_CosTable[0];
    for (i = 0; i < 1025; ++i)
        g_ACosTable[i] = (float)acos((float)i * 0.0019512195f - 1.0f);
    g_ACosTable[1025] = 0.0f;
    for (i = 0; i < 1025; ++i)
        g_ASinTable[i] = (float)asin((float)i * 0.0019512195f - 1.0f);
    g_ASinTable[1025] = 0.0f;
}
static bool near(double a, double b) { return fabs(a - b) < 1e-3; }
int main()
{
    g_ACosTable[1025] = 5.0f; g_ASinTable[1025] = 5.0f;
    GFInitMaths();
    bool ok = true;
    // entry 0: sample 1.0 (bits 0x3F000000 = 0.5) -> 1/sqrt(0.5) = 1.41421 bits 0x3FB504F3 -> (+0x2000 >> 15) & 0xFF
    unsigned long e0 = ((0x3FB504F3UL + 0x2000) >> 15) & 0xFF;
    if (g_InvSqrtMantissaTable[0] != e0) ok = false;
    if (g_InvSqrtMantissaTable[64] != 0xFF) ok = false;
    for (int k = 0; k < 128; ++k) if (g_InvSqrtMantissaTable[k] > 0xFF) ok = false;
    if (!near(g_CosTable[0], 1.0) || !near(g_CosTable[256], 0.0) || !near(g_CosTable[512], -1.0)) ok = false;
    if (g_CosTable[1024] != g_CosTable[0]) ok = false;
    if (!near(g_ACosTable[0], 3.14159265) || !near(g_ACosTable[1025], 0.0)) ok = false;
    if (!near(g_ASinTable[0], -1.5707963) || g_ASinTable[1025] != 0.0f) ok = false;
    if (!near(g_ASinTable[512], asin(512 * 0.0019512195 - 1.0))) ok = false;
    printf(ok ? "00a0db60_TEST PASS\n" : "00a0db60_TEST FAIL\n");
    return ok ? 0 : 1;
}