// CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite -- DRAFT
// Leaf: ?RenderSingleSprite@CEnginePrimitiveManagerParticleSpriteGroup@@QAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
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

void __cdecl operator delete(void* block);             // 0x00BFE9BC

struct CCPInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CTexturePointer {                               // CCountedPointer<CTexture>
    CTexture* Data;
    CCPInfo* Info;
    __forceinline ~CTexturePointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CGraphicFrameData { public:                             // Ego_r.pdb, 27 bytes
    unsigned long   ReferenceCount;                    // +0x00
    void*           Data;                              // +0x04
    CTexturePointer PTexture;                          // +0x08
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
    void SetupPixelShader(unsigned long a0, const CPixelShader& stipple, const CPixelShader& normal);  // 0x00BBCC70
};

enum ERenderPassType { RENDER_PASS_DISPLACEMENT = 6, RENDER_PASS_DISPLACEMENT_ALPHA_TEST = 7 };

extern long g_spriteRenderEnabled_0139b6e4;                                    // 0x0139B6E4
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

__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::Apply3DFaceMeDisplacementVertexShader()
{
    g_pShaderRenderManager->ApplyVertexShader(VertexShaders3DFaceMeSpriteDisplacement, false);
}
__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::ApplyCrossedSpriteDisplacementVertexShader()
{
    g_pRenderManagerCore->SetPolyCullMode(POLY_CULL_MODE_NONE);
    g_pShaderRenderManager->ApplyVertexShader(VertexShadersCrossedSpriteDisplacement, false);
}
__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::ApplyBillboardDisplacementVertexShader()
{
    g_pShaderRenderManager->ApplyVertexShader(VertexShadersBillboardSpriteDisplacement, false);
}
__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::Apply3DFaceMePointLightVertexShader(unsigned long lights)
{
    unsigned long index = lights;
    if (lights > VertexShaders3DFaceMeSpritePointLights.size() - 1)
        index = VertexShaders3DFaceMeSpritePointLights.size() - 1;
    g_pShaderRenderManager->ApplyVertexShader(VertexShaders3DFaceMeSpritePointLights[index], false);
}
__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::ApplyCrossedSpritePointLightVertexShader(unsigned long lights)
{
    g_pRenderManagerCore->SetPolyCullMode(POLY_CULL_MODE_NONE);
    unsigned long index = lights;
    if (lights > VertexShadersCrossedSpritePointLights.size() - 1)
        index = VertexShadersCrossedSpritePointLights.size() - 1;
    g_pShaderRenderManager->ApplyVertexShader(VertexShadersCrossedSpritePointLights[index], false);
}
__forceinline void CEnginePrimitiveManagerParticleSpriteGroup::ApplyBillboardPointLightVertexShader(unsigned long lights)
{
    unsigned long index = lights;
    if (lights > VertexShadersBillboardSpritePointLights.size() - 1)
        index = VertexShadersBillboardSpritePointLights.size() - 1;
    g_pShaderRenderManager->ApplyVertexShader(VertexShadersBillboardSpritePointLights[index], false);
}

void CEnginePrimitiveManagerParticleSpriteGroup::RenderSingleSprite(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    if (!g_spriteRenderEnabled_0139b6e4)
        return;
    CEngineInternalPrimitiveSprite& sprite = static_cast<CEngineInternalPrimitiveSprite&>(*sub.Parent);
    if (!g_pResourceManager->GraphicBanks[0]->EntryExists(sprite.BankIndex))
        return;
    const CGraphicInfo* info = g_pResourceManager->GraphicBanks[0]->GetGraphicInfo(sprite.BankIndex);
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
        g_pShaderRenderManager->ApplyPixelShader(PixelShaderDisplacement);
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
        g_pShaderRenderManager->ApplyPixelShader(PixelShaderDisplacement);
    } else {
        switch (sprite.BlendOp) {
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
        switch (sprite.BlendMode) {
        case ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE:
            g_pRenderManagerCore->SetSourceAlphaBlendMode(ALPHA_BLEND_MODE_ONE);
            g_pRenderManagerCore->SetDestAlphaBlendMode(ALPHA_BLEND_MODE_ONE);
            premultiplyAlpha = true;
            break;
        case ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE_INV_SRC_COLOUR:
            g_pRenderManagerCore->SetSourceAlphaBlendMode(ALPHA_BLEND_MODE_ONE);
            g_pRenderManagerCore->SetDestAlphaBlendMode(ALPHA_BLEND_MODE_INV_SRC_COLOUR);
            premultiplyAlpha = true;
            break;
        default:
            g_pRenderManagerCore->SetSourceAlphaBlendMode(ALPHA_BLEND_MODE_SRC_ALPHA);
            g_pRenderManagerCore->SetDestAlphaBlendMode(ALPHA_BLEND_MODE_INV_SRC_ALPHA);
            premultiplyAlpha = false;
            break;
        }
        CEnginePrimitiveManagerParticleGroup* particleGroupManager =
            static_cast<CEnginePrimitiveManagerParticleGroup*>(g_pEnginePrimitiveTypeInfo->PrimitiveManagers[24]);
        unsigned long lights = 0;
        if (sprite.Lit) {
            if (sprite.IsParticle)
                lights = particleGroupManager->SetUpPointLights();
            else {
                CEnginePrimitiveThemeChannelDesc channel(0, 0, 0);
                CEngineLocalLightArray* localLights = g_pEngineLightingManager->GetLightsInSphereFast(
                    C3DBoundingSphere(sprite.WorldPos, sprite.BSphereRadius), 5, channel);
                lights = localLights->LightCount;
                g_pEngineLightingManager->SetupShaderPointLights(*localLights);
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
            g_pShaderRenderManager->ApplyPixelShader(PixelShader);
    }
    g_pRenderManagerCore->SelectVertexBuffer(0, &VertexBuffer);
    g_pRenderManagerCore->SelectIndexBuffer(&IndexBuffer, 0);
    const CMatrix3x4& view = g_pEngineCamera->PeekInverseNonHomogenousViewTransform();
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
