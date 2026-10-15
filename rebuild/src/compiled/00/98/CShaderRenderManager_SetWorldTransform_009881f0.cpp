// CShaderRenderManager::SetWorldTransform(const CMatrix* matrix) -- store the 3x4 world matrix transposed into the 4x4 WorldMatrix (last row 0,0,0,1), mark the dependent constants dirty and clear WorldMatrixIsIdentity
// Leaf: SetWorldTransform
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009881F0, length 160 bytes (156 + int3 padding). No callees.
// Shape: as the landed SetViewTransform (0x00988350): twelve dword copies in destination order reading the
// source column-wise, one zero register for the last row, the 1.0f bit pattern, the dirty mask OR-ed in
// memory, and the identity flag cleared from the zero register's low byte.
// Layout: PDB CShaderRenderManager has WorldMatrixIsIdentity +0x1f8, WorldMatrix +0x200, UpdateFlags +0x3f8;
// retail's 12-byte CArrays put them at +0x1e8 / +0x1f0 / +0x3d8. Floats are copied as dwords (modelled as
// unsigned int, as in the landed relatives).
struct CMatrix {                                 // 3x4 row-major source (12 dwords)
    unsigned int m[12];
};

struct CShaderRenderManager {
    unsigned char _pad000[0x1e8];
    bool          WorldMatrixIsIdentity;         // +0x1e8
    unsigned char _pad1e9[7];
    unsigned int  WorldMatrix[16];               // +0x1f0
    unsigned char _pad230[0x3d8 - 0x230];
    unsigned int  UpdateFlags;                   // +0x3d8
    void SetWorldTransform(const CMatrix* matrix);
};

void CShaderRenderManager::SetWorldTransform(const CMatrix* matrix)
{
    WorldMatrix[0] = matrix->m[0];
    WorldMatrix[1] = matrix->m[3];
    WorldMatrix[2] = matrix->m[6];
    WorldMatrix[3] = matrix->m[9];
    WorldMatrix[4] = matrix->m[1];
    WorldMatrix[5] = matrix->m[4];
    WorldMatrix[6] = matrix->m[7];
    WorldMatrix[7] = matrix->m[10];
    WorldMatrix[8] = matrix->m[2];
    WorldMatrix[9] = matrix->m[5];
    WorldMatrix[10] = matrix->m[8];
    WorldMatrix[11] = matrix->m[11];
    WorldMatrix[12] = 0;
    WorldMatrix[13] = 0;
    WorldMatrix[14] = 0;
    WorldMatrix[15] = 0x3f800000u;
    unsigned int* flags = &UpdateFlags;          // a pointer local gives the memory-form `or`
    *flags |= 0xc990u;
    WorldMatrixIsIdentity = false;
}