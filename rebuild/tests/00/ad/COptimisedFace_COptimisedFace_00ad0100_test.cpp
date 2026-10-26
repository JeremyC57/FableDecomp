// behaviour test for COptimisedFace's copy constructor (0x00AD0100)
#include <cstdio>
#include <new>
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {} };
struct COptimisedFace {
    unsigned int VertexIndices[3]; unsigned int UniqueVertexIndices[3]; int Edges[3];
    C3DVector UTangent, VTangent; bool Exists; unsigned int OutputIndex; int Segment;
};
int main()
{
    COptimisedFace a;
    for (int i = 0; i < 3; ++i) { a.VertexIndices[i] = i + 1; a.UniqueVertexIndices[i] = i + 10; a.Edges[i] = -i; }
    a.UTangent.X = 1; a.UTangent.Y = 2; a.UTangent.Z = 3; a.VTangent.X = 4; a.VTangent.Y = 5; a.VTangent.Z = 6;
    a.Exists = true; a.OutputIndex = 99; a.Segment = -7;
    unsigned char buf[sizeof(COptimisedFace)];
    COptimisedFace* b = new (buf) COptimisedFace(a);
    int bad = 0;
    for (int i = 0; i < 3; ++i) if (b->VertexIndices[i] != a.VertexIndices[i] || b->UniqueVertexIndices[i] != a.UniqueVertexIndices[i] || b->Edges[i] != a.Edges[i]) bad = 1;
    if (b->UTangent.Z != 3 || b->VTangent.X != 4 || !b->Exists || b->OutputIndex != 99 || b->Segment != -7) bad = 1;
    if (bad) { std::printf("00ad0100_TEST FAIL\n"); return 1; }
    std::printf("00ad0100_TEST PASS\n");
    return 0;
}