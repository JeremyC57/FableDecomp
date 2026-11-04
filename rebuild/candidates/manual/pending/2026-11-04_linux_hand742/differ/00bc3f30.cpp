// CEnginePrimitiveManagerParticleLineGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass) -- draw a particle line group
// Grade: DIFFER(3272v3357) -- the bookmark test/or operand-order lane gap: in retail the two additive blend-mode cases differ in operand order, so VC7.1 cross-jumps their dest-blend blocks only after the `or` (0x00BC44AB jmp 0x00BC453E); ours merges them at the compare (-85 bytes), with the register knock-on (lights in edi, &ConstantBlock in edi not ebp, camera matrix base add scheduled later); 242 masked lines. Next idea: none in source until the lane gap is solved.
// Leaf: ?RenderSubPrimitive@CEnginePrimitiveManagerParticleLineGroup@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC3F30, 3357 bytes, thiscall, `ret 8`; vtable slot 5 of
//   CEnginePrimitiveManagerParticleLineGroup (vtable 0x012A6590). The catalog row 0x00BC3F3D is bogus: it is the `push esi` inside
//   this prologue (mov eax,[0x0139B864]; sub esp,0x1C; push ebx; xor ebx,ebx; cmp eax,ebx; push esi @0x00BC3F3D).
// Purpose: when the line switch 0x0139B864 is on, bind the group's bank graphic frame 0, cull none, pick blend + shaders per pass
//   (as the sprite-trail row), select the line vertex / index buffers, upload the camera's non-homogenous view matrix and its
//   inverse as columns plus the frame UV rectangle (VS constants 1..7), then batch the lines: each line is three constants
//   (start + width, end + 1, colour with doubled alpha, RGB premultiplied when the blend mode asks) and every 10 lines are drawn as
//   40 vertices / 20 triangles; the tail batch draws count/3 lines; finally cull CCW and restore the lighting defaults.
// Callees: CGraphicInfo::GetFrame 0x009FF8D0, SetUpPointLights 0x00BBC0B0, SetupPixelShader 0x00BBCC70, UpdateGlobalLightingStates
//   0x00B46C80, CShaderRenderManager::ApplyVertexShader 0x00988020 / ApplyPixelShader 0x00988140 / SetUserVertexShaderConstants
//   0x009896D0 and the PrepareForRender updaters, CRenderManagerCore::DrawCurrentBuffersAsIndexedTris 0x00A0AC60.
// Layout: retail line group manager = Ego_r minus 4 (CArray 0xC): VertexShadersLinePointLights +0x0C, VertexShaderDisplacement
//   +0x18, PixelShader +0x20, PixelShaderStipple +0x28, PixelShaderDisplacement +0x30, VertexBuffer +0x38, IndexBuffer +0x6C,
//   ConstantBlock[30] +0x80; line group BankIndex +0xAC, Flags +0xB0, BlendMode +0xB4, BlendOp +0xB8; line Start +0x48, End
//   +0x54, Width +0x60, Colour +0x64. Levers found: PeekGraphicBank (prologue), a reference per constant block in the loop.
// Header debt: local models for the camera, the line primitives, the particle-group managers and the CGraphic* classes.
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINESCREENEFFECTDISPLACEMENTRENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRGBFLOATCOLOUR
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CEngineScreenEffectDisplacementRenderer.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEnginePrimitiveTypeInfo.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CCountedPointer.h"
#include "engine/CVertexShader.h"
#include "engine/CPixelShader.h"
#include "engine/CArray.h"
#define CGraphicFrame CGraphicFrame_Layout
#include "engine/CGraphicFrame.h"
#undef CGraphicFrame
#define CEnginePrimitiveManagerParticleLineGroup CEnginePrimitiveManagerParticleLineGroup_Layout
#include "engine/CEnginePrimitiveManagerParticleLineGroup.h"
#undef CEnginePrimitiveManagerParticleLineGroup
#include "engine/CVertexBufferWin32.h"
#include "engine/CIndexBuffer.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CMatrix3x4.h"
#define CEngineCamera CEngineCamera_Layout
#include "engine/CEngineCamera.h"
#undef CEngineCamera

void __cdecl operator delete(void* block);                                         // 0x00BFE9BC
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }              // 0x01436E18
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }  // 0x01436E14
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }    // 0x01436E98
extern CEngineLightingManager* g_pEngineLightingManager;                           // 0x01436E9C
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;                       // 0x01436E84
extern CEngineScreenEffectDisplacementRenderer* g_pDisplacementRenderer_01436e38;  // 0x01436E38
class CEngineCamera;
extern CEngineCamera* g_pEngineCamera;                                             // 0x01436EA0
extern long g_particleLineRenderEnabled_0139b864;                                  // 0x0139B864 (descriptive)

