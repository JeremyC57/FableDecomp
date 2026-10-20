// Behaviour test for 0x00a0b320 CRenderManagerCore::SetPerspectiveTransform(float, float, float, float)
#include <cstdio>
#include <cmath>

struct _D3DMATRIX {
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
};
struct IDirect3DDevice9 {
    long state; _D3DMATRIX m; int calls;
    long SetTransform(long s, const void* matrix) { state = s; m = *(const _D3DMATRIX*)matrix; ++calls; return 0; }
};
struct CRenderManagerCore {
    IDirect3DDevice9* PD3DDevice;
    void SetD3DMatrix(_D3DMATRIX& m, float m11, float m12, float m13, float m14,
                      float m21, float m22, float m23, float m24,
                      float m31, float m32, float m33, float m34,
                      float m41, float m42, float m43, float m44)
    {
        m._11 = m11; m._12 = m12; m._13 = m13; m._14 = m14;
        m._21 = m21; m._22 = m22; m._23 = m23; m._24 = m24;
        m._31 = m31; m._32 = m32; m._33 = m33; m._34 = m34;
        m._41 = m41; m._42 = m42; m._43 = m43; m._44 = m44;
    }
    void SetPerspectiveTransform(float nearPlane, float farPlane, float nearDepth, float farDepth)
    {
        float depthOffset = (nearDepth - farDepth) * nearPlane * farPlane / (farPlane - nearPlane);
        _D3DMATRIX d3dMatrix;
        SetD3DMatrix(d3dMatrix, 1.0f, 0.0f, 0.0f, 0.0f,
                                0.0f, 1.0f, 0.0f, 0.0f,
                                0.0f, 0.0f, nearDepth - depthOffset / nearPlane, 1.0f,
                                0.0f, 0.0f, depthOffset, 0.0f);
        PD3DDevice->SetTransform(3, &d3dMatrix);
    }
};

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    IDirect3DDevice9 dev; dev.calls = 0; dev.state = 0;
    CRenderManagerCore rm; rm.PD3DDevice = &dev;
    rm.SetPerspectiveTransform(1.0f, 100.0f, 0.0f, 1.0f);
    const _D3DMATRIX& m = dev.m;
    // depth = _33 + _43 / z must map z = near -> 0 and z = far -> 1
    float dn = m._33 + m._43 / 1.0f, df = m._33 + m._43 / 100.0f;
    if (dev.calls != 1 || dev.state != 3 || m._11 != 1.0f || m._22 != 1.0f || m._34 != 1.0f || m._44 != 0.0f
        || m._12 != 0.0f || m._21 != 0.0f || m._31 != 0.0f || m._41 != 0.0f || !Near(dn, 0.0f) || !Near(df, 1.0f)
        || !Near(m._43, -100.0f / 99.0f)) { std::printf("00a0b320_TEST FAIL\n"); return 1; }
    std::printf("00a0b320_TEST PASS\n");
    return 0;
}