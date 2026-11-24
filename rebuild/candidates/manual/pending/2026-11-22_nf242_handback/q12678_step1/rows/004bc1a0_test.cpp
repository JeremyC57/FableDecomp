// NF242 (q12678 step 1): the CIEngine slot 8 / 11 / 12 / 13 overrides respelled from the retired VirtualNN_ placeholders to the PDB
// declarations engine/CIEngine.h now makes the default (Render, ConvertScreenPosToWorldPos, ...AtViewDistance, ConvertWorldPosToScreenPos).
#include <assert.h>
#include <stdio.h>
// Leaf: ?DrawRHSet@NGraphicAppearance@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@ABVCRightHandedSet@@MJ@Z
// Flags:
#define FABLE_RETAIL_CENGINEPRIMITIVELINE_INLINE_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CRGBCOLOUR_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CRightHandedSet.h"

namespace NGraphicAppearance
{
void FABLE_FASTCALL DrawRHSet(const CEnginePrimitivePrepareInfo& info, const C3DVector& coord,
                             const CRightHandedSet& rhset, float width, long alpha)
{
    info.Engine.AddPrimitive(CEnginePrimitiveLine(
        0, coord, coord + rhset.GetUp(), CRGBColour(100, 100, 255, (unsigned char)alpha),
        width, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD));
    info.Engine.AddPrimitive(CEnginePrimitiveLine(
        0, coord, coord + rhset.GetRight(), CRGBColour(255, 100, 100, (unsigned char)alpha),
        width, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD));
    info.Engine.AddPrimitive(CEnginePrimitiveLine(
        0, coord, coord + rhset.GetForward(), CRGBColour(255, 255, 255, (unsigned char)alpha),
        width, ENGINE_PRIMITIVE_BLEND_SOLID, 0, true, ENGINE_INTERPOLATION_SET_DEFAULT, ENGINE_PRIMITIVE_BLEND_OP_ADD));
}
}

#include "engine/CRGBFloatColour.h"
#include "fable_string.h"

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CInitBaseClass::CInitBaseClass() : Valid(false) {}
CInitBaseClass::~CInitBaseClass() {}
CIEngine::~CIEngine() {}
CCharString::~CCharString() {}

CInterpolationInfo::CInterpolationInfo()
    : Current(), Last(), PausedCurrent(), PausedLast(),
      BulletTimeCurrent(), BulletTimeLast(), WFServerCurrentTime(0)
{
}

CEnginePrimitivePrepareInfo::CEnginePrimitivePrepareInfo(
    const CInterpolationInfo& interpolation, CIEngine& engine, bool edit, const CCamera& camera)
    : Interpolate(interpolation), Engine(engine), Edit2DEngine(edit), Camera(camera)
{
}

