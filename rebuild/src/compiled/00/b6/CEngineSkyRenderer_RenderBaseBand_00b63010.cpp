// CEngineSkyRenderer::RenderBaseBand() -- draw the sky's base band inside a render-state capture block: apply the base-band vertex/pixel shaders, upload the current environment's base-band colour to user vertex constant 0, clear the tracked states +0x28b8/+0x28cc, flush the shader state (PrepareForRender(0) inlined) and draw the band's 0x24 indexed triangles
// NF151 (2026-11-20, q4541): the row-local CShaderRenderManager declaration respelled in place to ?ApplyPixelShader@CShaderRenderManager@@QAEXABVCPixelShader@@@Z (0x00988140; Ego_r.pdb 0x00AE47A0 + FableWin.pdb 0x030325A0: const CPixelShader&).
// NF127 (2026-11-20): the row-local CShaderRenderManager declarations respelled in place to ?ApplyVertexShader@CShaderRenderManager@@QAEXABVCVertexShader@@_N@Z (0x00988020; Ego_r.pdb 0x00AE4680 + FableWin.pdb 0x03031510: const CVertexShader&, bool).
// Leaf: RenderBaseBand
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B63010, length 592 bytes (body 0x245 + int3 padding), plain `ret`.
// Callees: CShaderRenderManager::ApplyVertexShader(CVertexShader*, bool) @ 0x00988020, ApplyPixelShader(CPixelShader*)
//   @ 0x00988140, SetUserVertexShaderConstant(ulong, const C4DVector*) @ 0x00989B50 and the inlined PrepareForRender
//   (landed model of 0x00B54100 RenderRainBox: SetVSConstants 0x0098B930, UpdatePixelShader 0x00988A20,
//   UpdatePixelShaderConstants 0x00989BF0, UpdateAmbient 0x00989760, UpdateFogColour 0x009897C0, UpdateLightGlobals
//   0x0098A760, ResetWorldTransform 0x00988290, UpdateCombinedProjectionTransform 0x00988A50); CRenderManagerCore::
//   DrawCurrentBuffersIndexed(ulong x4) @ 0x00A0AD40; CRenderStateManager::RestoreCaptureBlock @ 0x00A05840.
// Globals: the render-state manager 0x013CB508 (capture block), the render core 0x01436E18 (tracked states, the landed
//   RenderSky 0x00B66190 model: `state.Set(v)` forwarding to an inline ChangeState), the shader manager 0x01436E14 and the
//   engine state 0x01436E8C whose +0x198 provider yields the current environment (slot 1, landed 0x00B80790) and its
//   settings (slot 1); the settings' sky block (+0x54) holds the base-band colour at +0x50.
// Layout: engine/CEngineSkyRenderer.h (BaseBandVertexShader +0x118, BaseBandPixelShader +0x138).
#include "fable_compiler.h"
// NF45 (round 7): the render core is NEngineInternal::GPRenderManager 0x01436E18 (Ego_r.pdb / FableWin.pdb
//   ?GPRenderManager@NEngineInternal@@3PAVCRenderManager@@A, engine/CRenderManager.h; CRenderManagerCore / CRenderStateManager at
//   offset 0): the shared inline setters AlphaBlendEnable (+0x28B8) / AlphaTestEnable (+0x28CC) false and
//   ?DrawCurrentBuffersIndexed@CRenderManagerCore@@QAEXKKKK@Z 0x00A0AD40; the capture open / RestoreCaptureBlock pair on 0x013CB508
//   is a CRenderStateScopedCaptureBlock (engine/CRenderStateCaptureBlock.h). Was a local CRenderManagerCoreView /
//   CTrackedRenderState model with g_pRenderManagerCore / g_pRenderStateManager. Not changed here: the 0x01436E8C read
//   (NEngineInternal::GPEngineWorldMap -> PeekGameWorld()->DrawGetEnvironment()->PeekInterpolatedParams().ThemeChannels[0]
//   Colours[5], the 0x00B65A20 shape) still goes through the SEngineState_01436e8c / CEnvironmentProvider view.
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
namespace NEngineInternal { extern CRenderManager* GPRenderManager; }   // 0x01436E18
#include "engine/CEngineSkyRenderer.h"

