// Behaviour test for 0x00A22530 GFFastArcCos (links the REAL row object).
// The row reads one float and two neighbouring entries of GFastArcCosTable and returns the interpolated value times 1 / 2pi. The
// model follows retail's x87 sequence (0x00A22533..0x00A2257C) in double arithmetic, which is what the FPU does under the CRT's
// 53-bit precision: p = (value + 1) * 512.5 stays in the register; the index is the nearest integer (ties to even, the bare
// fistp) of (p - 0.5) AFTER that difference has been stored as a float (0x00A2254D); interp is the float-stored p minus the index
// (0x00A2255E reads the slot written by `fst` at 0x00A22543); the two products and the sum stay in the register. The value is
// compared bit for bit after the caller's float store. The table is filled with seeded random floats (not arc cosines), so a wrong
// index, a wrong neighbour or a swapped weight cannot hide behind a smooth curve; a second pass uses the engine's arc-cosine values.
#include <stdio.h>
#include <string.h>
#include <math.h>

float GFastArcCosTable[1026];                      // 0x013CB530, bound by the row
float __fastcall GFFastArcCos(float value);        // the row

static unsigned long g_seed = 0x00A22530;
static unsigned long Rnd() { g_seed = g_seed * 1664525UL + 1013904223UL; return g_seed >> 8; }
static float RndUnit() { return (float)(Rnd() & 0xFFFFFF) / 16777216.0f; }                 // [0, 1)
static unsigned long Bits(float f) { unsigned long u; memcpy(&u, &f, 4); return u; }

static long NearestEven(double v)
{
    double f = floor(v); double d = v - f; long r = (long)f;
    if (d > 0.5 || (d == 0.5 && (r & 1))) ++r;
    return r;
}

static float Model(float value, long* used_index)
{
    double p = ((double)value + 1.0) * 512.5;
    float stored_pos = (float)p;
    float argument = (float)(p - 0.5);
    long index = NearestEven((double)argument);
    double interp = (double)stored_pos - (double)index;
    double sum = (1.0 - interp) * (double)GFastArcCosTable[index] + interp * (double)GFastArcCosTable[index + 1];
    double scaled = sum * (double)0.15915494f;
    *used_index = index;
    return (float)scaled;
}

static int g_fail = 0;
static int g_cases = 0;
static int g_index_hit[1025];

static void One(float value)
{
    long index = 0;
    float want = Model(value, &index);
    float got = GFFastArcCos(value);
    ++g_cases;
    if (index >= 0 && index < 1025) g_index_hit[index] = 1;
    if (Bits(got) != Bits(want))
    {
        if (g_fail < 10) printf("FAIL value %.9g (bits %08lX): got %08lX want %08lX index %ld\n", (double)value, Bits(value), Bits(got), Bits(want), index);
        ++g_fail;
    }
}

static void Sweep()
{
    int k;
    One(-1.0f); One(1.0f); One(0.0f); One(0.5f); One(-0.5f); One(0.999999f); One(-0.999999f); One(0.95f);
    for (k = 0; k <= 1024; ++k)
    {
        // values whose scaled position sits on, just under and just over each table step and each half step (the rounding ties)
        float centre = (float)((k + 0.5) / 512.5 - 1.0);
        float edge = (float)(k / 512.5 - 1.0);
        One(edge); One(centre);
        One((float)(edge + 1e-6)); One((float)(edge - 1e-6)); One((float)(centre + 1e-6)); One((float)(centre - 1e-6));
    }
    for (k = 0; k < 20000; ++k) One(RndUnit() * 2.0f - 1.0f);
}

int main()
{
    int k, hit = 0;
    for (k = 0; k < 1026; ++k) GFastArcCosTable[k] = RndUnit() * 8.0f - 4.0f;
    Sweep();
    for (k = 0; k < 1026; ++k) GFastArcCosTable[k] = (float)acos(k * 2.0 / 1025.0 - 1.0);
    Sweep();
    for (k = 0; k < 1025; ++k) hit += g_index_hit[k];
    if (hit < 1024) { printf("FAIL coverage: %d table steps reached\n", hit); ++g_fail; }
    {
        // the engine's use: a quarter turn at 0, nothing at 1, half a turn at -1 (the table's resolution)
        double q = (double)GFFastArcCos(0.0f), z = (double)GFFastArcCos(1.0f), h = (double)GFFastArcCos(-1.0f);
        if (fabs(q - 0.25) > 1e-3 || fabs(z) > 8e-3 || fabs(h - 0.5) > 8e-3) { printf("FAIL turns %.6f %.6f %.6f\n", q, z, h); ++g_fail; }
    }
    if (g_fail) { printf("00a22530 FAIL (%d of %d)\n", g_fail, g_cases); return 1; }
    printf("00a22530_TEST PASS (%d cases, %d table steps)\n", g_cases, hit);
    return 0;
}