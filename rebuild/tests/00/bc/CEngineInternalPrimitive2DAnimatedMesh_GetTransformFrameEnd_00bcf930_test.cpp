// behaviour test for 00bcf930 GetTransformFrameStart/End: normalised orientation, CalcObjectMatrix, then the -90 degree X rotation
#include <cstdio>
#include <cmath>
struct C3DVector {
    float X, Y, Z;
    void Normalise() { float l = std::sqrt(X * X + Y * Y + Z * Z); X /= l; Y /= l; Z /= l; }
};
struct CRightHandedSet { C3DVector Up, Forward; void MakePerpendicular() {} };
struct CMatrix3x4 { float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
struct CMatrix3x3 {
    float E11, E12, E13, E21, E22, E23, E31, E32, E33;
    CMatrix3x3(float a, float b, float c, float d, float e, float f, float g, float h, float i)
        : E11(a), E12(b), E13(c), E21(d), E22(e), E23(f), E31(g), E32(h), E33(i) {}
};
static void Rotate(CMatrix3x4& m, const CMatrix3x3& b)
{
    CMatrix3x4 a(m);
    m.E11 = a.E11 * b.E11 + a.E12 * b.E21 + a.E13 * b.E31;
    m.E12 = a.E11 * b.E12 + a.E12 * b.E22 + a.E13 * b.E32;
    m.E13 = a.E11 * b.E13 + a.E12 * b.E23 + a.E13 * b.E33;
    m.E21 = a.E21 * b.E11 + a.E22 * b.E21 + a.E23 * b.E31;
    m.E22 = a.E21 * b.E12 + a.E22 * b.E22 + a.E23 * b.E32;
    m.E23 = a.E21 * b.E13 + a.E22 * b.E23 + a.E23 * b.E33;
    m.E31 = a.E31 * b.E11 + a.E32 * b.E21 + a.E33 * b.E31;
    m.E32 = a.E31 * b.E12 + a.E32 * b.E22 + a.E33 * b.E32;
    m.E33 = a.E31 * b.E13 + a.E32 * b.E23 + a.E33 * b.E33;
    m.E41 = a.E41 * b.E11 + a.E42 * b.E21 + a.E43 * b.E31;
    m.E42 = a.E41 * b.E12 + a.E42 * b.E22 + a.E43 * b.E32;
    m.E43 = a.E41 * b.E13 + a.E42 * b.E23 + a.E43 * b.E33;
}
// stand-in CalcObjectMatrix: rows Up*s, Forward*s, (1,2,3)*s, position
static void CalcObjectMatrix(CMatrix3x4& out, const C3DVector& p, const CRightHandedSet& o, float s)
{
    out.E11 = o.Up.X * s; out.E12 = o.Up.Y * s; out.E13 = o.Up.Z * s;
    out.E21 = o.Forward.X * s; out.E22 = o.Forward.Y * s; out.E23 = o.Forward.Z * s;
    out.E31 = 1 * s; out.E32 = 2 * s; out.E33 = 3 * s;
    out.E41 = p.X; out.E42 = p.Y; out.E43 = p.Z;
}
struct CMesh { float Radius; C3DVector Position; CRightHandedSet Orientation; };
static bool GetTransform(const CMesh* self, CMatrix3x4& transform)
{
    {
        CRightHandedSet orientation(self->Orientation);
        orientation.MakePerpendicular();
        orientation.Up.Normalise();
        orientation.Forward.Normalise();
        CalcObjectMatrix(transform, self->Position, orientation, self->Radius * 0.01f);
    }
    Rotate(transform, CMatrix3x3(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f));
    return true;
}
static int Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    CMesh mesh;
    mesh.Radius = 200.0f;
    mesh.Position.X = 4; mesh.Position.Y = 5; mesh.Position.Z = 6;
    mesh.Orientation.Up.X = 0; mesh.Orientation.Up.Y = 0; mesh.Orientation.Up.Z = 3;
    mesh.Orientation.Forward.X = 0; mesh.Orientation.Forward.Y = 4; mesh.Orientation.Forward.Z = 0;
    CMatrix3x4 m;
    if (!GetTransform(&mesh, m)) { std::printf("00bcf930_TEST FAIL\n"); return 1; }
    // (x, y, z) -> (x, z, -y): Up (0,0,2) -> (0,2,0); Forward (0,2,0) -> (0,0,-2); (2,4,6) -> (2,6,-4); pos -> (4,6,-5)
    float want[12] = { 0, 2, 0, 0, 0, -2, 2, 6, -4, 4, 6, -5 };
    const float* pm = &m.E11;
    for (int i = 0; i < 12; ++i)
        if (!Near(pm[i], want[i])) { std::printf("00bcf930_TEST FAIL\n"); return 1; }
    std::printf("00bcf930_TEST PASS\n");
    return 0;
}