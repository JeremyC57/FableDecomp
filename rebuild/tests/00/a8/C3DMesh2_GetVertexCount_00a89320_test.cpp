#include <cstdio>
// Behaviour test for C3DMesh2::GetVertexCount (retail 0x00A89320): the sum of VertexCount over every primitive.
class C3DPrimitive2 { public:
    unsigned char _pad_0x00[0x58];
    unsigned long VertexCount;
    unsigned long TriangleCount;
    unsigned char _pad_0x60[0x40];
};

class C3DMesh2 { public:
    unsigned char  _pad_0x00[0x90];
    unsigned long  PrimitiveCount;
    C3DPrimitive2* Primitives;
    unsigned long GetVertexCount();
};

unsigned long C3DMesh2::GetVertexCount()
{
    unsigned long total = 0;
    for (unsigned long i = 0; i < PrimitiveCount; ++i)
        total += Primitives[i].VertexCount;
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
    if (mesh.GetVertexCount() != 0) { std::printf("00a89320_TEST FAIL\n"); return 1; }
    mesh.PrimitiveCount = 3;
    unsigned long want = prims[0].VertexCount + prims[1].VertexCount + prims[2].VertexCount;
    if (mesh.GetVertexCount() != want) { std::printf("00a89320_TEST FAIL\n"); return 1; }
    mesh.PrimitiveCount = 2;
    if (mesh.GetVertexCount() != prims[0].VertexCount + prims[1].VertexCount) { std::printf("00a89320_TEST FAIL\n"); return 1; }
    std::printf("00a89320_TEST PASS\n");
    return 0;
}