enum EEnginePrimitiveBlendOp { ENGINE_PRIMITIVE_BLEND_OP_ADD, ENGINE_PRIMITIVE_BLEND_OP_SUBTRACT, ENGINE_PRIMITIVE_BLEND_OP_REV_SUBTRACT };
enum EEnginePrimitiveBlendMode { ENGINE_PRIMITIVE_BLEND_MODE_0, ENGINE_PRIMITIVE_BLEND_MODE_1, ENGINE_PRIMITIVE_BLEND_MODE_2,
                                 ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE, ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE_INV_SRC_COLOUR };

class CEngineSubPrimitiveBase : public CBaseClassNonCopyable {
public:
    unsigned long                 RenderFlags;      // +0x04
    unsigned char                 Alpha;            // +0x08
    CEngineInternalPrimitiveBase* Parent;           // +0x0c
    CEngineSubPrimitiveBase*      NextSubPrimitive; // +0x10
    CEnginePrimitiveManager*      Manager;          // +0x14
};
class CTexture;
class CGraphicFrameData { public:                   // Ego_r.pdb, 27 bytes
    unsigned long   ReferenceCount;                 // +0x00
    void*           Data;                           // +0x04
    CCountedPointer<CTexture> PTexture;             // +0x08
    unsigned short  TLX, TLY, Width, Height, Pitch; // +0x10
    unsigned char   Alpha : 1;                      // +0x1a
    unsigned char   BooleanAlpha : 1;
    unsigned char   UsesSurface : 1;
    unsigned char   SectionOfTPage : 1;
    unsigned char   IsInitialised : 1;
    unsigned char   ScreenGraphic : 1;
    void Uninitialise();                            // 0x009FCBA0
    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    __forceinline void Release() { if (--ReferenceCount == 0) delete this; }
    __forceinline CTexture* GetTexture() const { return PTexture.Data; }
    __forceinline float GetFloatTLX() const { return TLX * (1.0f / 32767.0f); }
    __forceinline float GetFloatTLY() const { return TLY * (1.0f / 32767.0f); }
};
class CGraphicFrame { public:
    CGraphicFrameData* FrameData;
    __forceinline CGraphicFrame() : FrameData(0) { }
    __forceinline ~CGraphicFrame() { if (FrameData) { FrameData->Release(); FrameData = 0; } }
    __forceinline CTexture* GetTexture() const { return FrameData->GetTexture(); }
    __forceinline float GetFloatTLX() const { return FrameData->GetFloatTLX(); }
    __forceinline float GetFloatTLY() const { return FrameData->GetFloatTLY(); }
};
class CGraphicInfoHeader { public:                  // CGraphicDataBank::CGraphicHeader
    unsigned short Width, Height, Depth, FrameWidth, FrameHeight, FrameCount;
    unsigned char  _pad0c[0x1c - 0x0c];
};
class CGraphicInfo { public:
    CGraphicInfoHeader Header;                      // +0x00
    unsigned long      TextureType;                 // +0x1c
    void*              Bank;                        // +0x20
    long               Index;                       // +0x24
    void*              Frames;                      // +0x28
    bool GetFrame(CGraphicFrame& frame, unsigned long frameNum, unsigned long mipmapLevel, unsigned long flags) const;   // 0x009FF8D0
    __forceinline float GetFrameWidthFactor() const { return (float)Header.FrameWidth / Header.Width; }
    __forceinline float GetFrameHeightFactor() const { return (float)Header.FrameHeight / Header.Height; }
};
class CGraphicDataBank { public:
    unsigned char _pad000[0x1e0];
    CArray<CGraphicInfo> GraphicList;               // +0x1e0
    __forceinline const CGraphicInfo* PeekGraphic(unsigned long index) const
    {
        return index < GraphicList.size() ? &GraphicList[index] : 0;
    }
};
class CEngineCamera { public:
    unsigned char _pad000[0x114];
    CMatrix3x4 NoScaleViewTransform;                // +0x114
    CMatrix3x4 InverseNoScaleViewTransform;         // +0x144
    const CMatrix3x4& PeekNonHomogenousViewTransform() const { return NoScaleViewTransform; }
    const CMatrix3x4& PeekInverseNonHomogenousViewTransform() const { return InverseNoScaleViewTransform; }
};
class CEngineInternalPrimitiveParticleLine : public CEngineInternalPrimitiveBase {
public:
    C3DVector  Start;                               // +0x48
    C3DVector  End;                                 // +0x54
    float      Width;                               // +0x60
    CRGBColour Colour;                              // +0x64
};
struct SParticleLineNode {                          // STLport list<CEnginePrimitiveHandle> node
    SParticleLineNode* _M_next;
    SParticleLineNode* _M_prev;
    void*              HandleVTable;                // +0x08 CEnginePrimitiveHandle
    CEngineInternalPrimitiveParticleLine* Data;     // +0x0c
};
class CEngineInternalPrimitiveParticleLineGroup : public CEngineInternalPrimitiveBase {
public:
    SParticleLineNode* Primitives;                  // +0x48
    unsigned char _pad4c[0xac - 0x4c];
    unsigned long BankIndex;                        // +0xac
    unsigned short Flags;                           // +0xb0
    EEnginePrimitiveBlendMode BlendMode;            // +0xb4
    EEnginePrimitiveBlendOp BlendOp;                // +0xb8
};
class CEnginePrimitiveManagerParticleGroup : public CEnginePrimitiveManager { public:
    long SetUpPointLights();                                                       // 0x00BBC0B0
    void SetupPixelShader(unsigned long constant, const CPixelShader& a, const CPixelShader& b);   // 0x00BBCC70
};
class CEngineParticleConstantBlock { public: float X, Y, Z, W; };
class CEnginePrimitiveManagerParticleLineGroup : public CEnginePrimitiveManager {
public:
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass);
private:
    __forceinline void ApplyLinePointLightVertexShader(unsigned long lights)
    {
        unsigned long index = lights;
        if (lights > VertexShadersLinePointLights.size() - 1)
            index = VertexShadersLinePointLights.size() - 1;
        ApplyVertexShader(VertexShadersLinePointLights[index]);
    }
    __forceinline void ApplyDisplacementVertexShader() { ApplyVertexShader(VertexShaderDisplacement); }
    CArray<CVertexShader> VertexShadersLinePointLights;   // +0x0c
    CVertexShader VertexShaderDisplacement;         // +0x18
    CPixelShader PixelShader;                       // +0x20
    CPixelShader PixelShaderStipple;                // +0x28
    CPixelShader PixelShaderDisplacement;           // +0x30
    CVertexBufferWin32 VertexBuffer;                // +0x38
    CIndexBuffer IndexBuffer;                       // +0x6c
    CEngineParticleConstantBlock ConstantBlock[30]; // +0x80
};

