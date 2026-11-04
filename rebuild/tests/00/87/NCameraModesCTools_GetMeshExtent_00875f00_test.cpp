// Behaviour test for NCameraModes::CTools::GetMeshExtent (0x00875F00): a model of the row -- 0 without a physics mesh, otherwise the
// squared length of the bounding box's X edge plus the squared length of its Y edge.
#include <stdio.h>
struct V { float X, Y, Z; float GetSquaredMagnitude() const { return X * X + Z * Z + Y * Y; } };
struct Mesh { V x, y; V GetBoundingBoxXEdge() const { return x; } V GetBoundingBoxYEdge() const { return y; } };
static float GetMeshExtent(const Mesh* mesh) { if (mesh) return mesh->GetBoundingBoxXEdge().GetSquaredMagnitude() + mesh->GetBoundingBoxYEdge().GetSquaredMagnitude(); return 0.0f; }
int main()
{
    Mesh m = { { 1.0f, 2.0f, 2.0f }, { 0.0f, 3.0f, 4.0f } };
    int ok = GetMeshExtent(&m) == 9.0f + 25.0f && GetMeshExtent(0) == 0.0f;
    printf(ok ? "00875f00_TEST PASS\n" : "00875f00_TEST FAIL\n");
    return ok ? 0 : 1;
}