// Behaviour test for CEngineInternalPrimitiveMeshBase::CalcObjectMatrix (0x00BEBAA0): object matrix from position, frame and scale.
#include <cstdio>
#include <cmath>
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
};
static C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}
class CRightHandedSet { public:
    C3DVector Up, Forward;
    CRightHandedSet(const C3DVector& u, const C3DVector& f) : Up(u), Forward(f) { }
    C3DVector GetRight() const { return CrossProduct(Forward, Up); }
};
class CMatrix3x4 { public: float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
static void CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale)
{
    C3DVector forward = rhset.Forward;
    C3DVector right = rhset.GetRight();
    C3DVector up = rhset.Up;
    right *= scale; forward *= scale; up *= scale;
    matrix.E11 = -right.X; matrix.E12 = -right.Y; matrix.E13 = -right.Z;
    matrix.E21 = -forward.X; matrix.E22 = -forward.Y; matrix.E23 = -forward.Z;
    matrix.E31 = up.X; matrix.E32 = up.Y; matrix.E33 = up.Z;
    matrix.E41 = pos.X; matrix.E42 = pos.Y; matrix.E43 = pos.Z;
}
static bool near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    CMatrix3x4 m;
    CRightHandedSet set(C3DVector(0, 0, 1), C3DVector(0, 1, 0));   // Z up, Y forward: Right = Y x Z = +X
    CalcObjectMatrix(m, C3DVector(5, 6, 7), set, 2.0f);
    bool ok = near(m.E11, -2) && near(m.E12, 0) && near(m.E13, 0) &&
              near(m.E21, 0) && near(m.E22, -2) && near(m.E23, 0) &&
              near(m.E31, 0) && near(m.E32, 0) && near(m.E33, 2) &&
              m.E41 == 5 && m.E42 == 6 && m.E43 == 7;
    if (!ok) { std::printf("00bebaa0_TEST FAIL\n"); return 1; }
    std::printf("00bebaa0_TEST PASS\n");
    return 0;
}
