// Behaviour test for std::fill<QuadricOptimiserInternals::COptimisedFace*> @ 0x00AD04A0
#include <cstdio>

class C3DVector { public: float X, Y, Z; };
struct COptimisedFace {
    unsigned int VertexIndices[3];
    unsigned int UniqueVertexIndices[3];
    int Edges[3];
    C3DVector UTangent;
    C3DVector VTangent;
    bool Exists;
    unsigned int OutputIndex;
    int Segment;
};

static void fill(COptimisedFace* first, COptimisedFace* last, const COptimisedFace& val)
{
    for ( ; first != last; ++first)
        *first = val;
}

int main()
{
    COptimisedFace faces[5];
    for (int i = 0; i < 5; ++i) { faces[i].Segment = -7; faces[i].Exists = false; faces[i].OutputIndex = 0; }
    COptimisedFace v;
    for (int k = 0; k < 3; ++k) { v.VertexIndices[k] = 10 + k; v.UniqueVertexIndices[k] = 20 + k; v.Edges[k] = -k; }
    v.UTangent.X = 1.0f; v.UTangent.Y = 0.0f; v.UTangent.Z = 0.0f;
    v.VTangent.X = 0.0f; v.VTangent.Y = 1.0f; v.VTangent.Z = 0.0f;
    v.Exists = true; v.OutputIndex = 99; v.Segment = 3;
    fill(faces + 1, faces + 4, v);
    int ok = faces[0].Segment == -7 && faces[4].Segment == -7;
    for (int i = 1; i < 4 && ok; ++i)
        if (!faces[i].Exists || faces[i].OutputIndex != 99 || faces[i].Segment != 3 || faces[i].VertexIndices[2] != 12
            || faces[i].Edges[1] != -1 || faces[i].VTangent.Y < 0.999f) ok = 0;
    fill(faces, faces, v);            // empty range
    if (faces[0].Segment != -7) ok = 0;
    if (!ok) { std::printf("00ad04a0_TEST FAIL\n"); return 1; }
    std::printf("00ad04a0_TEST PASS\n");
    return 0;
}