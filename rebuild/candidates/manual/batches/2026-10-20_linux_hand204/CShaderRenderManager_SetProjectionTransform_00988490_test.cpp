// Behaviour test for CShaderRenderManager::SetProjectionTransform(16 floats) (0x00988490): the matrix is stored transposed and the dirty flags are or-ed in.
#include <cstdio>
struct _D3DMATRIX {
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
};

struct CShaderRenderManager {
    unsigned char _pad_0x000[0x270];
    _D3DMATRIX    ProjectionMatrix;              // +0x270 (stored transposed)
    unsigned char _pad_0x2b0[0x3d8 - 0x2b0];
    unsigned long UpdateFlags;                   // +0x3D8

    // PDB: private SetD3DMatrix(_D3DMATRIX&, 16 floats), inlined.
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
    void SetProjectionTransform(float m11, float m12, float m13, float m14,
                                float m21, float m22, float m23, float m24,
                                float m31, float m32, float m33, float m34,
                                float m41, float m42, float m43, float m44);
};

void CShaderRenderManager::SetProjectionTransform(float m11, float m12, float m13, float m14,
                                                  float m21, float m22, float m23, float m24,
                                                  float m31, float m32, float m33, float m34,
                                                  float m41, float m42, float m43, float m44)
{
    SetD3DMatrix(ProjectionMatrix, m11, m21, m31, m41, m12, m22, m32, m42,
                 m13, m23, m33, m43, m14, m24, m34, m44);
    UpdateFlags |= 0xc990;
}
int main()
{
    static CShaderRenderManager m;
    m.UpdateFlags = 0x10000001;
    m.SetProjectionTransform(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
    const float* p = &m.ProjectionMatrix._11;
    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c) {
            float want = (float)(c * 4 + r + 1);
            if (p[r * 4 + c] != want) { std::printf("00988490_TEST FAIL\n"); return 1; }
        }
    if (m.UpdateFlags != (0x10000001ul | 0xc990ul) || (char*)&m.ProjectionMatrix - (char*)&m != 0x270 ||
        (char*)&m.UpdateFlags - (char*)&m != 0x3d8) { std::printf("00988490_TEST FAIL\n"); return 1; }
    std::printf("00988490_TEST PASS\n");
    return 0;
}