__forceinline void __fastcall CEnginePrimitiveRenderUtil::ApplyVertexShader(const CVertexShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(shader, false);
}
__forceinline void __fastcall CEnginePrimitiveRenderUtil::ApplyPixelShader(const CPixelShader& shader)
{
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(shader);
}
__forceinline CEngineCamera& __fastcall CEnginePrimitiveRenderUtil::GetCurrentCamera()
{
    return *g_pEngineCamera;
}
__forceinline void CEnginePrimitiveRenderUtil::SetupBlendOpState(EEnginePrimitiveBlendOp op)
{
    switch (op) {
    case ENGINE_PRIMITIVE_BLEND_OP_SUBTRACT:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_SUBTRACT);
        break;
    case ENGINE_PRIMITIVE_BLEND_OP_REV_SUBTRACT:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_REVSUBTRACT);
        break;
    default:
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_ADD);
        break;
    }
}
__forceinline bool CEnginePrimitiveRenderUtil::SetupAlphaBlendingStatesFromBlendMode(EEnginePrimitiveBlendMode mode)
{
    switch (mode) {
    case ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
        return true;
    case ENGINE_PRIMITIVE_BLEND_MODE_ADDITIVE_INV_SRC_COLOUR:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_COLOR);
        return true;
    default:
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        return false;
    }
}

