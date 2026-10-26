// behaviour test for 0x00B835D0 std::vector<CCountedPointer<CParticleSimulation> >::erase(position): the tail shifts down one slot (count
// blocks shared, the overwritten ones released), the vacated last slot is released and cleared, size drops by one, position returned
#include <cstdio>
struct Info { int RefCount; int Deleted; };
struct CP { void* Data; Info* PInfo; };
struct Vec { CP* start; CP* finish; };
static void Release(CP& p) { if (p.PInfo && --p.PInfo->RefCount == 0) p.PInfo->Deleted = 1; }
static void Assign(CP& to, const CP& from)
{
    if (to.PInfo != from.PInfo) { Release(to); to.Data = from.Data; to.PInfo = from.PInfo; if (to.PInfo) ++to.PInfo->RefCount; }
}
static CP* erase(Vec& v, CP* pos)                           // the function under test
{
    if (pos + 1 != v.finish)
        for (CP* s = pos + 1, *d = pos; s != v.finish; ++s, ++d) Assign(*d, *s);
    --v.finish;
    Release(*v.finish); v.finish->Data = 0; v.finish->PInfo = 0;
    return pos;
}
int main()
{
    int fail = 0;
    int o[3];
    Info i1 = { 1, 0 }, i2 = { 1, 0 }, i3 = { 1, 0 };
    CP a[3] = { { &o[0], &i1 }, { &o[1], &i2 }, { &o[2], &i3 } };
    Vec v; v.start = a; v.finish = a + 3;
    if (erase(v, a) != a) fail |= 1;                        // erase the first
    if (v.finish != a + 2 || a[0].Data != &o[1] || a[1].Data != &o[2] || a[2].PInfo != 0 || a[2].Data != 0) fail |= 2;
    if (!i1.Deleted || i2.RefCount != 1 || i3.RefCount != 1) fail |= 4;
    if (erase(v, a + 1) != a + 1 || v.finish != a + 1 || !i3.Deleted || a[1].PInfo != 0) fail |= 8;   // erase the last
    if (fail) { std::printf("00b835d0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00b835d0_TEST PASS\n");
    return 0;
}