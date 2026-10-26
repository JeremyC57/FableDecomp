// CShaderRenderManager::ResetWorldTransform() -- make the world matrix the identity (once) and mark the transforms dirty
// Retail: Fable.exe @ 0x00988290, 131 bytes, `ret`. Unless WorldMatrixIsIdentity is already set, the 4x4 WorldMatrix
// is set to the identity inline (the twelve off-diagonal zeros from one zeroed register, then the four 1.0f stores as
// immediates; written row by row, one statement per element, VC hoists 1.0f into edx and sinks the diagonal stores), UpdateFlags gains 0xC990 and WorldMatrixIsIdentity is set.
// Layout: retail offsets (Ego_r.pdb names): WorldMatrixIsIdentity +0x1E8, WorldMatrix +0x1F0, UpdateFlags +0x3D8 (the
// PDB has them 0x10 / 0x10 / 0x20 higher), as the landed UpdateWorldTransform 0x00988E30; local model.
class CMatrix4x4 { public:
    float m[4][4];
    void SetIdentity()
    {
        m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
        m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
        m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
        m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
    }
};
enum {
    SRM_UPDATE_WORLD_TRANSFORM_FLAGS = 0xC990
};
class CShaderRenderManager { public:
    void*         __vftable;                    // +0x000
    unsigned char _pad004[0x1e4];
    bool          WorldMatrixIsIdentity;        // +0x1e8
    unsigned char _pad1e9[0x7];
    CMatrix4x4    WorldMatrix;                  // +0x1f0
    unsigned char _pad230[0x1a8];
    unsigned long UpdateFlags;                  // +0x3d8
    void ResetWorldTransform();
};
void CShaderRenderManager::ResetWorldTransform()
{
    if (!WorldMatrixIsIdentity) {
        WorldMatrix.SetIdentity();
        UpdateFlags |= SRM_UPDATE_WORLD_TRANSFORM_FLAGS;
        WorldMatrixIsIdentity = true;
    }
}