// Only slot 20 is exercised. These definitions provide the rest of the shared interface's vtable.
void CIEngine::Validate() const {}
bool CIEngine::Initialise(const CEngineInit&) { return false; }
void CIEngine::Release() {}
void CIEngine::SetEnvironmentalEffectsAlwaysUpdate(bool) {}
void CIEngine::SetWorld(const CIDrawWorld*) {}
void CIEngine::WorldFrameUpdate(bool) {}
void CIEngine::PausedWorldFrameUpdate() {}
bool CIEngine::SetVideoOptions(const CEngineVideoOptions&) { return false; }
void CIEngine::Refresh() {}
bool CIEngine::CheckPrimitiveVisibility(unsigned __int64) { return false; }
float CIEngine::GetPrimitiveDrawDistanceFactor() const { return 0; }
bool CIEngine::ClipSphere(const C3DBoundingSphere&) const { return false; }
bool CIEngine::ClipBox(const C3DBoundingBox&) const { return false; }
void CIEngine::SetPrimitiveAnimation(CEnginePrimitiveHandle&, const CEnginePrimitiveAnimation&) {}
void CIEngine::SetPrimitivePosition(CEnginePrimitiveHandle&, const CEnginePrimitivePosition&) {}
unsigned long CIEngine::AddChildPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) { return 0; }
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, const CEnginePrimitive&, unsigned long) {}
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) {}
void CIEngine::GetChildPrimitiveHandle(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&) {}
void CIEngine::ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long) {}
void CIEngine::ClearAllChildPrimitives(CEnginePrimitiveHandle&) {}
void CIEngine::SetAttachedPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, const CEnginePrimitiveAttachmentInfo&, const CEnginePrimitiveHandle&, long, unsigned long) {}
void CIEngine::CollectPrimitiveStatsForArea(std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > >&, const C2DBoxF&, EEnginePrimitiveStatIndex, unsigned long) const {}
void CIEngine::ProcessPrimitiveStats(const std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > >&, EEnginePrimitiveStatIndex, unsigned long&, CArray<CCharString>&) const {}
const C3DAnimationInfo* CIEngine::GetEditorAnimation(const CWideString&) const { return 0; }
unsigned long CIEngine::AddCustomMorphTexture(CTexture&, const CCharString&) const { return 0; }
long CIEngine::GetStaticMapUpdatePassCount() { return 0; }
void CIEngine::StartStaticMapUpdatePass(const CWideString&, long, const CStaticMapGenerationVisibiilityInfo&, const CArray<CCharString>&) {}
bool CIEngine::IsStaticMapNeedingUpdate(long, const CCharString&, unsigned long) const { return false; }
void CIEngine::UpdateStaticMap(long, const CCharString&, unsigned long, long) {}
void CIEngine::CloseStaticMapFile() {}
void CIEngine::AddMap(long, long) {}
void CIEngine::RemoveMap(long) {}
void CIEngine::SetMapAsLoaded(long) {}
void CIEngine::SetMapAsUnloaded(long) {}
void CIEngine::SetMapVisibilityAllMaps(bool) {}
void CIEngine::SetMapVisibility(long, bool) {}
void CIEngine::AreaChanged(const C2DBoxI&) {}
void CIEngine::OnActivate() {}
void CIEngine::OnDeactivate() {}
bool CIEngine::IsActive() const { return false; }
void CIEngine::SetStaticMapFileForUse(const CWideString&) {}
void CIEngine::SetLandscapeDebugTintColour(long, CRGBColour) {}
void CIEngine::ReloadLocalDetailDefs() {}
void CIEngine::StartNewScene(unsigned long, float) {}
void CIEngine::CutscenePreload(bool, bool) {}
void CIEngine::CutsceneAddCameraAngle(const CCamera&) {}
void CIEngine::ChangeCameraAngle() {}
bool CIEngine::IsPreloaded() const { return false; }
void CIEngine::UpdateTextureMorphCache(const CArray<CEngineTextureMorph>&, bool, float, const std::set<long, std::less<long>, std::allocator<long> >*) {}
void CIEngine::SetScreenColourFilter(const CEngineColourFilterSettings&) {}
void CIEngine::SetSpotlightShadowMode(bool) {}
void CIEngine::SetSeaBodyToRender(CCharString) {}
void CIEngine::UpdateDebugOverlay(long, const CRGBColour*, const C2DBoxI&) {}
void CIEngine::EnableDebugOverlay(long, bool, bool) {}
void CIEngine::SetGlobalLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&) {}
void CIEngine::Set2DPrimitiveLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&) {}
void CIEngine::SetRenderTargetTexture(const CTexture*) {}
CTexture* CIEngine::AllocateTextureFromEngineResourcePools(unsigned long, const C2DExtentsI&, long, const CPixelFormat&) { return 0; }
void CIEngine::FreeTextureFromEngineResourcePools(CTexture*) {}
CMovableResourceMemoryPool* CIEngine::GetLoadingMemoryPool() { return 0; }
CMovableResourceMemoryPool* CIEngine::GetAnimationMemoryPool() { return 0; }
CGPUScratchBufferTextureManager* CIEngine::GetScratchBufferTextureManager() { return 0; }
bool CIEngine::DeallocateShadowBuffer() { return false; }
void CIEngine::ReallocateShadowBuffer() {}
void CIEngine::Enable3DSceneRendering(bool) {}
bool CIEngine::Is3DSceneRenderingEnabled() { return false; }
void CIEngine::Enable2DSceneRendering(bool) {}
bool CIEngine::Is2DSceneRenderingEnabled() { return false; }
void CIEngine::OverrideRain(float) {}
void CIEngine::OverrideSnow(float) {}
void CIEngine::RenderColorFilteredQuad(float, float, const C4DVector&, float, float, CTexture*) {}
const CRGBFloatColour CIEngine::PeekGlobalDiffuseColour() const { return CRGBFloatColour(0, 0, 0, 0); }
const CRGBFloatColour CIEngine::PeekGlobalAmbientColour() const { return CRGBFloatColour(0, 0, 0, 0); }
void CIEngine::AddPrimitive(const CEnginePrimitive&) { assert(false); }

