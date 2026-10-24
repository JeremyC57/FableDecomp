// Behaviour test for 0x00A36350 C3DMeshObject::GetHelperPointPositionAnimated: look the helper up by CRC, animate it, transform by the matrix.
#include <cstdio>
#include <cmath>

struct CCharStringData { const char* m_pChars; unsigned long m_length; };
static const char g_empty[] = "";
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_empty : m_pStringData->m_pChars; }
};
static unsigned long Calc(unsigned long seed, const char* data, unsigned long length)
{
    unsigned long h = seed;
    for (unsigned long i = 0; i < length; ++i) h = h * 31 + (unsigned char)data[i];
    return h;
}
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& p) : X(p.X), Y(p.Y), Z(p.Z) { }
};
struct C3DHelperPoint { C3DVector Position; int BoneIndex; };
struct C3DAnimationBlendState { float Offset; };
struct C3DAnimationManager { int Calls; };

static unsigned long g_hash = 0, g_index = 0;
struct C3DMeshStats {
    C3DHelperPoint m_points[2];
    virtual ~C3DMeshStats() { }
    virtual C3DVector GetHelperPointPositionAnimated(const C3DHelperPoint& helper, const C3DAnimationBlendState& state,
                                                     const C3DAnimationBlendState& other, float blend,
                                                     C3DAnimationManager& manager) const
    {
        ++manager.Calls;
        float d = state.Offset + other.Offset * blend;
        return C3DVector(helper.Position.X + d, helper.Position.Y + d, helper.Position.Z + d);
    }
    const C3DHelperPoint* GetHelperPoint(unsigned long nameHash, unsigned long index) const
    {
        g_hash = nameHash; g_index = index;
        return &m_points[index];
    }
};
struct CCountedPointer {
    const C3DMeshStats* Data; void* Info;
    const C3DMeshStats* operator->() const { return Data; }
};
struct C3DMeshObject {
    void* MeshInfo;
    CCountedPointer Mesh;
    float PositionMatrix[12];
    C3DVector GetHelperPointPositionAnimated(const CCharString& name, long index, const C3DAnimationBlendState& state,
                                             const C3DAnimationBlendState& other, float blend,
                                             C3DAnimationManager& manager) const
    {
        const C3DHelperPoint& helper = *Mesh->GetHelperPoint(Calc(0, name.GetChars(), name.GetLength()), index);
        C3DVector point = Mesh->GetHelperPointPositionAnimated(helper, state, other, blend, manager);
        const float x = point.X;
        const float y = point.Y;
        point.X = x*PositionMatrix[0] + y*PositionMatrix[3] + point.Z*PositionMatrix[6] + PositionMatrix[9];
        point.Y = x*PositionMatrix[1] + y*PositionMatrix[4] + point.Z*PositionMatrix[7] + PositionMatrix[10];
        point.Z = x*PositionMatrix[2] + y*PositionMatrix[5] + point.Z*PositionMatrix[8] + PositionMatrix[11];
        return point;
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    C3DMeshStats stats;
    stats.m_points[0].Position = C3DVector(9, 9, 9);
    stats.m_points[1].Position = C3DVector(1, 2, 3);
    C3DMeshObject obj;
    obj.MeshInfo = 0; obj.Mesh.Data = &stats; obj.Mesh.Info = 0;
    // rotation: x' = -y, y' = x, z' = z ; translation (10, 20, 30)
    float m[12] = { 0, 1, 0,  -1, 0, 0,  0, 0, 1,  10, 20, 30 };
    for (int i = 0; i < 12; ++i) obj.PositionMatrix[i] = m[i];
    CCharStringData d = { "HP_TIP", 6 };
    CCharString name = { &d };
    C3DAnimationBlendState a, b; a.Offset = 1.0f; b.Offset = 2.0f;
    C3DAnimationManager mgr; mgr.Calls = 0;
    C3DVector r = obj.GetHelperPointPositionAnimated(name, 1, a, b, 0.5f, mgr);
    // animated point = (3, 4, 5)
    if (!Near(r.X, 10 - 4) || !Near(r.Y, 20 + 3) || !Near(r.Z, 30 + 5) || mgr.Calls != 1 || g_index != 1 ||
        g_hash != Calc(0, "HP_TIP", 6)) {
        std::printf("00a36350_TEST FAIL\n");
        return 1;
    }
    std::printf("00a36350_TEST PASS\n");
    return 0;
}