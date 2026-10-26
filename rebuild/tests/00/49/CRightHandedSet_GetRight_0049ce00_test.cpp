#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}

class CRightHandedSet { public:
    C3DVector Up; C3DVector Forward;
    CRightHandedSet(const C3DVector& u, const C3DVector& f) : Up(u), Forward(f) { }
    C3DVector GetRight() const { return CrossProduct(Forward, Up); }
};

int main()
{
    // Forward = +Y, Up = +Z: right = Y x Z = +X.
    CRightHandedSet s(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f));
    C3DVector r = s.GetRight();
    if (r.X != 1.0f || r.Y != 0.0f || r.Z != 0.0f) { std::printf("0049ce00_TEST FAIL\n"); return 1; }
    // General case against the written-out components.
    CRightHandedSet t(C3DVector(1.0f, 2.0f, 3.0f), C3DVector(-4.0f, 0.5f, 2.0f));
    C3DVector q = t.GetRight();
    float x = 3.0f * 0.5f - 2.0f * 2.0f, y = 1.0f * 2.0f - 3.0f * -4.0f, z = 2.0f * -4.0f - 1.0f * 0.5f;
    if (q.X != x || q.Y != y || q.Z != z) { std::printf("0049ce00_TEST FAIL\n"); return 1; }
    std::printf("0049ce00_TEST PASS\n");
    return 0;
}