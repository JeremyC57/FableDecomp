// behaviour test for 0x007F5730 CThingCompare_Nearest::operator()<CThingPhysical>: null a returns b, null b returns a, else the nearer thing (squared 3D distance; b on a tie)
#include <cstdio>
struct Vec { float X, Y, Z; };
struct Thing { Vec Pos; };
static float LenSq(const Vec& p, const Vec& o)
{
    float x = p.X - o.X, y = p.Y - o.Y, z = p.Z - o.Z;
    return x * x + y * y + z * z;
}
static const Thing* Nearest(const Vec& pos, const Thing* a, const Thing* b)
{
    if (!a) return b;
    if (!b) return a;
    return (LenSq(a->Pos, pos) < LenSq(b->Pos, pos)) ? a : b;
}
int main()
{
    int fails = 0;
    Vec pos = { 1.0f, 2.0f, 3.0f };
    Thing t1 = { { 2.0f, 2.0f, 3.0f } };      // distance 1
    Thing t2 = { { 1.0f, 5.0f, 3.0f } };      // distance 3
    Thing t3 = { { 1.0f, 2.0f, 2.0f } };      // distance 1 (tie with t1)
    Thing t4 = { { -10.0f, 2.0f, 3.0f } };    // distance 11
    if (Nearest(pos, 0, &t1) != &t1) fails |= 1;
    if (Nearest(pos, &t2, 0) != &t2) fails |= 2;
    if (Nearest(pos, 0, 0) != 0) fails |= 4;
    if (Nearest(pos, &t1, &t2) != &t1 || Nearest(pos, &t2, &t1) != &t1) fails |= 8;
    if (Nearest(pos, &t1, &t3) != &t3 || Nearest(pos, &t3, &t1) != &t1) fails |= 16;   // tie: b
    if (Nearest(pos, &t4, &t2) != &t2) fails |= 32;
    const Thing* best = 0; const Thing* all[4] = { &t4, &t2, &t1, &t3 };
    for (int i = 0; i < 4; ++i) best = Nearest(pos, best, all[i]);
    if (best != &t3) fails |= 64;
    if (fails) { std::printf("007f5730_TEST FAIL %d\n", fails); return 1; }
    std::printf("007f5730_TEST PASS\n");
    return 0;
}