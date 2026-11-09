// Behaviour test for 0x00BEBAA0 CEngineInternalPrimitiveMeshBase::CalcObjectMatrix: rows -Right*s, -Forward*s, Up*s, translation pos,
// with Right = Forward x Up (CRightHandedSet::GetRight). C++03 models of C3DVector / CRightHandedSet / CMatrix3x4.
#include <stdio.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    const C3DVector operator^(const C3DVector& o) const { return C3DVector(Y*o.Z-Z*o.Y, Z*o.X-X*o.Z, X*o.Y-Y*o.X); }
    const C3DVector& operator*=(float v) { X*=v; Y*=v; Z*=v; return *this; }
};
class CRightHandedSet { protected: C3DVector Up, Forward; public:
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
    const C3DVector& GetForward() const { return Forward; }
    const C3DVector& GetUp() const { return Up; }
    C3DVector GetRight() const { return Forward ^ Up; }
};
class CMatrix3x4 { public:
    float E11,E12,E13,E21,E22,E23,E31,E32,E33,E41,E42,E43;
    void SetTranslateOnly(const C3DVector& t) { E41=t.X; E42=t.Y; E43=t.Z; }
};
class CEngineInternalPrimitiveMeshBase { public:
    static void __fastcall CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale);
};
void __fastcall CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(CMatrix3x4& matrix, const C3DVector& pos, const CRightHandedSet& rhset, float scale)
{
    C3DVector forward = rhset.GetForward();
    C3DVector right = rhset.GetRight();
    C3DVector up = rhset.GetUp();
    right *= scale;
    forward *= scale;
    up *= scale;
    matrix.E11 = (-right.X);
    matrix.E12 = (-right.Y);
    matrix.E13 = (-right.Z);
    matrix.E21 = -forward.X;
    matrix.E22 = -forward.Y;
    matrix.E23 = -forward.Z;
    matrix.E31 = up.X;
    matrix.E32 = up.Y;
    matrix.E33 = up.Z;
    matrix.SetTranslateOnly(pos);
}
static int Near(float a, float b) { return fabs(a - b) < 1e-5f; }
int main()
{
    // Up = +Z, Forward = +Y: Right = Y x Z = +X
    CRightHandedSet set(C3DVector(0, 0, 1), C3DVector(0, 1, 0));
    CMatrix3x4 m;
    CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(m, C3DVector(5, 6, 7), set, 2.0f);
    int ok = Near(m.E11, -2) && Near(m.E12, 0) && Near(m.E13, 0) && Near(m.E21, 0) && Near(m.E22, -2) && Near(m.E23, 0)
          && Near(m.E31, 0) && Near(m.E32, 0) && Near(m.E33, 2) && m.E41 == 5 && m.E42 == 6 && m.E43 == 7;
    // a skewed frame: Up (1,2,3), Forward (4,5,6): Right = F x U = (5*3-6*2, 6*1-4*3, 4*2-5*1) = (3,-6,3)
    CRightHandedSet skew(C3DVector(1, 2, 3), C3DVector(4, 5, 6));
    CEngineInternalPrimitiveMeshBase::CalcObjectMatrix(m, C3DVector(0, 0, 0), skew, 0.5f);
    ok = ok && Near(m.E11, -1.5f) && Near(m.E12, 3) && Near(m.E13, -1.5f) && Near(m.E21, -2) && Near(m.E22, -2.5f) && Near(m.E23, -3)
          && Near(m.E31, 0.5f) && Near(m.E32, 1) && Near(m.E33, 1.5f);
    puts(ok ? "00bebaa0_TEST PASS" : "00bebaa0_TEST FAIL");
    return ok ? 0 : 1;
}