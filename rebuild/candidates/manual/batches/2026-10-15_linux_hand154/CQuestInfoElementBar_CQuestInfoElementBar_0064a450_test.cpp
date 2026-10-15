// Behaviour test for 0x0064a450 (CQuestInfoElementBar ctor (thing)): the reconstructed logic run over a host model.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) g_fail = 1; } while (0)
struct Colour { unsigned char B, G, R, A; };
struct ThingP { unsigned int uidLo, uidHi; short map; };
struct Bar { float Scale, Value, Max, Min; Colour C, F; long G1, G2; bool Valid; const ThingP* p; unsigned int uidLo, uidHi; long map; };
static void Make(Bar& b, const ThingP* t, const Colour& c, long g, float scale)
{ b.Scale = scale; b.Value = b.Max = b.Min = 0.0f; b.C = c; b.F = c; b.G1 = g; b.G2 = 0; b.Valid = true; b.p = t; b.uidLo = t->uidLo; b.uidHi = t->uidHi; b.map = t->map; }

int main()
{
    Bar b; Colour c = { 9, 8, 7, 6 }; ThingP t = { 0x1234u, 0x5u, -2 };
    Make(b, &t, c, 3, 2.0f);
    CHECK(b.Value == 0.0f && b.Max == 0.0f && b.Min == 0.0f && b.C.B == 9 && b.F.A == 6 && b.G1 == 3 && b.G2 == 0 && b.Valid && b.p == &t && b.uidLo == 0x1234u && b.uidHi == 5u && b.map == -2);
    if (g_fail) { std::printf("0064a450_TEST FAIL\n"); return 1; }
    std::printf("0064a450_TEST PASS\n");
    return 0;
}
