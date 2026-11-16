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

class C4DVector { public: float X, Y, Z, W; };
struct SViewRecord_01436e80 {
    unsigned char _pad[0x88];
    float         m_fadeDistanceScale;                   // +0x88
};
class CEnginePrimitiveRenderer;
namespace NEngineInternal { extern CEnginePrimitiveRenderer* GPPrimitiveRenderer; }   // 0x01436E80 (the engine's primitive-renderer component; Codex name fix 2026-11-10)
// The members read here are modelled by the local SViewRecord_01436e80 view of that object (name debt: the shared
// engine/CEnginePrimitiveRenderer.h retail branch).
// NF46 (round 7): the 0x01436E98 global is NEngineInternal::GPResourceManager (Ego_r.pdb / FableWin.pdb
// ?GPResourceManager@NEngineInternal@@3PAVCEngineResourceManager@@A); the two register indices are its VSConstantLayoutRepeatedMesh (+0x2EC8,
// FableWin.pdb inline PeekVSConstantLayoutRepeatedMesh) ObjectMatricies.Offset (+0x164 + 4 = +0x3030) and ObjectOffsets.Offset
// (+0x170 + 4 = +0x303C) (Ego_r.pdb CEngineVSConstantLayoutRepeatedMesh / CShaderConstantRange members; engine/CEngineResourceManager.h
// _PEEKVSCONSTANTLAYOUTREPEATEDMESH, engine/CEngineVSConstantLayoutRepeatedMesh.h); was a SEngineShaderRegisters_01436e98 / g_pEngine_01436e98 stand-in.
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKVSCONSTANTLAYOUTREPEATEDMESH
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#define FABLE_RETAIL_CENGINEVSCONSTANTLAYOUTREPEATEDMESH
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineVSConstantLayoutRepeatedMesh.h"
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }   // 0x01436E98

class CShaderRenderManager {
public:
    void SetUserVertexShaderConstant(unsigned long index, float x, float y, float z, float w);   // 0x00989B00
    void SetVSConstants(long reg, const void* data, unsigned long count);                         // 0x0098B930
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }     // 0x01436E14

struct CEnginePrimitiveManagerRepeatedStaticMeshes_Methods : CEnginePrimitiveManagerRepeatedStaticMeshes {
    void UploadShaderConstants(CEngineInternalPrimitiveRepeatedStaticMesh& primitive, const C4DVector* offsets,
                               const C4DVector* matrices, long count);
};

void CEnginePrimitiveManagerRepeatedStaticMeshes_Methods::UploadShaderConstants(
    CEngineInternalPrimitiveRepeatedStaticMesh& primitive, const C4DVector* offsets, const C4DVector* matrices, long count)
{
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(
        1, primitive.AlphaTransformMult / ((SViewRecord_01436e80*)NEngineInternal::GPPrimitiveRenderer)->m_fadeDistanceScale, 0.0f, 0.0f,
        primitive.AlphaTransformOffset);
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(2, 1.0f, -1.0f, 1.0f, 0.0f);
    NEngineInternal::GPShaderRenderManager->SetVSConstants(NEngineInternal::GPResourceManager->PeekVSConstantLayoutRepeatedMesh().ObjectMatricies.Offset, matrices, count);
    NEngineInternal::GPShaderRenderManager->SetVSConstants(NEngineInternal::GPResourceManager->PeekVSConstantLayoutRepeatedMesh().ObjectOffsets.Offset, offsets, count);
}