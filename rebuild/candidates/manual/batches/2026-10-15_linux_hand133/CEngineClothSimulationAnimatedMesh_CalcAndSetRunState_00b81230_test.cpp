// Behaviour test for CEngineClothSimulationAnimatedMesh::CalcAndSetRunState @ 0x00B81230
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct C3DBoundingSphere { C3DVector Center; float Radius; };
struct C3DMeshInfo { C3DBoundingSphere BoundingSphere; };
struct COwner { float Scale; };
struct CMatrix3x4 { float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
struct CMesh {
    long RunState;
    CMatrix3x4 CurrentTransform;
    C3DMeshInfo* MeshInfo;
    COwner* Owner;
    C3DVector m_seenCentre; float m_seenRadius; long m_answer;
    long CalculateRunState(const C3DVector& centre, float radius) { m_seenCentre = centre; m_seenRadius = radius; return m_answer; }
    bool CalcAndSetRunState()
    {
        C3DVector centre = MeshInfo->BoundingSphere.Center;
        C3DVector world;
        float x = centre.Z * CurrentTransform.E31; x += centre.Y * CurrentTransform.E21; x += centre.X * CurrentTransform.E11; x += CurrentTransform.E41;
        world.X = x;
        float y = centre.Z * CurrentTransform.E32; y += centre.Y * CurrentTransform.E22; y += centre.X * CurrentTransform.E12; y += CurrentTransform.E42;
        world.Y = y;
        float z = centre.Z * CurrentTransform.E33; z += centre.Y * CurrentTransform.E23; z += centre.X * CurrentTransform.E13; z += CurrentTransform.E43;
        world.Z = z;
        RunState = CalculateRunState(world, MeshInfo->BoundingSphere.Radius * Owner->Scale);
        return RunState != 1;
    }
};
int main()
{
    CMesh m;
    CMatrix3x4 t = { 1, 0, 0,  0, 1, 0,  0, 0, 1,  10, 20, 30 };   // identity rotation + translation
    m.CurrentTransform = t;
    C3DMeshInfo info; info.BoundingSphere.Center.X = 1; info.BoundingSphere.Center.Y = 2; info.BoundingSphere.Center.Z = 3; info.BoundingSphere.Radius = 4;
    COwner owner; owner.Scale = 0.5f;
    m.MeshInfo = &info; m.Owner = &owner; m.m_answer = 1; m.RunState = 0;
    bool r = m.CalcAndSetRunState();
    if (r || m.RunState != 1 || m.m_seenCentre.X != 11 || m.m_seenCentre.Y != 22 || m.m_seenCentre.Z != 33 || m.m_seenRadius != 2.0f) {
        std::printf("00b81230_TEST FAIL\n"); return 1;
    }
    m.m_answer = 2;
    if (!m.CalcAndSetRunState() || m.RunState != 2) { std::printf("00b81230_TEST FAIL\n"); return 1; }
    std::printf("00b81230_TEST PASS\n");
    return 0;
}
