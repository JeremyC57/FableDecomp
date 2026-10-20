// CShaderRenderManager::SetProjectionTransform(float m11, ..., float m44) -- store sixteen floats as the projection matrix, transposed for the shader constants, and mark the dependent transforms dirty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00988490, 173 bytes, `ret 40h` (sixteen float arguments). No callees.
// Leaf: ?SetProjectionTransform@CShaderRenderManager
// Shape: the sixteen-float overload of the landed 0x00988540 (which copies a CMatrix4x4 and calls Transpose).
//   The body is the inlined private SetD3DMatrix(D3DMATRIX&, 16 floats) helper (the same helper the landed
//   CRenderManagerCore::SetPerspectiveTransform 0x00A0B3E0 inlines) called with the arguments in transposed
//   order, so the stores walk the member linearly (+0x270, +0x274, ...) as eax/edx dword pairs while the loads
//   stride the argument block by 0x10. Writing the sixteen stores straight into the body instead lets VC7.1
//   reorder the pairs. `UpdateFlags |= 0xC990` folds to `or dword ptr [ecx+3D8h],imm`.
// Retail layout: ProjectionMatrix +0x270, UpdateFlags +0x3D8 (Ego_r.pdb +0x280 / +0x3F8, as in 0x00988540).
// No engine header exists for CShaderRenderManager.

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