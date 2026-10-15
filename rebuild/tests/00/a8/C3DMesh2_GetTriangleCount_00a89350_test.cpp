#include <cstdio>
// Behaviour test for C3DMesh2::GetTriangleCount (retail 0x00A89350): the sum of TriangleCount over every primitive.
struct C3DPrimitive2 {
    unsigned char _pad_0x00[0x58];
    unsigned long VertexCount;
    unsigned long TriangleCount;
    unsigned char _pad_0x60[0x40];
};

struct C3DMesh2 {
    unsigned char  _pad_0x00[0x90];
    unsigned long  PrimitiveCount;
    C3DPrimitive2* Primitives;
    unsigned long GetTriangleCount();
};

unsigned long C3DMesh2::GetTriangleCount()
{
    unsigned long total = 0;
    for (unsigned long i = 0; i < PrimitiveCount; ++i)
        total += Primitives[i].TriangleCount;
    return total;
}

int main()
{
    static C3DPrimitive2 prims[3];
    prims[0].VertexCount = 10; prims[0].TriangleCount = 4;
    prims[1].VertexCount = 20; prims[1].TriangleCount = 7;
    prims[2].VertexCount = 30; prims[2].TriangleCount = 9;
    static C3DMesh2 mesh;
    mesh.Primitives = prims;
    mesh.PrimitiveCount = 0;
    if (mesh.GetTriangleCount() != 0) { std::printf("00a89350_TEST FAIL\n"); return 1; }
    mesh.PrimitiveCount = 3;
    unsigned long want = prims[0].TriangleCount + prims[1].TriangleCount + prims[2].TriangleCount;
    if (mesh.GetTriangleCount() != want) { std::printf("00a89350_TEST FAIL\n"); return 1; }
    mesh.PrimitiveCount = 2;
    if (mesh.GetTriangleCount() != prims[0].TriangleCount + prims[1].TriangleCount) { std::printf("00a89350_TEST FAIL\n"); return 1; }
    std::printf("00a89350_TEST PASS\n");
    return 0;
}