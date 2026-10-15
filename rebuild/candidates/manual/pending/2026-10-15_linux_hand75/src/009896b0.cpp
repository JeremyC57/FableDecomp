// CShaderRenderManager::SetShadowTextureTransform(const CMatrix4x4* matrix) -- uploads the shadow texture matrix to the current layout's shadow-texture registers (+0xe4) as 4 transposed vectors
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009896B0, length 25 bytes.
// Layout: engine/CVertexShaderConstantLayout.h (its CShaderConstantRange members viewed as {vptr, Offset, Count}).
// Callees: CShaderRenderManager::SetVSMatrixConstantTransposed @ 0x00988840 (`__thiscall`, register, matrix, vector count).
// Shape: a member forwarder on `this` (ecx kept), the matrix argument read first, the layout register second,
// `push 4` as the row count; three pushed arguments against one received, so no tail jump (`call ; ret 4`).
struct CMatrix4x4 { float m[16]; };

#include "engine/CVertexShaderConstantLayout.h"

struct CShaderConstantRange {                   // the 12-byte ranges of engine/CVertexShaderConstantLayout.h (PDB layout)
    void*         __vftable;                    // +0
    long          Offset;                       // +4: first constant register
    unsigned long Count;                        // +8: vector count
};

inline const CShaderConstantRange& ConstantRange(const unsigned char* range)
{
    return *reinterpret_cast<const CShaderConstantRange*>(range);
}

struct CShaderRenderManager {
    void*                        __vftable;                 // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004

    void SetVSMatrixConstantTransposed(unsigned long startRegister, const CMatrix4x4* matrix, unsigned long rows);   // 0x00988840
    void SetShadowTextureTransform(const CMatrix4x4* matrix);
};

void CShaderRenderManager::SetShadowTextureTransform(const CMatrix4x4* matrix)
{
    SetVSMatrixConstantTransposed(ConstantRange(CurrentVSConstantLayout->ShadowTextureTransform).Offset, matrix, 4);
}
