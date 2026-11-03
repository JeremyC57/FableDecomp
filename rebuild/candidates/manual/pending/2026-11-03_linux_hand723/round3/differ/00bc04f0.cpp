// CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- draw one
//   particle sprite: bind its bank frame's texture inside a render-state capture block, pick the blend / alpha-test states and the
//   vertex / pixel shaders for the pass, upload the billboard axes and the sprite constants, draw the 2-triangle quad, then restore
// Leaf: ?RenderSingleSprite@CEnginePrimitiveManagerParticleSpriteGroup@@QAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Flags: none
// Grade: DIFFER(3786v3751) -- 135 masked diff lines (q.py, FABLE_HDR_OVERLAY=hdr/, cl 13.10.3077): (1) VC7.1 here hoists the
//   alpha-ref constant 0x10 (pass 7) into ebx right after the first GetFrame, sharing it with the frame release's IsInitialised
//   test (`test byte [esi+0x1a],0x10`), so `this` is spilled instead of living in ebx (retail `mov ebx,ecx`): the pass-6/7 states
//   then load the current value into ebx instead of retail's `cmp [eax+off],esi` (~49 lines; with the constant spelled 0x11 the
//   hoist is gone and `this` lands in ebx with retail's frame slots -- experiment o1/x15, not a source form); (2) the documented
//   SetRenderState bookmark test/or operand-order lane gap (retail pass 7 and the default blend-op blocks use `test eax,esi`,
//   the ADDITIVE blend-mode blocks `test esi,eax`; ours the other way), which also moves the cross-jump merge of the pass-7
//   crossed-sprite cull block and the ADDITIVE_INV_SRC_COLOUR dest block; (3) scheduling: the camera-matrix base add, the zero
//   constant register at the tail (retail `xor edi,edi` before SetSelfIlluminationAmbient(0)), the texture pointer's Info/Data
//   zeroing order in the inlined frame-data delete.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BC04F0, length 3751 bytes (0xEA7), __thiscall, `ret 8`, frame 0x30.
// Identity: Ego_r.pdb and FableWin.pdb ?RenderSingleSprite@CEnginePrimitiveManagerParticleSpriteGroup@@QAEXAAVCEngineSubPrimitiveBase@@
//   W4ERenderPassType@@@Z; caller CEnginePrimitiveManagerSprites::RenderSubPrimitive 0x00BAD2E0 (the manager table slot +0x7C).
// Callees: CBankFile::EntryExists 0x009CC190 (QBE_NK), CGraphicInfo::GetFrame 0x009FF8D0 (QBE_NAAVCGraphicFrame@@KKK, twice),
//   IDirect3DDevice9 SetTexture (+0x104) / SetStreamSource (+0x190) / SetIndices (+0x1A0) through the inlined CRenderManagerCore
//   AttachTextureToStage / SelectVertexBuffer / SelectIndexBuffer, CShaderRenderManager::ApplyVertexShader 0x00988020 /
//   ApplyPixelShader 0x00988140 / SetUserVertexShaderConstants 0x009896D0 (KJPAM) / SetUserVertexShaderConstant 0x00989A60
//   (K, const C3DVector&, M) and 0x00989B00 (KMMMM), PrepareForRender(0x15) inline (SetVSConstants 0x0098B930, UpdatePixelShader
//   0x00988A20, UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals
//   0x0098A760, UpdateWorldSpaceLighting 0x0098A540, UpdateCombinedProjectionTransform 0x00988A50, UpdateCameraPos 0x00989700),
//   CEnginePrimitiveManagerParticleGroup::SetUpPointLights 0x00BBC0B0 / SetupPixelShader 0x00BBCC70, CEngineLightingManager::
//   GetLightsInSphereFast 0x00B46B90 / SetupShaderPointLights 0x00B44F20 / UpdateGlobalLightingStates 0x00B46C80,
//   CRenderManagerCore::DrawCurrentBuffersAsIndexedTris 0x00A0AC60, CRenderStateManager::RestoreCaptureBlock 0x00A05840,
//   CGraphicFrameData::Uninitialise 0x009FCBA0, operator delete 0x00BFE9BC.
// Inline in retail (both PDBs name them, no retail copy): CEnginePrimitiveRenderUtil::ApplyVertexShader / ApplyPixelShader /
//   GetCurrentCamera / FindLocalLights (desc by value: the temporary is built then copied, retail's double store) /
//   SetupLocalLights (SI) and SetupBlendOpState / SetupAlphaBlendingStatesFromBlendMode (QAE, the bool = premultiply), the six
//   sprite-group shader pickers (Apply{3DFaceMe,Billboard,CrossedSprite}{Displacement,PointLight}VertexShader), CEngineResourceManager::
//   PeekGraphicBank, CEngineCamera::PeekInverseNonHomogenousViewTransform, CEngineComponent::PeekRenderEnabled, CRGBFloatColour(
//   const CRGBColour&), the lighting manager's RestoreDefaultPrimitiveParameters / SetSelfIlluminationAmbient, GFFastSin/GFFastCos
//   (cosine table 0x013CD550, the shared GFFloatToLongNear helper), CGraphicFrame / CGraphicFrameData release.
// Globals: 0x0139B6E4 (a .data long initialised to 1, read only here: the sprite render switch; descriptive name),
//   g_pRenderManagerCore 0x01436E18, g_pShaderRenderManager 0x01436E14, g_pResourceManager 0x01436E98, g_pEngineCamera 0x01436EA0,
//   g_pEngineLightingManager 0x01436E9C, g_pEnginePrimitiveTypeInfo 0x01436E84 (manager 24 = CEnginePrimitiveManagerParticleGroup,
//   its GetSupportedTypes 0x00BBCF30 pushes type 0x18), g_pDisplacementRenderer_01436e38 0x01436E38 (RTTI of vtable 0x012A3A94:
//   CEngineScreenEffectDisplacementRenderer : CEngineComponent); render-state tables CRenderStateManager::D3DBlendOpTable 0x01396ED0,
//   D3DAlphaBlendModeTable 0x01396F68, D3DAlphaTestFuncTable 0x01396DD8, D3DPolyCullModeTable 0x01396FB0; floats 1/255 0x01231724,
//   1/32767 0x0129C81C (GetFloatTLX/TLY), 1024 0x01230010, 0.5 0x0122F59C, 1.0 0x0122DED8, 0.0 0x0122DEDC.
// Shape: pass 6 (displacement, only while the displacement renderer is enabled) and pass 7 (alpha-tested displacement) each set
//   their states and pick a displacement vertex shader by the sprite flags (Sprite3D, RotateAroundZAxis + cull NONE, billboard);
//   every other pass sets the blend op / blend mode from the sprite, lights it (particle: the group's point lights; else the
//   5 nearest lights in its bounding sphere; unlit: 100% self-illumination) and picks the point-light shader for min(lights,
//   size-1). The common tail binds the buffers, writes ConstantBlock[0..2] (camera right / up axes, offset factors, the frame's
//   width / height factors, RotateAroundCentre) and, for a visible sprite whose frame loads, constants 4..7 (position, cos/sin of
//   the angle and the size, the colour premultiplied when the blend mode asks for it, the frame's texture-page origin).
// Levers found: colour + a C3DVector of the premultiplied RGB as separate draw-block locals (retail's 0x30 frame: sphere and RGB share
//   a slot, the colour does not); `index = lights; if (lights > size()-1) index = size()-1;` for the shader clamp; FindLocalLights'
//   by-value desc for the channel copy; PeekGraphicBank for the bank / sprite load order; the lighting setters must be
//   __forceinline (plain inline is called out of line here).
// Layout debt: CGraphicFrameData / CGraphicFrame / CGraphicDataBank / CGraphicInfo are the round-2 local models (no engine headers
//   for them; CGraphicFrame.h exists but its retail branch has no ctor / dtor / GetTexture yet); CEnginePrimitiveManagerParticleGroup
//   is declared here (no header). ERenderPassType's two enumerators are descriptive (the PDBs keep only the tag).
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERPARTICLESPRITEGROUP
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINECAMERA
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_CBANKFILE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#include "engine/CEnginePrimitiveManagerParticleSpriteGroup.h"
#include "engine/CRenderManagerCore.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEngineSubPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveSprite.h"
#include "engine/CEngineLocalLightArray.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CBankFile.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/CCountedPointer.h"

