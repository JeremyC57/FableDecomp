// Behaviour test for 00987bf0: body copied from src/00987bf0.cpp (CMatrix34_Retail::Transform, the catalog's propagated "CQuaternion::operator*").
#include <cstdio>
#include <cstring>
#include <math.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
class C4DVector { public:
    float X, Y, Z, W;
};
struct CRow_Retail {                               // one matrix row: three direction terms and the translation term
    float X, Y, Z, T;
    float Dot3(const C4DVector& o) const { return Z * o.Z + Y * o.Y + X * o.X; }   // C3DVector::DotProduct order (landed 0x00C256A0)
};
struct CMatrix34_Retail {
    CRow_Retail R[3];
    void Transform(const C4DVector& in, C4DVector& out, float w) const;
};

void CMatrix34_Retail::Transform(const C4DVector& in, C4DVector& out, float w) const
{
    C4DVector t = in;
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
    C4DVector in; in.X = 1; in.Y = 2; in.Z = 3; in.W = 99;
    C4DVector out; memset(&out, 0, sizeof(out));
    // 1. w = 1: full affine transform; out.W = w; in.W ignored
    m.Transform(in, out, 1.0f);
    CHECK(Near(out.X, 24)); CHECK(Near(out.Y, 52)); CHECK(Near(out.Z, 80)); CHECK(out.W == 1.0f);
    // 2. w = 0: rotation part only
    m.Transform(in, out, 0.0f);
    CHECK(Near(out.X, 14)); CHECK(Near(out.Y, 32)); CHECK(Near(out.Z, 50)); CHECK(out.W == 0.0f);
    // 3. w = 2: translation doubled
    m.Transform(in, out, 2.0f);
    CHECK(Near(out.X, 34)); CHECK(Near(out.Y, 72)); CHECK(Near(out.Z, 110)); CHECK(out.W == 2.0f);
    // 4. in and out aliased: the input copy makes it safe
    C4DVector both = in; m.Transform(both, both, 1.0f);
    CHECK(Near(both.X, 24)); CHECK(Near(both.Y, 52)); CHECK(Near(both.Z, 80)); CHECK(both.W == 1.0f);
    if (fails == 0) printf("00987bf0_TEST PASS\n");
    else printf("00987bf0_TEST FAIL (%d)\n", fails);
    return fails;
}