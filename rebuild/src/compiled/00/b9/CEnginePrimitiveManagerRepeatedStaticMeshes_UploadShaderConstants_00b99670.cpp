// CEnginePrimitiveManagerRepeatedStaticMeshes::UploadShaderConstants(CEngineInternalPrimitiveRepeatedStaticMesh&, const C4DVector*, const C4DVector*, long) -- sets vertex-shader constants 1 (fade-scaled alpha transform) and 2 (1, -1, 1, 0), then uploads the object offsets and matrices to their constant registers
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B99670, length 147 bytes (+ int3 padding).
// Signature from Ego_r.pdb: ?UploadShaderConstants@CEnginePrimitiveManagerRepeatedStaticMeshes@@AAEXAAVCEngineInternalPrimitiveRepeatedStaticMesh@@PBVC4DVector@@1J@Z
// (`ret 10h`, `this` unused).
// Callees: 0x00989B00 CShaderRenderManager::SetUserVertexShaderConstant(unsigned long, float, float, float, float)
// and 0x0098B930 CShaderRenderManager::SetVSConstants(long, const void*, unsigned long), both on the shader render
// manager global @ 0x01436E14; the view record global @ 0x01436E80 (fade-distance scale at +0x88, landed
// 0x00B98DA0) and the engine global @ 0x01436E98 (constant register indices at +0x3030 and +0x303c).
// Layout: engine/CEngineInternalPrimitiveRepeatedStaticMesh.h (PDB): AlphaTransformMult +0x84, AlphaTransformOffset +0x88.
// Shape: the first constant's x is `AlphaTransformMult / fadeScale` (x87, stored into the reserved argument slot), its
// w the offset re-pushed as a dword; the second constant is all immediates; the matrices pointer is passed with the
// +0x3030 register first, the offsets pointer with +0x303c second, the count re-read once into esi.
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#include "engine/CEngineInternalPrimitiveRepeatedStaticMesh.h"

struct C4DVector { float X, Y, Z, W; };
struct SViewRecord_01436e80 {
    unsigned char _pad[0x88];
    float         m_fadeDistanceScale;                   // +0x88
};
extern SViewRecord_01436e80* g_pViewRecord_01436e80;     // 0x01436E80
struct SEngineShaderRegisters_01436e98 {
    unsigned char _pad[0x3030];
    long          m_objectMatrixRegister;                // +0x3030
    unsigned char _pad3034[0x8];
    long          m_objectOffsetRegister;                // +0x303c
};
extern SEngineShaderRegisters_01436e98* g_pEngine_01436e98;   // 0x01436E98

class CShaderRenderManager {
public:
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00
    void SetVSConstants(long reg, const void* data, unsigned long count);                         // 0x0098B930
};
extern CShaderRenderManager* g_pShaderRenderManager;     // 0x01436E14

struct CEnginePrimitiveManagerRepeatedStaticMeshes_Methods : CEnginePrimitiveManagerRepeatedStaticMeshes {
    void UploadShaderConstants(CEngineInternalPrimitiveRepeatedStaticMesh& primitive, const C4DVector* offsets,
                               const C4DVector* matrices, long count);
};

void CEnginePrimitiveManagerRepeatedStaticMeshes_Methods::UploadShaderConstants(
    CEngineInternalPrimitiveRepeatedStaticMesh& primitive, const C4DVector* offsets, const C4DVector* matrices, long count)
{
    g_pShaderRenderManager->SetUserVertexShaderConstant(
        1, primitive.AlphaTransformMult / g_pViewRecord_01436e80->m_fadeDistanceScale, 0.0f, 0.0f,
        primitive.AlphaTransformOffset);
    g_pShaderRenderManager->SetUserVertexShaderConstant(2, 1.0f, -1.0f, 1.0f, 0.0f);
    g_pShaderRenderManager->SetVSConstants(g_pEngine_01436e98->m_objectMatrixRegister, matrices, count);
    g_pShaderRenderManager->SetVSConstants(g_pEngine_01436e98->m_objectOffsetRegister, offsets, count);
}