void __cdecl operator delete(void* block);             // 0x00BFE9BC

class CGraphicFrameData { public:                             // Ego_r.pdb, 27 bytes
    unsigned long   ReferenceCount;                    // +0x00
    void*           Data;                              // +0x04
    CCountedPointer<CTexture> PTexture;                // +0x08
    unsigned short  TLX, TLY, Width, Height, Pitch;    // +0x10
    unsigned char   Alpha : 1;                         // +0x1a
    unsigned char   BooleanAlpha : 1;
    unsigned char   UsesSurface : 1;
    unsigned char   SectionOfTPage : 1;
    unsigned char   IsInitialised : 1;
    unsigned char   ScreenGraphic : 1;
    void Uninitialise();                               // 0x009FCBA0
    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    __forceinline void Release()
    {
        if (--ReferenceCount == 0)
            delete this;
    }
    __forceinline CTexture* GetTexture() const { return PTexture.Data; }   // ?GetTexture@CGraphicFrameData@@QBEPAVCTexture@@XZ
    __forceinline float GetFloatTLX() const { return TLX * (1.0f / 32767.0f); }   // ?GetFloatTLX@CGraphicFrameData@@QBEMXZ
    __forceinline float GetFloatTLY() const { return TLY * (1.0f / 32767.0f); }   // ?GetFloatTLY@CGraphicFrameData@@QBEMXZ
};
class CGraphicFrame { public:                                 // intrusive pointer to CGraphicFrameData
    CGraphicFrameData* FrameData;
    __forceinline CGraphicFrame() : FrameData(0) { }
    __forceinline ~CGraphicFrame() { ReleaseData(); }
private:
    __forceinline void ReleaseData()
    {
        if (FrameData) {
            FrameData->Release();
            FrameData = 0;
        }
    }
public:
    __forceinline CTexture* GetTexture() const { return FrameData->GetTexture(); }
    __forceinline float GetFloatTLX() const { return FrameData->GetFloatTLX(); }
    __forceinline float GetFloatTLY() const { return FrameData->GetFloatTLY(); }
};
class CGraphicInfo;
class CGraphicDataBank : public CBankFile { public:
    unsigned char _pad110[0x1e0 - 0x110];
    CArray<CGraphicInfo> GraphicList;                  // +0x1e0 (0x2c-byte records)
    __forceinline const CGraphicInfo* GetGraphicInfo(unsigned long index) const;
};
class CGraphicDataBankHeader { public:                 // CGraphicDataBank::CGraphicHeader
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount;
    unsigned char  _pad0c[0x1c - 0x0c];
};
class CGraphicInfo { public:
    CGraphicDataBankHeader Header;                     // +0x00
    unsigned long      TextureType;                    // +0x1c
    CGraphicDataBank*  Bank;                           // +0x20
    long               Index;                          // +0x24
    void*              Frames;                         // +0x28
    bool GetFrame(CGraphicFrame& frame, unsigned long frameNum, unsigned long mipmapLevel, unsigned long flags) const;   // 0x009FF8D0
    __forceinline float GetFrameWidthFactor() const { return (float)Header.FrameWidth / Header.Width; }
    __forceinline float GetFrameHeightFactor() const { return (float)Header.FrameHeight / Header.Height; }
};
__forceinline const CGraphicInfo* CGraphicDataBank::GetGraphicInfo(unsigned long index) const
{
    return index < GraphicList.size() ? &GraphicList[index] : 0;
}

