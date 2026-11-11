#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
struct CTCPhysicsStandard {
    virtual ~CTCPhysicsStandard() {}
    void*           PPhysicsMeshInfo;
    C3DVector       OffsetVector3D;
    CRightHandedSet RHSet;
    CRightHandedSet OldRHSet;
    long            RHSetLastFrameSet;
    C3DVector vfunc_70() { return RHSet.Forward; }
};
int main()
{
    CTCPhysicsStandard p;
    p.RHSet.Up.X = 9.0f; p.RHSet.Forward.X = 1.0f; p.RHSet.Forward.Y = 2.0f; p.RHSet.Forward.Z = -3.5f;
    p.OldRHSet.Up.X = 7.0f;
    C3DVector v = p.vfunc_70();
    if (v.X != 1.0f || v.Y != 2.0f || v.Z != -3.5f) { std::printf("00723c00_TEST FAIL\n"); return 1; }
    std::printf("00723c00_TEST PASS\n");
    return 0;
}