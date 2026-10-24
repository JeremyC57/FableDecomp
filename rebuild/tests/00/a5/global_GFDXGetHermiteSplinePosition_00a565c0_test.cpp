// Behaviour test for 0x00A565C0 GFDXGetHermiteSplinePosition(C3DVector&, p0, t0, p1, t1, float)
#include <cstdio>

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct D3DXVECTOR3 {
    float x, y, z;
    D3DXVECTOR3() { }
    D3DXVECTOR3(float fx, float fy, float fz) : x(fx), y(fy), z(fz) { }
};
// stand-in for the d3dx9_25 import (thunk 0x00BFEFDA): the cubic Hermite basis
static D3DXVECTOR3* D3DXVec3Hermite(D3DXVECTOR3* pOut, const D3DXVECTOR3* pV1, const D3DXVECTOR3* pT1,
                                    const D3DXVECTOR3* pV2, const D3DXVECTOR3* pT2, float s)
{
    float s2 = s * s, s3 = s2 * s;
    float h1 = 2.0f * s3 - 3.0f * s2 + 1.0f, h2 = s3 - 2.0f * s2 + s, h3 = -2.0f * s3 + 3.0f * s2, h4 = s3 - s2;
    pOut->x = h1 * pV1->x + h2 * pT1->x + h3 * pV2->x + h4 * pT2->x;
    pOut->y = h1 * pV1->y + h2 * pT1->y + h3 * pV2->y + h4 * pT2->y;
    pOut->z = h1 * pV1->z + h2 * pT1->z + h3 * pV2->z + h4 * pT2->z;
    return pOut;
}
void GFDXGetHermiteSplinePosition(C3DVector& out, const C3DVector& p0, const C3DVector& t0, const C3DVector& p1,
                                  const C3DVector& t1, float s)
{
    D3DXVECTOR3 dxP0(p0.X, p0.Y, p0.Z);
    D3DXVECTOR3 dxT0(t0.X, t0.Y, t0.Z);
    D3DXVECTOR3 dxP1(p1.X, p1.Y, p1.Z);
    D3DXVECTOR3 dxT1(t1.X, t1.Y, t1.Z);
    D3DXVECTOR3 dxOut;
    D3DXVec3Hermite(&dxOut, &dxP0, &dxT0, &dxP1, &dxT1, s);
    out = C3DVector(dxOut.x, dxOut.y, dxOut.z);
}

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    C3DVector p0(0.0f, 0.0f, 0.0f), t0(1.0f, 0.0f, 0.0f), p1(2.0f, 4.0f, -6.0f), t1(0.0f, 1.0f, 0.0f), out;
    GFDXGetHermiteSplinePosition(out, p0, t0, p1, t1, 0.0f);
    bool ok = Near(out.X, 0.0f) && Near(out.Y, 0.0f) && Near(out.Z, 0.0f);
    GFDXGetHermiteSplinePosition(out, p0, t0, p1, t1, 1.0f);
    ok = ok && Near(out.X, 2.0f) && Near(out.Y, 4.0f) && Near(out.Z, -6.0f);
    GFDXGetHermiteSplinePosition(out, p0, t0, p1, t1, 0.5f);    // h = (0.5, 0.125, 0.5, -0.125)
    ok = ok && Near(out.X, 1.125f) && Near(out.Y, 1.875f) && Near(out.Z, -3.0f);
    if (!ok) { std::printf("00a565c0_TEST FAIL\n"); return 1; }
    std::printf("00a565c0_TEST PASS\n");
    return 0;
}