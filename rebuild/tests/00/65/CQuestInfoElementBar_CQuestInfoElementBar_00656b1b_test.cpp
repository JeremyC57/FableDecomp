// Behaviour test for 0x00656b1b (CQuestInfoElementBar ctor (no thing)): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Colour { unsigned char B, G, R, A; };
struct ThingP { unsigned int uidLo, uidHi; short map; };
struct Bar { float Scale, Value, Max, Min; Colour C, F; long G1, G2; bool Valid; const ThingP* p; unsigned int uidLo, uidHi; long map; };
static void Make(Bar& b, float maxV, float minV, const Colour& c, const Colour& f, long g1, long g2, float scale)
{ b.Scale = scale; b.Value = b.Max = maxV; b.Min = minV; b.C = c; b.F = f; b.G1 = g1; b.G2 = g2; b.Valid = false; b.p = 0; b.uidLo = b.uidHi = 0; b.map = 0; }

int main()
{
    Bar b; Colour c = { 1, 2, 3, 4 }; Colour f = { 5, 6, 7, 8 };
    Make(b, 10.0f, 1.0f, c, f, 11, 12, 0.5f);
    CHECK(b.Value == 10.0f && b.Max == 10.0f && b.Min == 1.0f && b.C.R == 3 && b.F.A == 8 && b.G1 == 11 && b.G2 == 12 && !b.Valid && !b.p && b.map == 0 && b.Scale == 0.5f);
    if (g_fail) { std::printf("00656b1b_TEST FAIL\n"); return 1; }
    std::printf("00656b1b_TEST PASS\n");
    return 0;
}