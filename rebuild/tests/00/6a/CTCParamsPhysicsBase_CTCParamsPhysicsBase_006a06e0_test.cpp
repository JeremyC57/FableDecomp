// Behaviour test for CTCParamsPhysicsBase's constructor @ 0x006A06E0: position and both basis vectors copied.
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CTCParamsPhysicsBase { public:
    C3DVector Position;
    CRightHandedSet Rhs;
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs) : Position(position), Rhs(rhs) {}
};
int main()
{
    CRightHandedSet rhs; rhs.Up = C3DVector(0.0f, 0.0f, 1.0f); rhs.Forward = C3DVector(1.0f, 2.0f, 3.0f);
    CTCParamsPhysicsBase p(C3DVector(4.0f, 5.0f, 6.0f), rhs);
    if (p.Position.X != 4.0f || p.Position.Z != 6.0f || p.Rhs.Up.Z != 1.0f || p.Rhs.Forward.Y != 2.0f || p.Rhs.Forward.Z != 3.0f)
    { std::printf("006a06e0_TEST FAIL\n"); return 1; }
    std::printf("006a06e0_TEST PASS\n");
    return 0;
}