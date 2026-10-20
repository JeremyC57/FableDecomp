// Behaviour test for 00addfe0: body copied from src/00addfe0.cpp (CMatrix34_Retail::Transform, the catalog's propagated "CQuaternion::operator*").
#include <cstdio>
#include <cstring>
#include <math.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C4DVector {
    float X, Y, Z, W;
};
struct CRow_Retail {                               // one matrix row: three direction terms and the translation term
    float X, Y, Z, T;
    float Dot3(const C4DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }   // C3DVector::DotProduct order (landed 0x00C256A0)
};
struct CMatrix34_Retail {
    CRow_Retail R[3];
    void Transform(C4DVector& out, float w) const;
};

void CMatrix34_Retail::Transform(C4DVector& out, float w) const
{
    C4DVector t = out;
    out.X = R[0].Y * t.Y + R[0].Z * t.Z + R[0].T * w + R[0].X * t.X;
    float y = R[1].Dot3(t);
    y += R[1].T * w;
    out.Y = y;
    float z = R[2].Dot3(t);
    z += R[2].T * w;
    out.Z = z;
    out.W = w;
}
static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }
int main()
{
    CMatrix34_Retail m;
    // rows: [1 2 3 | 10], [4 5 6 | 20], [7 8 9 | 30]
    float v[12] = { 1, 2, 3, 10, 4, 5, 6, 20, 7, 8, 9, 30 };
    memcpy(&m, v, sizeof(v));
    C4DVector v1; v1.X = 1; v1.Y = 2; v1.Z = 3; v1.W = 99;
    // 1. w = 1: transformed in place; W = w; the old W is ignored
    m.Transform(v1, 1.0f);
    CHECK(Near(v1.X, 24)); CHECK(Near(v1.Y, 52)); CHECK(Near(v1.Z, 80)); CHECK(v1.W == 1.0f);
    // 2. w = 0: rotation part only
    C4DVector v2; v2.X = 1; v2.Y = 2; v2.Z = 3; v2.W = 5;
    m.Transform(v2, 0.0f);
    CHECK(Near(v2.X, 14)); CHECK(Near(v2.Y, 32)); CHECK(Near(v2.Z, 50)); CHECK(v2.W == 0.0f);
    // 3. w = 2: translation doubled
    C4DVector v3; v3.X = 1; v3.Y = 2; v3.Z = 3; v3.W = 0;
    m.Transform(v3, 2.0f);
    CHECK(Near(v3.X, 34)); CHECK(Near(v3.Y, 72)); CHECK(Near(v3.Z, 110)); CHECK(v3.W == 2.0f);
    if (fails == 0) printf("00addfe0_TEST PASS\n");
    else printf("00addfe0_TEST FAIL (%d)\n", fails);
    return fails;
}