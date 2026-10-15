// Behaviour test for CEngineClothSimulationMeshBase::GetWindVector @ 0x00B80FE0
#include <cstdio>
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    void ScaleInto(C3DVector& out, float s) const { out.X = X * s; out.Y = Y * s; out.Z = Z * s; }
};
class CMatrix3x4 { public: C3DVector Row1, Row2, Row3, Position; };
class CSimulationEnvironment {
public:
    C3DVector m_wind; C3DVector m_lastQuery;
    C3DVector GetWindAt(const C3DVector& position) { m_lastQuery = position; return m_wind; }
};
CSimulationEnvironment g_environment;
CSimulationEnvironment* g_pSimulationEnvironment = &g_environment;
class CEngineClothSimulationMeshBase {
public:
    CMatrix3x4 CurrentTransform, LastTransform, InverseTransform;
    C3DVector ConvertVectorToLocal(const C3DVector& v) const
    {
        float x = v.Y * InverseTransform.Row2.X; x += v.Z * InverseTransform.Row3.X; x += v.X * InverseTransform.Row1.X;
        float y = v.Y * InverseTransform.Row2.Y; y += v.X * InverseTransform.Row1.Y; y += v.Z * InverseTransform.Row3.Y;
        float z = v.Y * InverseTransform.Row2.Z; z += v.X * InverseTransform.Row1.Z; z += v.Z * InverseTransform.Row3.Z;
        C3DVector local; local.X = x; local.Y = y; local.Z = z;
        return local;
    }
    C3DVector GetWindVector() const
    {
        C3DVector v(CurrentTransform.Position);
        g_pSimulationEnvironment->GetWindAt(v).ScaleInto(v, 100.0f);
        return ConvertVectorToLocal(v);
    }
};
static void Set(C3DVector& v, float x, float y, float z) { v.X = x; v.Y = y; v.Z = z; }
int main()
{
    CEngineClothSimulationMeshBase m;
    Set(m.CurrentTransform.Position, 7, 8, 9);
    Set(m.InverseTransform.Row1, 1, 0, 0);
    Set(m.InverseTransform.Row2, 0, 0, 1);   // swaps Y and Z
    Set(m.InverseTransform.Row3, 0, 1, 0);
    Set(g_environment.m_wind, 0.5f, 0.25f, 2.0f);
    C3DVector r = m.GetWindVector();
    if (g_environment.m_lastQuery.X != 7 || g_environment.m_lastQuery.Z != 9) { std::printf("00b80fe0_TEST FAIL\n"); return 1; }
    if (r.X != 50.0f || r.Y != 200.0f || r.Z != 25.0f) { std::printf("00b80fe0_TEST FAIL\n"); return 1; }
    std::printf("00b80fe0_TEST PASS\n");
    return 0;
}