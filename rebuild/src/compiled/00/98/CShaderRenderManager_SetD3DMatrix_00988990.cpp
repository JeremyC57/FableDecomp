// CShaderRenderManager::SetD3DMatrix(const CMatrix& matrix) -- expand a 4x3 engine matrix into a D3D 4x4 matrix (rows padded with 0, last row's w = 1), returned by value
// Leaf: SetD3DMatrix
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00988990, length 144 bytes (131 + int3 padding). No callees.
// Shape: `return D3DXMATRIX(m0, m1, m2, 0, m3, m4, m5, 0, m6, m7, m8, 0, m9, m10, m11, 1);` through the
// inline sixteen-float constructor, built straight into the hidden result: the arguments evaluate right to
// left, so the last seven source floats are loaded onto the x87 stack (eight-register limit) and the first
// five go through integer registers into frame temporaries (one in the dead argument slot); the three zero
// pads share one register and w is the 1.0f bit pattern. `this` is unused; `ret 8` (result pointer + matrix).
struct CMatrix {                                 // 4 rows of 3 floats
    float m[12];
};

struct D3DXMATRIX {
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
    D3DXMATRIX(float f11, float f12, float f13, float f14,
               float f21, float f22, float f23, float f24,
               float f31, float f32, float f33, float f34,
               float f41, float f42, float f43, float f44)
    {
        _11 = f11; _12 = f12; _13 = f13; _14 = f14;
        _21 = f21; _22 = f22; _23 = f23; _24 = f24;
        _31 = f31; _32 = f32; _33 = f33; _34 = f34;
        _41 = f41; _42 = f42; _43 = f43; _44 = f44;
    }
};

struct CShaderRenderManager {
    D3DXMATRIX SetD3DMatrix(const CMatrix& matrix);
};

D3DXMATRIX CShaderRenderManager::SetD3DMatrix(const CMatrix& matrix)
{
    return D3DXMATRIX(matrix.m[0], matrix.m[1], matrix.m[2], 0.0f,
                      matrix.m[3], matrix.m[4], matrix.m[5], 0.0f,
                      matrix.m[6], matrix.m[7], matrix.m[8], 0.0f,
                      matrix.m[9], matrix.m[10], matrix.m[11], 1.0f);
}