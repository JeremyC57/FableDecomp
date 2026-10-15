#include <cstdio>
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CRetailVector { void* m_first; void* m_last; void* m_end; };
struct CTCPhysicsRigidBody {
    virtual ~CTCPhysicsRigidBody() {}
    CRetailVector   CollisionNormal, CollidingVertexIndex, Collidors;
    void*           PPhysicsMeshInfo;
    CRightHandedSet RHSet;
    CRightHandedSet OldRHSet;
    C3DVector vfunc_70() { return RHSet.Forward; }
};
int main()
{
    CTCPhysicsRigidBody b;
    b.RHSet.Up.Z = 5.0f; b.RHSet.Forward.X = 0.25f; b.RHSet.Forward.Y = -1.0f; b.RHSet.Forward.Z = 8.0f;
    b.OldRHSet.Forward.X = 99.0f;
    C3DVector v = b.vfunc_70();
    if (v.X != 0.25f || v.Y != -1.0f || v.Z != 8.0f) { std::printf("00759b20_TEST FAIL\n"); return 1; }
    std::printf("00759b20_TEST PASS\n");
    return 0;
}
