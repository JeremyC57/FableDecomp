// Behaviour test for 0x00988990 CShaderRenderManager::SetD3DMatrix: 4x3 -> padded 4x4.
#include <cstdio>
struct CMatrix { float m[12]; };
struct D3DXMATRIX {
    float v[16];
    D3DXMATRIX(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p)
    { v[0] = a; v[1] = b; v[2] = c; v[3] = d; v[4] = e; v[5] = f; v[6] = g; v[7] = h; v[8] = i; v[9] = j; v[10] = k; v[11] = l; v[12] = m; v[13] = n; v[14] = o; v[15] = p; }
};
struct Mgr {
    D3DXMATRIX SetD3DMatrix(const CMatrix& x)
    {
        return D3DXMATRIX(x.m[0], x.m[1], x.m[2], 0.0f, x.m[3], x.m[4], x.m[5], 0.0f, x.m[6], x.m[7], x.m[8], 0.0f, x.m[9], x.m[10], x.m[11], 1.0f);
    }
};
int main()
{
    CMatrix c; for (int i = 0; i < 12; ++i) c.m[i] = (float)(i + 1);
    Mgr g; D3DXMATRIX r = g.SetD3DMatrix(c);
    if (r.v[0] != 1 || r.v[3] != 0 || r.v[4] != 4 || r.v[7] != 0 || r.v[10] != 9 || r.v[12] != 10 || r.v[14] != 12 || r.v[15] != 1) { std::printf("00988990_TEST FAIL\n"); return 1; }
    std::printf("00988990_TEST PASS\n");
    return 0;
}