class CEnginePrimitiveManagerParticleGroup : public CEnginePrimitiveManager { public:
    long SetUpPointLights();                                                                          // 0x00BBC0B0
    void SetupPixelShader(unsigned long constantIndex, const CPixelShader& stipple, const CPixelShader& normal);  // 0x00BBCC70
};

enum ERenderPassType { RENDER_PASS_DISPLACEMENT = 6, RENDER_PASS_DISPLACEMENT_ALPHA_TEST = 7 };

extern long g_spriteRenderEnabled_0139b6e4;                                    // 0x0139B6E4 (descriptive)
extern CRenderManagerCore* g_pRenderManagerCore;                                // 0x01436E18
extern CShaderRenderManager* g_pShaderRenderManager;                            // 0x01436E14
extern CEngineResourceManager* g_pResourceManager;                              // 0x01436E98
extern CEngineCamera* g_pEngineCamera;                                          // 0x01436EA0
extern CEngineLightingManager* g_pEngineLightingManager;                        // 0x01436E9C
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;                    // 0x01436E84
extern CEngineScreenEffectDisplacementRenderer* g_pDisplacementRenderer_01436e38;  // 0x01436E38
extern float g_CosTable[1025];                                                  // 0x013CD550

__forceinline float __fastcall GFFastCos(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i &= 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}
__forceinline float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i = (i - 256) & 0x3ff;
    return (1.0f - frac) * g_CosTable[i] + frac * g_CosTable[i + 1];
}

