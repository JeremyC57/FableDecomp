// CShaderRenderManager::SetProjectionTransform(const CMatrix4x4& projection) -- store the projection matrix, transpose it in place for the shader constants, and mark the dependent transforms dirty
// Retail: Fable.exe (TLC, VC7.1) @ 0x00988540, 54 bytes (`?SetProjectionTransform@CShaderRenderManager@@QAEXABVCMatrix4x4@@@Z`, Ego_r.pdb).
// Callees: CMatrix4x4::Transpose() @ 0x00A5CAB0 (in place: copy to an aligned temporary, write it back by
// columns; the manifest label "C3DMeshFileBoneChunk::SetLocalizationMatrix" is propagated).
//
// Shape: the 64-byte POD matrix assignment is a `rep movsd` with the destination address kept in eax for
// the `this` of Transpose; `UpdateFlags |= 0xC990` is a load / or / store interleaved with the pops.
// Retail layout: ProjectionMatrix +0x270 and UpdateFlags +0x3D8 (Ego_r.pdb +0x280 / +0x3F8). No engine header
// exists for CShaderRenderManager.
// Leaf: ?SetProjectionTransform@CShaderRenderManager

struct CMatrix4x4 {
    float M[4][4];
    void Transpose();                            // 0x00A5CAB0
};

struct CShaderRenderManager {
    unsigned char _pad_0x000[0x270];
    CMatrix4x4    ProjectionMatrix;              // +0x270
    unsigned char _pad_0x2b0[0x3d8 - 0x2b0];
    unsigned long UpdateFlags;                   // +0x3D8
    void SetProjectionTransform(const CMatrix4x4& projection);
};

void CShaderRenderManager::SetProjectionTransform(const CMatrix4x4& projection)
{
    ProjectionMatrix = projection;
    ProjectionMatrix.Transpose();
    UpdateFlags |= 0xc990;
}
