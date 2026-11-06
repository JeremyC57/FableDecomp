// Behaviour test for C3DPrimitive2::LoadBinary (0x00A92900): a model of the row -- header fields read in order, flags with
// bit 2 cleared, static / animated blocks placed in the caller's memory block (pointer advanced by count * size), vertex buffer
// sized VertexCount * max(reps, 1), index buffer IndexCount * reps * 2 bytes with the dynamic usage / pool when flags bit 0 is
// set, then one cloth primitive per count; a failing cloth load clears the cloth list and the load still succeeds.
#include <stdio.h>
#include <string.h>
struct Stream { const unsigned long* P; unsigned long Get() { return *P++; } };
struct Prim {
    long MaterialIndex, Reps; unsigned long Flags, StaticCount, AnimCount, VertexCount, TriCount, IndexCount;
    unsigned long VBVerts, IBBytes, IBUsage; int IBPool; unsigned long ClothCount; bool Uploaded;
};
static unsigned char* g_Mem;
static bool Load(Prim& p, Stream& s, unsigned long flags, unsigned char** memory, int failCloth)
{
    p.MaterialIndex = (long)s.Get(); p.Reps = (long)s.Get();
    p.StaticCount = s.Get(); p.AnimCount = s.Get(); p.VertexCount = s.Get(); p.TriCount = s.Get(); p.IndexCount = s.Get();
    p.Flags = s.Get() & ~4ul;
    p.StaticCount = s.Get(); p.AnimCount = s.Get();
    *memory += p.StaticCount * 0x20; *memory += p.AnimCount * 0x24;
    long reps = p.Reps; if (reps <= 1) reps = 1;
    p.VBVerts = p.VertexCount * reps;
    p.IBBytes = p.IndexCount * reps * 2;
    p.IBUsage = 8; p.IBPool = 1;
    if (flags & 1) { p.IBUsage = 0x18; p.IBPool = 2; }
    unsigned long cloth = s.Get(); p.ClothCount = cloth;
    for (unsigned long c = 0; c < cloth; ++c) if ((int)c == failCloth) { p.ClothCount = 0; break; }
    p.Uploaded = true;
    return true;
}
int main()
{
    int ok = 1; static unsigned char block[4096];
    const unsigned long data[] = { 7, 3, 9, 9, 100, 50, 150, 0x0F, 2, 1, 4 };
    Prim p; memset(&p, 0, sizeof(p)); Stream s = { data }; unsigned char* m = block;
    ok &= Load(p, s, 1, &m, -1);
    ok &= p.MaterialIndex == 7 && p.Flags == 0x0B && p.StaticCount == 2 && p.AnimCount == 1 && m == block + 2 * 0x20 + 0x24;
    ok &= p.VBVerts == 300 && p.IBBytes == 900 && p.IBUsage == 0x18 && p.IBPool == 2 && p.ClothCount == 4 && p.Uploaded;
    const unsigned long data2[] = { 1, 0, 0, 0, 10, 5, 15, 4, 0, 0, 3 };
    Prim q; memset(&q, 0, sizeof(q)); Stream t = { data2 }; m = block;
    ok &= Load(q, t, 0, &m, 1);
    ok &= q.Flags == 0 && q.VBVerts == 10 && q.IBBytes == 30 && q.IBUsage == 8 && q.IBPool == 1 && q.ClothCount == 0 && m == block;
    printf(ok ? "00a92900_TEST PASS\n" : "00a92900_TEST FAIL\n");
    return ok ? 0 : 1;
}