void CEnginePrimitiveManagerParticleLineGroup::RenderSubPrimitive(CEngineSubPrimitiveBase& sub, ERenderPassType pass)
{
    if (!g_particleLineRenderEnabled_0139b864)
        return;
    CEngineInternalPrimitiveParticleLineGroup* group = static_cast<CEngineInternalPrimitiveParticleLineGroup*>(sub.Parent);
    const CGraphicInfo* info = NEngineInternal::GPResourceManager->PeekGraphicBank(static_cast<EGraphicBankNo>(0))->PeekGraphic(group->BankIndex);
    CGraphicFrame frame;
    if (!info->GetFrame(frame, 0, 0, 0))
        return;
    NEngineInternal::GPRenderManager->AttachTextureToStage(0, frame.GetTexture());
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    CEnginePrimitiveManagerParticleGroup* groupManager = (CEnginePrimitiveManagerParticleGroup*)g_pEnginePrimitiveTypeInfo->PrimitiveManagers[24];
    bool premultiply;
    if (pass == 6 && g_pDisplacementRenderer_01436e38->PeekRenderEnabled()) {
        premultiply = false;
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_ADD);
        NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_SRC_ALPHA);
        NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_INV_SRC_ALPHA);
        ApplyDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
    } else if (pass == 7) {
        premultiply = false;
        ApplyDisplacementVertexShader();
        ApplyPixelShader(PixelShaderDisplacement);
    } else {
        SetupBlendOpState(group->BlendOp);
        premultiply = SetupAlphaBlendingStatesFromBlendMode(group->BlendMode);
        unsigned long lights = 0;
        if (group->Flags & 0x80)
            lights = groupManager->SetUpPointLights();
        else
            g_pEngineLightingManager->SetSelfIlluminationAmbient(100);
        g_pEngineLightingManager->UpdateGlobalLightingStates();
        ApplyLinePointLightVertexShader(lights);
        groupManager->SetupPixelShader(0, PixelShaderStipple, PixelShader);
    }
    NEngineInternal::GPRenderManager->SelectVertexBuffer(0, &VertexBuffer);
    NEngineInternal::GPRenderManager->SelectIndexBuffer(&IndexBuffer, 0);
    const CMatrix3x4& view = GetCurrentCamera().PeekNonHomogenousViewTransform();
    ConstantBlock[0].X = view.E11;
    ConstantBlock[0].Y = view.E21;
    ConstantBlock[0].Z = view.E31;
    ConstantBlock[0].W = view.E41;
    ConstantBlock[1].X = view.E12;
    ConstantBlock[1].Y = view.E22;
    ConstantBlock[1].Z = view.E32;
    ConstantBlock[1].W = view.E42;
    ConstantBlock[2].X = view.E13;
    ConstantBlock[2].Y = view.E23;
    ConstantBlock[2].Z = view.E33;
    ConstantBlock[2].W = view.E43;
    const CMatrix3x4& inverse = GetCurrentCamera().PeekInverseNonHomogenousViewTransform();
    ConstantBlock[3].X = inverse.E11;
    ConstantBlock[3].Y = inverse.E21;
    ConstantBlock[3].Z = inverse.E31;
    ConstantBlock[3].W = inverse.E41;
    ConstantBlock[4].X = inverse.E12;
    ConstantBlock[4].Y = inverse.E22;
    ConstantBlock[4].Z = inverse.E32;
    ConstantBlock[4].W = inverse.E42;
    ConstantBlock[5].X = inverse.E13;
    ConstantBlock[5].Y = inverse.E23;
    ConstantBlock[5].Z = inverse.E33;
    ConstantBlock[5].W = inverse.E43;
    ConstantBlock[6].X = frame.GetFloatTLX();
    ConstantBlock[6].Y = frame.GetFloatTLY();
    ConstantBlock[6].Z = info->GetFrameWidthFactor();
    ConstantBlock[6].W = info->GetFrameHeightFactor();
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(1, 7, &ConstantBlock[0].X);
    unsigned long count = 0;
    for (SParticleLineNode* node = group->Primitives->_M_next; node != group->Primitives; node = node->_M_next) {
        CEngineInternalPrimitiveParticleLine& line = *node->Data;
        CRGBFloatColour colour(line.Colour);
        colour.A *= 2.0f;
        if (premultiply) {
            colour.R *= colour.A;
            colour.G *= colour.A;
            colour.B *= colour.A;
        }
        CEngineParticleConstantBlock& start = ConstantBlock[count];
        start.X = line.Start.X;
        start.Y = line.Start.Y;
        start.Z = line.Start.Z;
        start.W = line.Width;
        ++count;
        CEngineParticleConstantBlock& end = ConstantBlock[count];
        end.X = line.End.X;
        end.Y = line.End.Y;
        end.Z = line.End.Z;
        end.W = 1.0f;
        ++count;
        CEngineParticleConstantBlock& col = ConstantBlock[count];
        col.X = colour.R;
        col.Y = colour.G;
        col.Z = colour.B;
        col.W = colour.A;
        ++count;
        if (count == 30) {
            NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(8, count, &ConstantBlock[0].X);
            NEngineInternal::GPShaderRenderManager->PrepareForRender(0x11);
            NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, 40, 0, 20);
            count = 0;
        }
    }
    if (count) {
        unsigned long lines = count / 3;
        NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstants(8, lines * 3, &ConstantBlock[0].X);
        NEngineInternal::GPShaderRenderManager->PrepareForRender(0x11);
        NEngineInternal::GPRenderManager->DrawCurrentBuffersAsIndexedTris(0, lines * 4, 0, lines * 2);
    }
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_CCW);
    g_pEngineLightingManager->RestoreDefaultPrimitiveParameters();
}
