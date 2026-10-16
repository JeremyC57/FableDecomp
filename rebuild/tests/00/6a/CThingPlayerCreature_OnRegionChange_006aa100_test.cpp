#include <cstdio>
// Behaviour model of CThingPlayerCreature::OnRegionChange (0x006AA100).
struct V { float X, Y, Z; void Zero() { X = Y = Z = 0.0f; } };
struct P { V a, r, l; int count; void Run() { a.Zero(); r.Zero(); l.Zero(); if (count != 0) count = 0; } };
int main()
{
    P p; p.a.X = 1; p.a.Y = 2; p.a.Z = 3; p.r = p.a; p.l = p.a; p.count = 4;
    p.Run();
    if (p.a.X != 0 || p.a.Z != 0 || p.r.Y != 0 || p.l.X != 0 || p.count != 0) { std::printf("006aa100_TEST FAIL\n"); return 1; }
    std::printf("006aa100_TEST PASS\n");
    return 0;
}