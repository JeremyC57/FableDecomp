#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CRENDERMANAGER2D_LIFETIME_PASS23
#define FABLE_RETAIL_CTEXTURE
#include "engine/Direct3D9Types.h"
#define FABLE_RETAIL_CSYSTEMMANAGER_LIFETIME_PASS23
#define private public
// CEngineScreenEffectGlowRenderer::ApplyGlow() -- blends the glow textures onto the back buffer.
// Leaf: ?ApplyGlow@CEngineScreenEffectGlowRenderer@@AAEXXZ
// Flags: default
#define FABLE_RETAIL_CRENDERMANAGERCORE_DRAWASINDEXEDTRIS
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_SCOPED_FORCEINLINE
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESCREENEFFECTGLOWRENDERER_APPLYGLOW
#include "engine/CEngineScreenEffectGlowRenderer.h"
#include "engine/CRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CScopedShadersEnable.h"

namespace NEngineInternal {
extern CRenderManager* GPRenderManager;
extern CShaderRenderManager* GPShaderRenderManager;
}

void CEngineScreenEffectGlowRenderer::ApplyGlow()
{
    CRenderStateScopedCaptureBlock block;
    CScopedShadersEnable shaders_enable(true, true);
    NEngineInternal::GPShaderRenderManager->ApplyVertexShader(VertexShaderApplyGlow, false);
    NEngineInternal::GPShaderRenderManager->ApplyPixelShader(PixelShaderApplyGlow);
    NEngineInternal::GPShaderRenderManager->SetPixelShaderConstant(5, C4DVector(IntensityR * 8.0f, IntensityG * 8.0f, IntensityB * 8.0f, 1.0f));
    NEngineInternal::GPRenderManager->SetAlphaBlendEnable(true);
    NEngineInternal::GPRenderManager->SetAlphaTestEnable(false);
    NEngineInternal::GPRenderManager->SetSourceAlphaBlendMode(ALPHA_BLEND_ONE);
    NEngineInternal::GPRenderManager->SetDestAlphaBlendMode(ALPHA_BLEND_ONE);
    if (ThresholdR < 1.0f)
        NEngineInternal::GPRenderManager->SetBlendOp(BLEND_OP_REVSUBTRACT);
    NEngineInternal::GPRenderManager->SetZBufferEnable(false);
    NEngineInternal::GPRenderManager->SetPolyCullMode(POLY_CULL_NONE);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(0, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMag(1, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPRenderManager->SetTextureFilterModeMin(1, TEXTURE_FILTER_LINEAR);
    NEngineInternal::GPShaderRenderManager->PrepareForRender(0);
    const CTexture* textures[2];
    textures[0] = GlowTexture64;
    textures[1] = GlowTexture16;
    static unsigned short ids[] = {0, 1, 3, 1, 2, 3};
    NEngineInternal::GPRenderManager->DrawAsIndexedTris(textures, 2, TextureToBackBufferVerts, 24, 4, ids, 2);
}

#include "engine/CDisplayManager.h"
#include "engine/CSystemManager.h"
#include "engine/CEngineInit.h"
#undef private
#include <stdio.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CInitBaseClass::CInitBaseClass() {}
CInitBaseClass::~CInitBaseClass() {}
CSystemManager::CSystemManager() {}
CSystemManager::~CSystemManager() {}
CBusyResourceManager::CBusyResourceManager() {}
CBusyResourceManager::~CBusyResourceManager() {}
template <class T> boost::scoped_ptr<T>::~scoped_ptr() {}
CDisplayManager::CDisplayManager(CSystemManager& system) : SystemManager(system) {}
CDisplayManager::~CDisplayManager() {}
CWideString::CWideString() : storage_(0) {}
CWideString::~CWideString() {}
CCharString::CCharString() : storage_(0) {}
CCharString::~CCharString() {}
CEngineInit::CEngineInit() {}
CEngineInit::~CEngineInit() {}
CEngineMemoryInit::CEngineMemoryInit() {}
CVertexShaderConstantLayout::CVertexShaderConstantLayout() {}
CVertexShaderConstantLayout::~CVertexShaderConstantLayout() {}
CCharString CVertexShaderConstantLayout::PeekName() const { return CCharString(); }
CShaderConstantRange::CShaderConstantRange() {}
CShaderConstantRange::~CShaderConstantRange() {}

CEngineComponent::CEngineComponent() {}
CEngineComponent::~CEngineComponent() {}
void CEngineComponent::FrameStart(const CEngineComponentParams&) {}
void CEngineComponent::FrameEnd(const CEngineComponentParams&) {}
bool CEngineComponent::LoadNewScene(const CEngineComponentLoadNewSceneParams&) { return true; }
void CEngineComponent::Render(const CEngineComponentRenderParams&) {}
void CEngineComponent::PreLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::PostLayerRender(const CEngineComponentRenderParams&) {}
void CEngineComponent::WorldFrameUpdate(bool) {}
void CEngineComponent::PausedWorldFrameUpdate() {}
CCharString CEngineComponent::GetConsoleEnableFunctionName() { return CCharString(); }
void CEngineComponent::OnReleaseDefaultPoolResources() {}
bool CEngineComponent::OnRestoreDefaultPoolResources() { return true; }
CEngineScreenEffectGlowRenderer::CEngineScreenEffectGlowRenderer(const CEngineInit&) {}
unsigned long CEngineScreenEffectGlowRenderer::PeekSceneFilterFlags() const { return 0; }

CVertexShader::CVertexShader() : Data(0) {}
CPixelShader::CPixelShader() : Data(0) {}
CCharString CShaderBase::GetDebugName() const { return CCharString(); }
CCharString CVertexShader::GetDebugName() const { return CCharString(); }
CCharString CPixelShader::GetDebugName() const { return CCharString(); }
unsigned long CVertexShader::CountReferences() { return 0; }
unsigned long CPixelShader::CountReferences() { return 0; }

CRenderManager::CRenderManager(CDisplayManager& display, const CWideString& name, unsigned long flags)
    : CRenderManager2D(display, name, flags) {}
CRenderManager2D::CRenderManager2D(CDisplayManager& display, const CWideString&, unsigned long flags)
    : CRenderManagerCore(display, flags) {}
CRenderManager2D::~CRenderManager2D() {}
CRenderManagerCore::CRenderManagerCore(CDisplayManager&, unsigned long flags) : CRenderStateManager(flags) {}
CRenderManagerCore::~CRenderManagerCore() {}
void CTexture::Uninitialise() {}

CRenderStateManager::CStateInfo::CStateInfo()
    : CurrentState(0), DesiredState(0), BookmarkMask(0), DirtyListFlag(false), StateType(0), TextureStage(0) {}
CRenderStateManager::CDeviceState::CDeviceState() {}
CRenderStateManager::CRenderStateManager(unsigned long flags)
    : PD3DDevice(0), StateStackSize(0), StateUpdateListSize(0), AllStatesListSize(0), CurrentBookmarkMask(1), InitFlags(flags) {}
CRenderStateManager::~CRenderStateManager() {}
unsigned long CRenderStateManager::D3DAlphaBlendModeTable[13] = {0};
unsigned long CRenderStateManager::D3DBlendOpModeTable[5] = {0};
unsigned long CRenderStateManager::D3DPolyCullModeTable[3] = {0};
unsigned long CRenderStateManager::D3DTextureFilterModeTable[3] = {0};

namespace NEngineInternal {
CRenderManager* GPRenderManager;
CShaderRenderManager* GPShaderRenderManager;
}
CRenderStateManager* CRenderStateCaptureBlock::StateManager;
CShaderRenderManager CShaderRenderManager::SingletonInstance;
CShaderRenderManager::CShaderRenderManager()
    : LightPool(0, 0), WorldMatrixIsIdentity(true), VertexShadersEnabled(false), PixelShadersEnabled(false), PSConstTable(6), UpdateFlags(0) {}
CShaderRenderManager::~CShaderRenderManager() {}
CSimpleMemoryPool::CSimpleMemoryPool(unsigned long capacity, unsigned long growth) : FirstMemoryBlock(0), Capacity(capacity), GrowthGranularity(growth) {}
void CSimpleMemoryPool::ClearMemoryBlocks() {}

namespace {
const CEngineScreenEffectGlowRenderer* glow;
bool drawn;
bool restored;
bool appliedVertex;
bool appliedPixel;
bool constantUploaded;
bool subtract;
bool drawOK;
}
void CShaderRenderManager::EnableVertexShaders() { VertexShadersEnabled = true; }
void CShaderRenderManager::EnablePixelShaders() { PixelShadersEnabled = true; }
void CShaderRenderManager::DisableVertexShaders() { VertexShadersEnabled = false; }
void CShaderRenderManager::DisablePixelShaders() { PixelShadersEnabled = false; }
void CShaderRenderManager::ApplyVertexShader(const CVertexShader& shader, bool force)
{
    appliedVertex = &shader == &glow->VertexShaderApplyGlow && !force && VertexShadersEnabled && PixelShadersEnabled;
}
void CShaderRenderManager::ApplyPixelShader(const CPixelShader& shader)
{
    appliedPixel = appliedVertex && &shader == &glow->PixelShaderApplyGlow;
}
void CShaderRenderManager::UpdatePixelShaderConstants()
{
    const C4DVector& v = PSConstTable[5];
    constantUploaded = appliedPixel && v.X == 2.0f && v.Y == -4.0f && v.Z == 12.0f && v.W == 1.0f;
}
void CShaderRenderManager::SetVSConstants(long, const void*, unsigned long) {}
void CShaderRenderManager::UpdatePixelShader() {}
void CShaderRenderManager::UpdateBoneTransform() {}
void CShaderRenderManager::UpdateWorldSpaceTextureTransform() {}
void CShaderRenderManager::UpdateObjectSpaceTextureTransform() {}
void CShaderRenderManager::UpdateWorldSpaceFogTransform() {}
void CShaderRenderManager::UpdateObjectSpaceFogTransform() {}
void CShaderRenderManager::UpdateAmbient() {}
void CShaderRenderManager::UpdateFogColour() {}
void CShaderRenderManager::UpdateLightGlobals() {}
void CShaderRenderManager::UpdateObjectSpaceLighting(unsigned long) {}
void CShaderRenderManager::UpdateWorldSpaceLighting() {}
void CShaderRenderManager::UpdateLocalisedSphereMapping() {}
void CShaderRenderManager::ResetWorldTransform() {}
void CShaderRenderManager::UpdateCombinedProjectionTransform() {}
void CShaderRenderManager::UpdateCameraPos() {}

void CRenderStateManager::RestoreCaptureBlock()
{
    restored = drawn && StateStack[0].State == &BookmarkDummyState && CurrentBookmarkMask == 2;
    while (StateStackSize > 1) {
        CStateStackElement& saved = StateStack[--StateStackSize];
        saved.State->DesiredState = saved.Value;
        saved.State->BookmarkMask &= ~CurrentBookmarkMask;
        saved.State->DirtyListFlag = false;
    }
    StateStackSize = StateUpdateListSize = 0;
    CurrentBookmarkMask >>= 1;
}

void CRenderManagerCore::DrawAsIndexedTris(const CTexture** textures, unsigned long count, const void* vertices,
                                          long stride, long number, void* indices, long triangles)
{
    static const unsigned short expected[] = {0, 1, 3, 1, 2, 3};
    drawOK = constantUploaded && !drawn && count == 2 && textures[0] == glow->GlowTexture64
             && textures[1] == glow->GlowTexture16 && vertices == glow->TextureToBackBufferVerts
             && stride == 24 && number == 4 && triangles == 2;
    for (unsigned int i = 0; i < 6; ++i)
        drawOK = drawOK && ((const unsigned short*)indices)[i] == expected[i];
    drawOK = drawOK && DeviceState.AlphaBlendEnable.DesiredState == 1
             && DeviceState.AlphaTestEnable.DesiredState == 0 && DeviceState.ZBufferEnable.DesiredState == 0
             && DeviceState.SourceAlphaBlendMode.DesiredState == D3DAlphaBlendModeTable[ALPHA_BLEND_ONE]
             && DeviceState.DestAlphaBlendMode.DesiredState == D3DAlphaBlendModeTable[ALPHA_BLEND_ONE]
             && DeviceState.PolyCullMode.DesiredState == D3DPolyCullModeTable[POLY_CULL_NONE]
             && DeviceState.BlendOpMode.DesiredState == (subtract ? D3DBlendOpModeTable[BLEND_OP_REVSUBTRACT] : 0);
    for (long stage = 0; stage < 2; ++stage)
        drawOK = drawOK && DeviceState.TextureFilterModeMag[stage].DesiredState == D3DTextureFilterModeTable[TEXTURE_FILTER_LINEAR]
                 && DeviceState.TextureFilterModeMin[stage].DesiredState == D3DTextureFilterModeTable[TEXTURE_FILTER_LINEAR];
    drawn = true;
}

int main()
{
    CSystemManager system;
    CDisplayManager display(system);
    const CWideString name;
    CRenderManager render(display, name, 0);
    CEngineInit init;
    CEngineScreenEffectGlowRenderer renderer(init);
    CTexture tex64, tex16;
    renderer.GlowTexture64 = &tex64;
    renderer.GlowTexture16 = &tex16;
    renderer.IntensityR = 0.25f;
    renderer.IntensityG = -0.5f;
    renderer.IntensityB = 1.5f;

    glow = &renderer;
    NEngineInternal::GPRenderManager = &render;
    CShaderRenderManager& shaders = CShaderRenderManager::Get();
    NEngineInternal::GPShaderRenderManager = &shaders;
    CRenderStateCaptureBlock::StateManager = &render;
    render.D3DAlphaBlendModeTable[ALPHA_BLEND_ONE] = 2;
    render.D3DBlendOpModeTable[BLEND_OP_REVSUBTRACT] = 3;
    render.D3DPolyCullModeTable[POLY_CULL_NONE] = 1;
    render.D3DTextureFilterModeTable[TEXTURE_FILTER_LINEAR] = 2;
    render.DeviceState.AlphaTestEnable.DesiredState = 1;
    render.DeviceState.ZBufferEnable.DesiredState = 1;

    const float values[] = {0.5f, 1.0f, 1.5f};
    for (unsigned int i = 0; i < 3; ++i) {
        drawn = restored = appliedVertex = appliedPixel = constantUploaded = drawOK = false;
        shaders.VertexShadersEnabled = (i == 1);
        shaders.PixelShadersEnabled = (i == 2);
        renderer.ThresholdR = values[i];
        subtract = (i == 0);
        renderer.ApplyGlow();
        if (!drawOK || !restored || shaders.VertexShadersEnabled != (i == 1) || shaders.PixelShadersEnabled != (i == 2))
            return 1 + i;
        if (render.DeviceState.AlphaTestEnable.DesiredState != 1 || render.DeviceState.ZBufferEnable.DesiredState != 1
            || render.DeviceState.BlendOpMode.DesiredState != 0 || render.StateStackSize != 0)
            return 4;
    }
    puts("00b60f70_TEST PASS");
    return 0;
}