class C4DVector { public: float X, Y, Z, W; };


template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};
class CShaderConstantRange { public:
    void* _vptr;                                        // +0
    long Offset;                                        // +4
    unsigned long Count;                                // +8
};
class CVertexShaderConstantLayout { public:
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10 the other ranges
    CArray<C4DVector> ReservedPresets;                  // +0x140
};
class CVertexShader;
class CPixelShader;

class CShaderRenderManager {
public:
    void ApplyVertexShader(const CVertexShader& shader, bool force);                  // 0x00988020
    void ApplyPixelShader(const CPixelShader& shader);                                // 0x00988140
    void SetUserVertexShaderConstant(unsigned long index, const C4DVector* value);   // 0x00989B50

    FABLE_FORCEINLINE void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetData(),
                           CurrentVSConstantLayout->ReservedPresets.Size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

    void* _vptr;                                        // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char _pad008[0x1e0];                       // +0x008
    bool WorldMatrixIsIdentity;                         // +0x1e8
    unsigned char _pad1e9[0x1ef];                       // +0x1e9
    unsigned long UpdateFlags;                          // +0x3d8

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930
    void UpdatePixelShader();                           // 0x00988A20
    void UpdatePixelShaderConstants();                  // 0x00989BF0
    void UpdateBoneTransform();                         // 0x0098A8E0
    void UpdateWorldSpaceTextureTransform();            // 0x009894B0
    void UpdateObjectSpaceTextureTransform();           // 0x00989230
    void UpdateWorldSpaceFogTransform();                // 0x00989640
    void UpdateObjectSpaceFogTransform();               // 0x00989540
    void UpdateAmbient();                               // 0x00989760
    void UpdateFogColour();                             // 0x009897C0
    void UpdateLightGlobals();                          // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long flags);   // 0x00989CB0
    void UpdateWorldSpaceLighting();                    // 0x0098A540
    void UpdateLocalisedSphereMapping();                // 0x00988E80
    public: void ResetWorldTransform(); private:                         // 0x00988290
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
};
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }    // 0x01436E14

struct SSkySettings {
    unsigned char _pad00[0x50];
    C4DVector     BaseBandColour;                       // +0x50
};
struct SEnvironmentSettings {
    unsigned char _pad00[0x54];
    SSkySettings* m_pSky;                               // +0x54
};
class CEnvironment { public:
    virtual void v00();
    virtual SEnvironmentSettings* GetSettings();        // slot 1
};
struct CEnvironmentProvider {
    virtual void v00();
    virtual CEnvironment* GetCurrentEnvironment();      // slot 1
};
struct SEngineState_01436e8c {
    unsigned char         _pad[0x198];
    CEnvironmentProvider* m_pEnvironmentProvider;       // +0x198
};
extern SEngineState_01436e8c* g_pEngineState_01436e8c;  // 0x01436E8C

struct CEngineSkyRenderer_Methods : CEngineSkyRenderer {
    void RenderBaseBand();
};

void CEngineSkyRenderer_Methods::RenderBaseBand()
{
    CRenderStateScopedCaptureBlock capture_block;
    SEnvironmentSettings* settings = g_pEngineState_01436e8c->m_pEnvironmentProvider->GetCurrentEnvironment()->GetSettings();
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(*(CVertexShader*)BaseBandVertexShader, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(*(CPixelShader*)BaseBandPixelShader);
    SSkySettings* sky = settings->m_pSky;
    C4DVector colour = sky->BaseBandColour;
    NEngineInternal::GPShaderRenderManager->SetUserVertexShaderConstant(0, &colour);
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(false);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    NEngineInternal::GPRenderManager->DrawCurrentBuffersIndexed(0x196, 0x25, 0x804, 0x24);
}