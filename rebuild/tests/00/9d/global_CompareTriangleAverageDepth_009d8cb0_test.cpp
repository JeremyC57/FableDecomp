// Behaviour test for CompareTriangleAverageDepth (0x009D8CB0): back-to-front qsort over mean vertex depth.
#include <cstdio>
#include <cstdlib>

struct STriangleVertex
{
    float         x, y;
    float         z;
    unsigned char _pad0C[0x20 - 0x0C];
};

struct STriangleRecord
{
    STriangleVertex v[3];
};

static float AverageDepth(const STriangleRecord* t)
{
    return (t->v[2].z + t->v[1].z + t->v[0].z) * (1.0f / 3.0f);
}

int CompareTriangleAverageDepth(const void* left, const void* right)
{
    const STriangleRecord* a = (const STriangleRecord*)left;
    const STriangleRecord* b = (const STriangleRecord*)right;
    return AverageDepth(a) > AverageDepth(b) ? -1 : 1;
}

static void Set(STriangleRecord& t, float a, float b, float c) { t.v[0].z = a; t.v[1].z = b; t.v[2].z = c; }

int main()
{
    if (sizeof(STriangleRecord) != 0x60) { std::printf("009d8cb0_TEST FAIL\n"); return 1; }
    STriangleRecord t[3];
    Set(t[0], 1.0f, 2.0f, 3.0f);     // mean 2
    Set(t[1], 9.0f, 0.0f, 0.0f);     // mean 3
    Set(t[2], -3.0f, 0.0f, 0.0f);    // mean -1
    if (CompareTriangleAverageDepth(&t[1], &t[0]) != -1) { std::printf("009d8cb0_TEST FAIL\n"); return 1; }
    if (CompareTriangleAverageDepth(&t[0], &t[1]) != 1) { std::printf("009d8cb0_TEST FAIL\n"); return 1; }
    std::qsort(t, 3, sizeof(STriangleRecord), CompareTriangleAverageDepth);
    if (t[0].v[0].z != 9.0f || t[1].v[2].z != 3.0f || t[2].v[0].z != -3.0f) { std::printf("009d8cb0_TEST FAIL\n"); return 1; }
    std::printf("009d8cb0_TEST PASS\n");
    return 0;
}