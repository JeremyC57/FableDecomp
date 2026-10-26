// behaviour test for 0x00AC7E10 COptimisedPrimitive::FindFaceTextureTangents
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct COptimisedVertex { C3DVector Position, Normal, TextureCoord; };
struct COptimisedFace { unsigned int VertexIndices[3]; C3DVector UTangent, VTangent; };
struct COptimisedPrimitive {
    COptimisedVertex* Vertices;
    C3DVector FindTextureTangent(const C3DVector& p0, const C3DVector& p1, const C3DVector& p2, float t0, float t1, float t2)
    { C3DVector r; r.X = p0.X + t0; r.Y = p1.Y + t1; r.Z = p2.Z + t2; return r; }
    void FindFaceTextureTangents(COptimisedFace& face)
    {
        COptimisedVertex& v0 = Vertices[face.VertexIndices[0]];
        COptimisedVertex& v1 = Vertices[face.VertexIndices[1]];
        COptimisedVertex& v2 = Vertices[face.VertexIndices[2]];
        face.UTangent = FindTextureTangent(v0.Position, v1.Position, v2.Position, v0.TextureCoord.X, v1.TextureCoord.X, v2.TextureCoord.X);
        face.VTangent = FindTextureTangent(v0.Position, v1.Position, v2.Position, v0.TextureCoord.Y, v1.TextureCoord.Y, v2.TextureCoord.Y);
    }
};
int main()
{
    COptimisedVertex v[4];
    for (int i = 0; i < 4; ++i) {
        v[i].Position.X = v[i].Position.Y = v[i].Position.Z = (float)(i * 10);
        v[i].TextureCoord.X = (float)i; v[i].TextureCoord.Y = (float)(i + 100); v[i].TextureCoord.Z = 0;
    }
    COptimisedPrimitive p; p.Vertices = v;
    COptimisedFace f = { { 3, 1, 2 } };
    p.FindFaceTextureTangents(f);
    if (f.UTangent.X != 33.0f || f.UTangent.Y != 11.0f || f.UTangent.Z != 22.0f
        || f.VTangent.X != 133.0f || f.VTangent.Y != 111.0f || f.VTangent.Z != 122.0f)
    { std::printf("00ac7e10_TEST FAIL\n"); return 1; }
    std::printf("00ac7e10_TEST PASS\n");
    return 0;
}