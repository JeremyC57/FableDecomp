// Behaviour test for N3DPrimitive::CAnimatedBlock::ConvertToVBFormatBump (0x00A90040): a standalone copy of the model and logic.
// Checks: one output vertex per VertexList entry taken from the mesh vertex it indexes (position, normal, uv), bone indices x3 as
// bytes and weights as bytes, zero padding to four, the U tangent copied, the handedness sign (+1 when Cross(N,U).U >= 0, else -1 -- both branches; retail dots against the U tangent, so VTangents values that would flip the sign must not change it), and
// that nothing beyond the block's vertex count is written.
#include <cstdio>
#include <cstring>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector Cross(const C3DVector& other) const { return C3DVector((Y*other.Z-Z*other.Y),(Z*other.X-X*other.Z),(X*other.Y-Y*other.X)); }
    const float Dot(const C3DVector& other) const { return X*other.X+(Y*other.Y+Z*other.Z); }
};
class C3DVertex2 { public:
    C3DVector Position, Normal; float U, V;
    const C3DVector GetPosition() const { return Position; }
    const C3DVector GetNormal() const { return Normal; }
    float GetU() const { return U; }
    float GetV() const { return V; }
};
class C3DIndexedBoneWeight { public: unsigned long BoneIndex, Weight; };
template<class T> class CArray { public:
    T* m_begin; T* m_end; T* m_capacity;
    unsigned long size() const { return (unsigned long)(m_end - m_begin); }
    const T& operator[](unsigned long i) const { return m_begin[i]; }
    void Set(T* p, unsigned long n) { m_begin = p; m_end = p + n; m_capacity = p + n; }
};
class CTVertexNormBlend4Tex1BumpBase { public:
    float X, Y, Z; unsigned char BlendingIndices[4]; unsigned char BlendingWeights[4];
    float NormalX, NormalY, NormalZ, U1, V1; float UTangent[3]; float VTangentSign;
};
template<class Base> struct CGenericVertex : public Base {
    void SetPosition(const C3DVector& p) { this->X = p.X; this->Y = p.Y; this->Z = p.Z; }
    void SetNormal(const C3DVector& p) { this->NormalX = p.X; this->NormalY = p.Y; this->NormalZ = p.Z; }
    void SetU1(float u) { this->U1 = u; }
    void SetV1(float v) { this->V1 = v; }
};
class CTVertexNormBlend4Tex1Bump : public CGenericVertex<CTVertexNormBlend4Tex1BumpBase> { public:};
struct CMeshData {
    void* vfptr; CArray<C3DVertex2> Vertices; unsigned char TriangleLists[12], Groups[12], BoundingSphere[16];
    CArray<C3DVector> UTangents; CArray<C3DVector> VTangents;
};
struct CBlockData { void* vfptr; unsigned long IVRefCount; CArray<unsigned long> VertexList; CArray<C3DIndexedBoneWeight> BoneWeights; };
struct CAnimatedBlock {
    unsigned long PrimitiveCount, StartIndex; bool IsStrip, DegenerateTriangles; unsigned char ChangeFlags;
    unsigned long BoneConstantsFixupOffset, VertexCount; unsigned short BonesPerVertex; bool PalettedFlag; unsigned char GroupCount;
    unsigned char Groups[18]; CBlockData* Data;
    void ConvertToVBFormatBump(CTVertexNormBlend4Tex1Bump* out, CMeshData& mesh)
    {
        unsigned long count = Data->VertexList.size();
        for (unsigned long i = 0; i < count; ++i) {
            unsigned long index = Data->VertexList[i];
            const C3DVertex2& src = mesh.Vertices[index];
            CTVertexNormBlend4Tex1Bump& vertex = out[i];
            vertex.SetPosition(src.GetPosition());
            vertex.SetNormal(src.GetNormal());
            vertex.SetU1(src.GetU());
            vertex.SetV1(src.GetV());
            unsigned long j;
            for (j = 0; j < BonesPerVertex; ++j) {
                const C3DIndexedBoneWeight& weight = Data->BoneWeights[i * BonesPerVertex + j];
                vertex.BlendingIndices[j] = (unsigned char)(weight.BoneIndex * 3);
                vertex.BlendingWeights[j] = (unsigned char)weight.Weight;
            }
            for (; j < 4; ++j) {
                vertex.BlendingIndices[j] = 0;
                vertex.BlendingWeights[j] = 0;
            }
            const C3DVector& u_tangent = mesh.UTangents[index];
            const C3DVector& v_tangent = mesh.UTangents[index];
            vertex.UTangent[0] = u_tangent.X;
            vertex.UTangent[1] = u_tangent.Y;
            vertex.UTangent[2] = u_tangent.Z;
            C3DVector crossprod = src.GetNormal().Cross(u_tangent);
            if (crossprod.Dot(v_tangent) >= 0.0f) vertex.VTangentSign = 1.0f;
            else vertex.VTangentSign = -1.0f;
        }
    }
};
static int Fail(const char* what) { std::printf("failed check: %s\n", what); std::printf("00a90040_TEST FAIL\n"); return 1; }
int main()
{
    if (sizeof(CTVertexNormBlend4Tex1Bump) != 0x38 || sizeof(C3DVertex2) != 0x20 || sizeof(CAnimatedBlock) != 0x30) return Fail("layout");
    C3DVertex2 verts[4]; C3DVector utan[4]; C3DVector vtan[4];
    for (int k = 0; k < 4; ++k) {
        verts[k].Position = C3DVector(1.0f + k, 10.0f + k, 100.0f + k);
        verts[k].Normal = C3DVector(1.0f + k, 2.0f, 3.0f);
        verts[k].U = 0.25f * k; verts[k].V = 0.5f * k;
        utan[k] = C3DVector(4.0f, 5.0f + k, 6.0f);
    }
    // vertex 0: Cross(N,U).U is mathematically 0, but the float-rounded cross product of these values dots with U to about
    // -1.4e-7, so the function must take the VTangentSign = -1 branch for it (vertices 1..3 give exactly 0 -> +1).
    verts[0].Normal = C3DVector(0.2f, 0.9f, 0.3f); utan[0] = C3DVector(1.3f, 0.1f, 1.3f);
    // VTangents is never read by the function: choose it so that reading it would flip every sign (-Cross for the +1 vertices,
    // +Cross for the -1 vertex), so a model that dotted against VTangents fails the sign check.
    for (int k = 0; k < 4; ++k) {
        C3DVector c = verts[k].Normal.Cross(utan[k]);
        vtan[k] = (k == 0) ? c : C3DVector(-c.X, -c.Y, -c.Z);
    }
    const float expected_sign[4] = { -1.0f, 1.0f, 1.0f, 1.0f };
    CMeshData mesh; std::memset(&mesh, 0, sizeof(mesh));
    mesh.Vertices.Set(verts, 4); mesh.UTangents.Set(utan, 4); mesh.VTangents.Set(vtan, 4);
    unsigned long list[3] = { 2, 0, 3 };
    C3DIndexedBoneWeight weights[6] = { {1, 200}, {2, 55}, {7, 255}, {0, 0}, {90, 128}, {100, 127} };
    CBlockData data; std::memset(&data, 0, sizeof(data));
    data.VertexList.Set(list, 3); data.BoneWeights.Set(weights, 6);
    CAnimatedBlock block; std::memset(&block, 0, sizeof(block));
    block.BonesPerVertex = 2; block.Data = &data;
    CTVertexNormBlend4Tex1Bump out[4]; std::memset(out, 0xCC, sizeof(out));
    block.ConvertToVBFormatBump(out, mesh);
    for (int i = 0; i < 3; ++i) {
        const C3DVertex2& s = verts[list[i]]; const CTVertexNormBlend4Tex1Bump& d = out[i];
        if (d.X != s.Position.X || d.Y != s.Position.Y || d.Z != s.Position.Z) return Fail("position");
        if (d.NormalX != s.Normal.X || d.NormalY != s.Normal.Y || d.NormalZ != s.Normal.Z) return Fail("normal");
        if (d.U1 != s.U || d.V1 != s.V) return Fail("uv");
        for (int j = 0; j < 2; ++j) {
            if (d.BlendingIndices[j] != (unsigned char)(weights[i * 2 + j].BoneIndex * 3)) return Fail("bone index");
            if (d.BlendingWeights[j] != (unsigned char)weights[i * 2 + j].Weight) return Fail("bone weight");
        }
        for (int j = 2; j < 4; ++j) if (d.BlendingIndices[j] != 0 || d.BlendingWeights[j] != 0) return Fail("padding");
        if (d.UTangent[0] != utan[list[i]].X || d.UTangent[1] != utan[list[i]].Y || d.UTangent[2] != utan[list[i]].Z) return Fail("tangent");
        if (d.VTangentSign != expected_sign[list[i]]) return Fail("sign");
    }
    if (out[2].BlendingIndices[0] != (unsigned char)(90 * 3) || out[2].BlendingIndices[1] != (unsigned char)(100 * 3)) return Fail("index wrap");
    const unsigned char* tail = (const unsigned char*)&out[3];
    for (unsigned k = 0; k < sizeof(out[3]); ++k) if (tail[k] != 0xCC) return Fail("overrun");
    // four bones per vertex: no padding; zero bones: all padding
    block.BonesPerVertex = 4; data.VertexList.Set(list, 1);
    C3DIndexedBoneWeight w4[4] = { {1, 1}, {2, 2}, {3, 3}, {4, 4} }; data.BoneWeights.Set(w4, 4);
    block.ConvertToVBFormatBump(out, mesh);
    for (int j = 0; j < 4; ++j) if (out[0].BlendingIndices[j] != 3 * (j + 1) || out[0].BlendingWeights[j] != j + 1) return Fail("four bones");
    block.BonesPerVertex = 0;
    block.ConvertToVBFormatBump(out, mesh);
    for (int j = 0; j < 4; ++j) if (out[0].BlendingIndices[j] != 0 || out[0].BlendingWeights[j] != 0) return Fail("zero bones");
    // empty block: nothing written
    data.VertexList.Set(list, 0); std::memset(out, 0xCC, sizeof(out));
    block.ConvertToVBFormatBump(out, mesh);
    if (((const unsigned char*)out)[0] != 0xCC) return Fail("empty");
    std::printf("00a90040_TEST PASS\n");
    return 0;
}