__forceinline void __fastcall CEnginePrimitiveRenderUtil::ApplyVertexShader(const CVertexShader& shader)
{
    g_pShaderRenderManager->ApplyVertexShader(shader, false);
}
__forceinline void __fastcall CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& shader)
{
    g_pShaderRenderManager->ApplyPixelShader(shader);
}
__forceinline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera()
{
    return *g_pEngineCamera;
}
__forceinline CEngineLocalLightArray* __fastcall CEnginePrimitiveRenderUtil::FindLocalLights(const C3DVector& position, float radius,
                                                                                          CEnginePrimitiveThemeChannelDesc channel)
{
    return g_pEngineLightingManager->GetLightsInSphereFast(C3DBoundingSphere(position, radius), 5, channel);
}
__forceinline void __fastcall CEnginePrimitiveRenderUtil::SetupLocalLights(const CEngineLocalLightArray& lights)
{
    g_pEngineLightingManager->SetupShaderPointLights(lights);
}
__forceinline void CEnginePrimitiveRenderUtil::SetupBlendOpState(EEnginePrimitiveBlendOp op)
{
    switch (op) {
    case ENGINE_PRIMITIVE_BLEND_OP_SUBTRACT:
        g_pRenderManagerCore->SetBlendOp(BLEND_OP_SUBTRACT);
        break;
    case ENGINE_PRIMITIVE_BLEND_OP_REV_SUBTRACT:
        g_pRenderManagerCore->SetBlendOp(BLEND_OP_REV_SUBTRACT);
        break;
    default:
        g_pRenderManagerCore->SetBlendOp(BLEND_OP_ADD);
        break;
    }
}
__forceinline bool CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode)
{
    switch (mode) {
    case ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE:
        g_pRenderManagerCore->SetSourceAlphaBlendMode(ALPHA_BLEND_MODE_ONE);
        g_pRenderManagerCore->SetDestAlphaBlendMode(ALPHA_BLEND_MODE_ONE);
        return true;
    case ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE_INV_SRC_COLOUR:
        g_pRenderManagerCore->SetSourceAlphaBlendMode(ALPHA_BLEND_MODE_ONE);
        g_pRenderManagerCore->SetDestAlphaBlendMode(ALPHA_BLEND_MODE_INV_SRC_COLOUR);
        return true;
    default:
        g_pRenderManagerCore->SetSourceAlphaBlendMode(ALPHA_BLEND_MODE_SRC_ALPHA);
        g_pRenderManagerCore->SetDestAlphaBlendMode(ALPHA_BLEND_MODE_INV_SRC_ALPHA);
        return false;
    }
}

