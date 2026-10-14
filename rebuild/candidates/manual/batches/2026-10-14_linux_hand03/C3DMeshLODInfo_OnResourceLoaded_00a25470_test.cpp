#include <cstdio>

struct Owner { long removed; Owner() : removed(-1) {} void RemoveLOD(unsigned long lod, bool) { removed = (long)lod; } };
struct Lod { void* file; Owner* owner; unsigned char lod; bool buildOk;
    bool BuildMesh(bool, bool) { return buildOk; }
    void OnResourceLoaded() { if (file) { if (!BuildMesh(true, false)) owner->RemoveLOD(lod, false); } } };
int main()
{
    Owner o; Lod a = { &o, &o, 2, false }; a.OnResourceLoaded(); if (o.removed != 2) { std::printf("00a25470_TEST FAIL\n"); return 1; }
    o.removed = -1; Lod b = { &o, &o, 2, true }; b.OnResourceLoaded(); Lod c = { 0, &o, 3, false }; c.OnResourceLoaded(); if (o.removed != -1) { std::printf("00a25470_TEST FAIL\n"); return 1; }
    std::printf("00a25470_TEST PASS\n");
    return 0;
}
