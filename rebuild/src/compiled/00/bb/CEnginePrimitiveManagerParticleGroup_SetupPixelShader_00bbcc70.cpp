// CEnginePrimitiveManagerParticleGroup::SetupPixelShader(unsigned long reg, const CPixelShader& stipple_shader, const CPixelShader& normal_shader) -- choose the particle group's pixel shader: a z-writing group draws with the stipple-fade shader (screen-door alpha), any other group with the normal one
// Leaf: ?SetupPixelShader@CEnginePrimitiveManagerParticleGroup@@QAEXKABVCPixelShader@@0@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBCC70, length 415 bytes, thiscall, `ret 0Ch`; public non-virtual (not in a vtable).
//   Identity: Ego_r.pdb and FableWin.pdb ?SetupPixelShader@CEnginePrimitiveManagerParticleGroup@@QAEXKABVCPixelShader@@0@Z.
//   Callers 0x00BBE6C2 / 0x00BBFE49 / 0x00BC0E6B / 0x00BC2C77 / 0x00BC4608 pass reg 0, the group's stipple shader, then its normal shader.
// Purpose: if the group writes the z-buffer (WriteZBuffer, this+0x22) it cannot alpha-blend, so it fades by stippling: load user
//   vertex-shader constant `reg` with (1/32, 1/32, 320, 240) = stipple-texture UV scale and the half screen size, set texture
//   stage 1 to wrap in U and V, attach the resource manager's StipleFadeTexture to stage 1 and apply the stipple pixel shader.
//   Otherwise zero the constant and apply the normal pixel shader.
// Callees: CShaderRenderManager::SetUserVertexShaderConstant 0x00989B00, CShaderRenderManager::ApplyPixelShader 0x00988140,
//   IDirect3DDevice9::SetTexture (vtable +0x104) through the inline CRenderManagerCore::AttachTextureToStage (its out-of-line
//   COMDAT copy is 0x009A0CF0; retail expands it here). Inline: CRenderStateManager::SetTextureUVModeU / SetTextureUVModeV
//   (-> private SetRenderState -> AddStateToUpdateList; stage-1 states at core +0x2EF8 / +0x2F98, D3D value from
//   CRenderStateManager::D3DTextureUVModeTable[TEXTURE_UV_WRAP] 0x01396DC8), CEngineResourceManager::GetStipleFadeTexture (+0x2AAC).
// Globals: NEngineInternal::GPShaderRenderManager 0x01436E14, GPRenderManager 0x01436E18, GPResourceManager 0x01436E98.
// Shape: `this` is read once (the flag); esi/edi are pushed only on the stipple path. The two-level member inlines give retail's
//   block 2 its core-pointer copy (`mov ecx,eax`) and scoped ebp temp; the texture's address is null-tested inside
//   AttachTextureToStage (`texture ? texture->PD3DTexture : 0`), which is the `add ecx,2AACh ; je`.
// Headers: needs the overlay opt-in FABLE_RETAIL_CENGINERESOURCEMANAGER_GETSTIPLEFADETEXTURE (engine/CEngineResourceManager.h).
// Layout: CEnginePrimitiveManagerParticleGroup has no shared header; local packed model as in the landed sibling 0x00BBC460
//   (Ego_r.pdb members +0x0C..+0x24, CurrentWorldPos at +0x0E).
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETSTIPLEFADETEXTURE
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/C3DVector.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98

#pragma pack(push, 1)
class CEnginePrimitiveManagerParticleGroup : public CEnginePrimitiveManager {
public:
    void SetupPixelShader(unsigned long reg, const CPixelShader& stipple_shader, const CPixelShader& normal_shader);
protected:
    bool PointLightsSet;                            // +0x0c
    signed char PointLightCount;                    // +0x0d
    C3DVector CurrentWorldPos;                      // +0x0e
    unsigned char _pad1a[2];
    float CurrentBSRadius;                          // +0x1c
    bool ClipEffectToWeatherMask;                   // +0x20
    bool ReadZBuffer;                               // +0x21
    bool WriteZBuffer;                              // +0x22
    bool DisplayBoundingBox;                        // +0x23
    bool DisplayBoundingSphere;                     // +0x24
};
#pragma pack(pop)

void CEnginePrimitiveManagerParticleGroup::SetupPixelShader(unsigned long reg, const CPixelShader& stipple_shader, const CPixelShader& normal_shader)
{
    if (WriteZBuffer) {
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(reg, 1.0f / 32.0f, 1.0f / 32.0f, 320.0f, 240.0f);
        NEngineInternal::GPRenderManager->SetTextureUVModeU(1, TEXTURE_UV_WRAP);
        NEngineInternal::GPRenderManager->SetTextureUVModeV(1, TEXTURE_UV_WRAP);
        NEngineInternal::GPRenderManager->AttachTextureToStage(1, &NEngineInternal::GPResourceManager->GetStipleFadeTexture());
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(stipple_shader);
    } else {
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(reg, 0.0f, 0.0f, 0.0f, 0.0f);
        NEngineInternal::GPShaderRenderManager->ApplyPixelShader(normal_shader);
    }
}