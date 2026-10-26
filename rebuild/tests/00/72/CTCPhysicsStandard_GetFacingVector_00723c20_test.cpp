#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet { public: C3DVector Up, Forward; };
struct CTCPhysicsStandard {
    CRightHandedSet RHSet, OldRHSet;
    C3DVector GetFacingVector(float t)
    {
        C3DVector v = (RHSet.Forward - OldRHSet.Forward) * t + OldRHSet.Forward;
        return v;
    }
};
int main()
{
    CTCPhysicsStandard p; p.OldRHSet.Forward = C3DVector(1.0f, -3.0f, 4.0f); p.RHSet.Forward = C3DVector(3.0f, 1.0f, 6.0f);
    p.RHSet.Up = C3DVector(9.0f, 9.0f, 9.0f); p.OldRHSet.Up = C3DVector(7.0f, 7.0f, 7.0f);
    C3DVector r = p.GetFacingVector(0.5f);
    if (r.X != 2.0f || r.Y != -1.0f || r.Z != 5.0f) { std::printf("00723c20_TEST FAIL\n"); return 1; }
    std::printf("00723c20_TEST PASS\n");
    return 0;
}