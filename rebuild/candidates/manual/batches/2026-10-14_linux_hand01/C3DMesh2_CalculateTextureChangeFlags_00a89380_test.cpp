#include <cstdio>
struct C3DMesh2;
static C3DMesh2* g_seen[4]; static int g_calls = 0;
struct C3DPrimitive2 { unsigned char _body[0xa0]; void CalculateTextureChangeFlags(C3DMesh2* m) { g_seen[g_calls++] = m; } };
struct C3DMesh2 { unsigned char _pad_0x00[0x90]; unsigned long PrimitiveCount; C3DPrimitive2* Primitives;
    void CalculateTextureChangeFlags() { for (unsigned long i = 0; i < PrimitiveCount; ++i) Primitives[i].CalculateTextureChangeFlags(this); } };
int main()
{
    C3DPrimitive2 prims[3]; C3DMesh2 m; m.PrimitiveCount = 3; m.Primitives = prims;
    m.CalculateTextureChangeFlags();
    if (g_calls != 3 || g_seen[0] != &m || g_seen[2] != &m) { std::printf("00a89380_TEST FAIL\n"); return 1; }
    m.PrimitiveCount = 0; m.CalculateTextureChangeFlags(); if (g_calls != 3) { std::printf("00a89380_TEST FAIL\n"); return 1; }
    std::printf("00a89380_TEST PASS\n"); return 0;
}