namespace
{
struct RecordedLine
{
    C3DVector Start;
    C3DVector End;
    CRGBColour Colour;
    float Width;
};

class RecordingEngine : public CIEngine
{
public:
    RecordingEngine() : Count(0) {}
    virtual void AddPrimitive(const CEnginePrimitive& primitive)
    {
        assert(Count < 3);
        assert(primitive.Type == ENGINE_PRIMITIVE_LINE);
        assert(primitive.InterpolationSet == ENGINE_INTERPOLATION_SET_DEFAULT);
        const CEnginePrimitiveLine& line = static_cast<const CEnginePrimitiveLine&>(primitive);
        assert(line.BankIndex == 0 && line.UseZBuffer);
        assert(line.BlendOp == ENGINE_PRIMITIVE_BLEND_OP_ADD);
        assert(line.BlendMode == ENGINE_PRIMITIVE_BLEND_SOLID);
        Lines[Count].Start = line.Start;
        Lines[Count].End = line.End;
        Lines[Count].Colour = line.Colour;
        Lines[Count].Width = line.Width;
        ++Count;
    }

    RecordedLine Lines[3];
    int Count;

private:
    virtual void SetCamera(const CCamera*) {}
    virtual void SetWindow(const C2DBoxF*) {}
    virtual void Render(const CEngineInterpolationInfo&, unsigned long) {}
    virtual bool ConvertScreenPosToWorldPos(const C2DVector&, C3DVector&) const { return false; }
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float, const C2DVector&, C3DVector&) const { return false; }
    virtual bool ConvertWorldPosToScreenPos(const C3DVector&, C2DVector&) const { return false; }
    virtual bool PreChangeResolution() { return false; }
    virtual bool PostChangeResolution(unsigned long, unsigned long, unsigned long) { return false; }
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) {}
};

void CheckVector(const C3DVector& value, float x, float y, float z)
{
    assert(value.X == x && value.Y == y && value.Z == z);
}

void CheckColour(const CRGBColour& value, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    assert(value.R == r && value.G == g && value.B == b && value.A == a);
}
}

int main()
{
    RecordingEngine engine;
    CInterpolationInfo interpolation;
    CCamera camera = CCamera();
    CEnginePrimitivePrepareInfo info(interpolation, engine, false, camera);
    CRightHandedSet rhset;
    rhset.SetUp(C3DVector(1.0f, 2.0f, 3.0f));
    rhset.SetForward(C3DVector(4.0f, 5.0f, 6.0f));

    NGraphicAppearance::DrawRHSet(info, C3DVector(10.0f, 20.0f, 30.0f), rhset, 0.25f, 0x1ab);
    assert(engine.Count == 3);
    for (int i = 0; i != 3; ++i)
    {
        CheckVector(engine.Lines[i].Start, 10.0f, 20.0f, 30.0f);
        assert(engine.Lines[i].Width == 0.25f);
    }
    CheckVector(engine.Lines[0].End, 11.0f, 22.0f, 33.0f);
    CheckVector(engine.Lines[1].End, 13.0f, 14.0f, 33.0f);
    CheckVector(engine.Lines[2].End, 14.0f, 25.0f, 36.0f);
    CheckColour(engine.Lines[0].Colour, 100, 100, 255, 0xab);
    CheckColour(engine.Lines[1].Colour, 255, 100, 100, 0xab);
    CheckColour(engine.Lines[2].Colour, 255, 255, 255, 0xab);

    engine.Count = 0;
    rhset.SetUp(C3DVector(0.0f, 0.0f, 0.0f));
    rhset.SetForward(C3DVector(0.0f, 0.0f, 0.0f));
    NGraphicAppearance::DrawRHSet(info, C3DVector(-2.0f, 0.0f, 4.0f), rhset, 0.0f, 0);
    assert(engine.Count == 3);
    for (int j = 0; j != 3; ++j)
    {
        CheckVector(engine.Lines[j].End, -2.0f, 0.0f, 4.0f);
        assert(engine.Lines[j].Width == 0.0f && engine.Lines[j].Colour.A == 0);
    }
    puts("004bc1a0_TEST PASS");
}