__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::Apply3DFaceMeDisplacementVertexShader()
{
    ApplyVertexShader(VertexShaders3DFaceMeSpriteDisplacement);
}
__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::ApplyCrossedSpriteDisplacementVertexShader()
{
    g_pRenderManagerCore->SetPolyCullMode(POLY_CULL_MODE_NONE);
    ApplyVertexShader(VertexShadersCrossedSpriteDisplacement);
}
__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::ApplyBillboardDisplacementVertexShader()
{
    ApplyVertexShader(VertexShadersBillboardSpriteDisplacement);
}
__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::Apply3DFaceMePointLightVertexShader(unsigned long lights)
{
    unsigned long index = lights;
    if (lights > VertexShaders3DFaceMeSpritePointLights.size() - 1)
        index = VertexShaders3DFaceMeSpritePointLights.size() - 1;
    ApplyVertexShader(VertexShaders3DFaceMeSpritePointLights[index]);
}
__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::ApplyCrossedSpritePointLightVertexShader(unsigned long lights)
{
    g_pRenderManagerCore->SetPolyCullMode(POLY_CULL_MODE_NONE);
    unsigned long index = lights;
    if (lights > VertexShadersCrossedSpritePointLights.size() - 1)
        index = VertexShadersCrossedSpritePointLights.size() - 1;
    ApplyVertexShader(VertexShadersCrossedSpritePointLights[index]);
}
__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::ApplyBillboardPointLightVertexShader(unsigned long lights)
{
    unsigned long index = lights;
    if (lights > VertexShadersBillboardSpritePointLights.size() - 1)
        index = VertexShadersBillboardSpritePointLights.size() - 1;
    ApplyVertexShader(VertexShadersBillboardSpritePointLights[index]);
}

void CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    if (!g_spriteRenderEnabled_0139b6e4)
        return;
    CEngineInternalPrimitiveSprite& sprite = static_cast<CEngineInternalPrimitiveSprite&>(*sub.Parent);
    if (!g_pResourceManager->PeekGraphicBank(static_cast<EGraphicBankNo>(0))->EntryExists(sprite.BankIndex))
        return;
    const CGraphicInfo* info = g_pResourceManager->PeekGraphicBank(static_cast<EGraphicBankNo>(0))->GetGraphicInfo(sprite.BankIndex);
    CGraphicFrame frame;
    if (info->GetFrame(frame, 0, 0, 0)) {
    CRenderStateCaptureBlock captureBlock;
    g_pRenderManagerCore->AttachTextureToStage(0, frame.GetTexture());
    bool premultiplyAlpha;
    if (pass == RENDER_PASS_DISPLACEMENT && g_pDisplacementRenderer_01436e38->PeekRenderEnabled()) {
        premultiplyAlpha = false;
        g_pRenderManagerCore->SetBlendOp(BLEND_OP_ADD);
        g_pRenderManagerCore->SetSourceAlphaBlendMode(ALPHA_BLEND_MODE_SRC_ALPHA);
        g_pRenderManagerCore->SetDestAlphaBlendMode(ALPHA_BLEND_MODE_INV_SRC_ALPHA);
        if (sprite.Sprite3D)
            Apply3DFaceMeDisplacementVertexShader();
        else if (sprite.RotateAroundZAxis)
            ApplyCrossedSpriteDisplacementVertexShader();
        else
            ApplyBillboardDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
    } else if (pass == RENDER_PASS_DISPLACEMENT_ALPHA_TEST) {
        premultiplyAlpha = false;
        g_pRenderManagerCore->SetAlphaTestEnable(true);
        g_pRenderManagerCore->SetAlphaRef(0x10);
        g_pRenderManagerCore->SetAlphaTestFunc(ALPHA_TEST_FUNC_GREATER);
        if (sprite.Sprite3D)
            Apply3DFaceMeDisplacementVertexShader();
        else if (sprite.RotateAroundZAxis)
            ApplyCrossedSpriteDisplacementVertexShader();
        else
            ApplyBillboardDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
    } else {
        SetupBlendOpState(sprite.BlendOp);
        premultiplyAlpha = SetupAlphaBlendingStatesFromBlendMode(sprite.BlendMode);
        CEnginePrimitiveManagerParticleGroup* particleGroupManager =
            static_cast<CEnginePrimitiveManagerParticleGroup*>(g_pEnginePrimitiveTypeInfo->PrimitiveManagers[24]);
        unsigned long lights = 0;
        if (sprite.Lit) {
            if (sprite.IsParticle)
                lights = particleGroupManager->SetUpPointLights();
            else {
                CEngineLocalLightArray* localLights =
                    FindLocalLights(sprite.WorldPos, sprite.BSphereRadius, CEnginePrimitiveThemeChannelDesc(0, 0, 0));
                lights = localLights->LightCount;
                SetupLocalLights(*localLights);
            }
        } else
            g_pEngineLightingManager->SetSelfIlluminationAmbient(100);
        g_pEngineLightingManager->UpdateGlobalLightingStates();
        if (sprite.Sprite3D)
            Apply3DFaceMePointLightVertexShader(lights);
        else if (sprite.RotateAroundZAxis)
            ApplyCrossedSpritePointLightVertexShader(lights);
        else
            ApplyBillboardPointLightVertexShader(lights);
        if (sprite.IsParticle)
            particleGroupManager->SetupPixelShader(0, PixelShaderStipple, PixelShader);
        else
            ApplyPixelShader(PixelShader);
    }
    g_pRenderManagerCore->SelectVertexBuffer(0, &VertexBuffer);
    g_pRenderManagerCore->SelectIndexBuffer(&IndexBuffer, 0);
    const CMatrix3x4& view = GetCurrentCamera().PeekInverseNonHomogenousViewTransform();
    ConstantBlock[0].X = view.E11;
    ConstantBlock[0].Y = view.E21;
    ConstantBlock[0].Z = sprite.OffsetFactorX;
    ConstantBlock[0].W = sprite.OffsetFactorY;
    ConstantBlock[1].X = view.E12;
    ConstantBlock[1].Y = view.E22;
    ConstantBlock[1].Z = info->GetFrameWidthFactor();
    ConstantBlock[1].W = info->GetFrameHeightFactor();
    ConstantBlock[2].X = view.E13;
    ConstantBlock[2].Y = view.E23;
    ConstantBlock[2].Z = sprite.RotateAroundCentre ? 1.0f : 0.0f;
    ConstantBlock[2].W = 0.0f;
    g_pShaderRenderManager->SetUserVertexShaderConstants(1, 3, &ConstantBlock[0].X);
    if (sprite.Colour.A > 0 && sprite.SizeX > 0.0f && sprite.SizeY > 0.0f && info->GetFrame(frame, sprite.Frame, 0, 0)) {
        CRGBFloatColour colour(sprite.Colour);
        float alphaScale = premultiplyAlpha ? colour.A : 1.0f;
        C3DVector premultiplied(colour.R * alphaScale, colour.G * alphaScale, colour.B * alphaScale);
        g_pShaderRenderManager->SetUserVertexShaderConstant(4, sprite.WorldPos, 0.0f);
        g_pShaderRenderManager->SetUserVertexShaderConstant(5, GFFastCos(sprite.Angle), GFFastSin(sprite.Angle), sprite.SizeX, sprite.SizeY);
        g_pShaderRenderManager->SetUserVertexShaderConstant(6, premultiplied.X, premultiplied.Y, premultiplied.Z, colour.A);
        g_pShaderRenderManager->SetUserVertexShaderConstant(7, frame.GetFloatTLX(), frame.GetFloatTLY(), 0.0f, 0.0f);
        g_pShaderRenderManager->PrepareForRender(0x15);
        g_pRenderManagerCore->DrawCurrentBuffersAsIndexedTris(0, 6, 0, 2);
    }
    g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
    }
}
