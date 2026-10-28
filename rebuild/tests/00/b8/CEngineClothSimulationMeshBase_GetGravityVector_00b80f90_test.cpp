// Behaviour test for CEngineClothSimulationMeshBase::GetGravityVector (0x00B80F90): the manager's gravity scaled by the cloth gravity factor, rotated into the mesh's local space.
#include <cstdio>
struct V { float X, Y, Z; };
struct Manager { float GX, GY, GZ, ClothGravityFactor; };
static Manager g_manager;
struct Mesh { float E11, E12, E13, E21, E22, E23, E31, E32, E33;
    V ConvertVectorToLocal(const V& v) const {
        V r; r.X = v.Y * E21 + v.Z * E31 + v.X * E11; r.Y = v.Y * E22 + v.X * E12 + v.Z * E32; r.Z = v.Y * E23 + v.X * E13 + v.Z * E33; return r; }
    V GetGravityVector() const { float f = g_manager.ClothGravityFactor; V g; g.X = g_manager.GX * f; g.Y = g_manager.GY * f; g.Z = g_manager.GZ * f; return ConvertVectorToLocal(g); } };
int main()
{
    g_manager.GX = 0.0f; g_manager.GY = 0.0f; g_manager.GZ = -10.0f; g_manager.ClothGravityFactor = 0.5f;
    Mesh m = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };
    V g = m.GetGravityVector();
    if (g.X != 0.0f || g.Y != 0.0f || g.Z != -5.0f) { std::printf("00b80f90_TEST FAIL\n"); return 1; }
    Mesh r = { 0, 1, 0, -1, 0, 0, 0, 0, 1 };           // 90 degrees about Z; gravity along X
    g_manager.GX = 2.0f; g_manager.GZ = 0.0f; g = r.GetGravityVector();
    if (g.X != 0.0f || g.Y != 1.0f || g.Z != 0.0f) { std::printf("00b80f90_TEST FAIL\n"); return 1; }
    std::printf("00b80f90_TEST PASS\n");
    return 0;
}