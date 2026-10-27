// Behaviour test for CPhysicsMeshObject::operator= (0x006A6BE0): re-points the counted mesh (release old, add-ref new) and copies matrix/scale.
#include <cstdio>
static int g_deleted = 0;
class CPhysicsMesh { public:
    int RefCount;
    virtual ~CPhysicsMesh() {}
    virtual void DeleteThis() { ++g_deleted; }
};
struct MeshPtr {
    CPhysicsMesh* Object;
    MeshPtr& operator=(const MeshPtr& o)
    {
        if (Object != o.Object) {
            CPhysicsMesh* old = Object;
            if (old) { if (--old->RefCount == 0) old->DeleteThis(); Object = 0; }
            Object = o.Object;
            if (Object) ++Object->RefCount;
        }
        return *this;
    }
};
class CMatrix3x4 { public: float M[12]; };
class CPhysicsMeshObject { public:
    MeshPtr Mesh; CMatrix3x4 PositionMatrix; float Scale; float OO_Scale;
    CPhysicsMeshObject& operator=(const CPhysicsMeshObject& o)
    { Mesh = o.Mesh; PositionMatrix = o.PositionMatrix; Scale = o.Scale; OO_Scale = o.OO_Scale; return *this; }
};
int main()
{
    CPhysicsMesh m1, m2; m1.RefCount = 1; m2.RefCount = 1;
    CPhysicsMeshObject a, b;
    a.Mesh.Object = &m1; b.Mesh.Object = &m2;
    for (int i = 0; i < 12; ++i) { a.PositionMatrix.M[i] = 0.0f; b.PositionMatrix.M[i] = (float)i; }
    b.Scale = 2.0f; b.OO_Scale = 0.5f;
    CPhysicsMeshObject& r = (a = b);
    if (&r != &a || a.Mesh.Object != &m2 || m2.RefCount != 2 || m1.RefCount != 0 || g_deleted != 1
        || a.PositionMatrix.M[11] != 11.0f || a.Scale != 2.0f || a.OO_Scale != 0.5f) { std::printf("006a6be0_TEST FAIL\n"); return 1; }
    a = b;
    if (m2.RefCount != 2 || g_deleted != 1) { std::printf("006a6be0_TEST FAIL\n"); return 1; }
    std::printf("006a6be0_TEST PASS\n");
    return 0;
}