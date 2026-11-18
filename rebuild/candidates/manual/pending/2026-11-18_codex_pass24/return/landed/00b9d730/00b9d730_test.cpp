#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#include "engine/CMatrix3x4.h"
#include <cassert>
#include <cstdio>

struct BoundingSphereFixture
{
    C3DVector Center;
    float Radius;
    const C3DVector GetCenter() const { return Center; }
    float GetRadius() const { return Radius; }
};

class C3DPrimitive2
{
public:
    BoundingSphereFixture BoundingSphere;
    BoundingSphereFixture GetBoundingSphere() const { return BoundingSphere; }
};

class CEngineInternalPrimitiveStaticMesh
{
public:
    CMatrix3x4 ObjectTransform;
    float Scale;
};

struct CameraFixture
{
    C3DVector position;
    bool called;
    C3DVector captured_center;
    float captured_radius;

    bool ClipSphere(const C3DVector& center, float radius)
    {
        called = true;
        captured_center = center;
        captured_radius = radius;

        const float x = center.X - position.X;
        const float y = center.Y - position.Y;
        const float z = center.Z - position.Z;
        return z + radius >= 2.0f && z - radius <= 6.0f &&
               x <= radius && -x <= radius && y <= radius && -y <= radius;
    }
};

namespace NEngineInternal
{
CameraFixture* GPCamera;
}

class CEnginePrimitiveRenderUtil
{
public:
    static CameraFixture& GetCurrentCamera() { return *NEngineInternal::GPCamera; }
};

class CEnginePrimitiveManagerStaticMeshesBase
{
public:
    bool IsInsideFustrum(const C3DPrimitive2*, const CEngineInternalPrimitiveStaticMesh&);
};

// The selected implementation is reproduced verbatim for this standalone fixture.
bool CEnginePrimitiveManagerStaticMeshesBase::IsInsideFustrum(const C3DPrimitive2* mesh_prim, const CEngineInternalPrimitiveStaticMesh& primitive)
{
    C3DVector center = mesh_prim->GetBoundingSphere().GetCenter();
    primitive.ObjectTransform.TransformPoint(center);
    const float& sphere_radius = mesh_prim->GetBoundingSphere().GetRadius();
    float radius = sphere_radius * primitive.Scale;
    return CEnginePrimitiveRenderUtil::GetCurrentCamera().ClipSphere(center, radius);
}

int main()
{
    CameraFixture camera;
    camera.position = C3DVector(2.0f, -1.0f, 3.0f);
    NEngineInternal::GPCamera = &camera;
    CEnginePrimitiveManagerStaticMeshesBase manager;
    C3DPrimitive2 mesh;
    mesh.BoundingSphere.Center = C3DVector(1.0f, 2.0f, -3.0f);
    mesh.BoundingSphere.Radius = 2.0f;

    CEngineInternalPrimitiveStaticMesh primitive;
    CMatrix3x4& m = primitive.ObjectTransform;
    m.E11 = 0; m.E12 = 2; m.E13 = 0;
    m.E21 = -3; m.E22 = 0; m.E23 = 0;
    m.E31 = 0; m.E32 = 0; m.E33 = 4;

    for (int x = -2; x <= 2; ++x) {
        for (int y = -2; y <= 2; ++y) {
            for (int z = 0; z <= 8; ++z) {
                for (int scale = 0; scale <= 2; ++scale) {
                    m.E41 = 8.0f + static_cast<float>(x);
                    m.E42 = -3.0f + static_cast<float>(y);
                    m.E43 = 15.0f + static_cast<float>(z);
                    primitive.Scale = 0.5f * static_cast<float>(scale);
                    camera.called = false;
                    const bool actual = manager.IsInsideFustrum(&mesh, primitive);
                    const bool expected = x >= -scale && x <= scale &&
                                          y >= -scale && y <= scale &&
                                          z >= 2 - scale && z <= 6 + scale;
                    assert(actual == expected);
                    assert(camera.called && camera.captured_radius == scale);
                    assert(camera.captured_center.X == 2.0f + x);
                    assert(camera.captured_center.Y == -1.0f + y);
                    assert(camera.captured_center.Z == 3.0f + z);
                    assert(mesh.BoundingSphere.Center.X == 1.0f);
                    assert(mesh.BoundingSphere.Center.Y == 2.0f);
                    assert(mesh.BoundingSphere.Center.Z == -3.0f);
                    assert(mesh.BoundingSphere.Radius == 2.0f);
                }
            }
        }
    }
    std::puts("00b9d730_TEST PASS");
    return 0;
}
