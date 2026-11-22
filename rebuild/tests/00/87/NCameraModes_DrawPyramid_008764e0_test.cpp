// NF244 (q12680 step 1): the CIEngine slot 11 / 12 / 13 overrides respelled from the retired VirtualNN_ placeholders to the PDB
// declarations engine/CIEngine.h now makes the default (ConvertScreenPosToWorldPos, ...AtViewDistance, ConvertWorldPosToScreenPos).
// NCameraModes::DrawPyramid(const CEnginePrimitivePrepareInfo&, const C3DVector&, float) -- draws an animated pyramid of three wire rings.
// Leaf: ?DrawPyramid@NCameraModes@@YIXABVCEnginePrimitivePrepareInfo@@ABVC3DVector@@M@Z
// Flags: /Gr
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_MATH_TRIG_TWO_PI
#define FABLE_RETAIL_CRGBCOLOUR
#include "rebuild_abi.h"
#include "engine/GFSin.h"
#include "engine/GFCos.h"
#include "engine/GFSqr.h"
#include "engine/GFExp.h"
#include "engine/GFTanh.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVELINE_INLINE_CTOR
#include "engine/CEnginePrimitiveLine.h"
#include "engine/CEnginePrimitiveEnums.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/CWorld.h"
#include "fable_game.h"
#pragma intrinsic(sin, cos, exp)

namespace NCameraModes {
void __fastcall DrawPyramid(const CEnginePrimitivePrepareInfo& info, const C3DVector& centre, float size)
{
    C3DVector pt[4];
    CRGBColour col;
    long number_of_guys = 3;
    long seconds_per_cycle = 3;
    for (long c2 = 0; c2 < number_of_guys; ++c2) {
        unsigned long frames_per_cycle = seconds_per_cycle * CMainGameComponent::GetConstantFPS();
        // Inferred: the floating phase local keeps the retail reciprocal multiply before the frame conversion.
        const float phase = (float)c2 / number_of_guys;
        unsigned long number = (unsigned long)(CWorld::GetFrame() + phase * frames_per_cycle);
        unsigned long dyn_frame = number % frames_per_cycle;
        float scale_factor = (float)dyn_frame / frames_per_cycle;
        float dyn_z = (float)(scale_factor * 1.5f * size);
        float dyn_size = (float)((1.05f - scale_factor) * size);
        float x = scale_factor;
        float k = 11.0f;
        // Bracket: group the red and green fade products to reproduce the pointer setup at 0x0087662D.
        float red = (float)GFExp((-k * GFSqr(x)));
        float green = (float)GFExp((-k * GFSqr((float)(x - 0.5f))));
        float blue = (float)GFExp(-k * GFSqr((float)(x - 1.0f)));
        col = CRGBColour((unsigned char)(red * 255.0f), (unsigned char)(green * 255.0f), (unsigned char)(blue * 255.0f));
        for (long c1 = 0; c1 < 4; ++c1) {
            C3DVector temp_vec;
            temp_vec.X = (long)(1.25f - GFSqr((float)(1.5f - c1))) * dyn_size;
            temp_vec.Y = (long)GFTanh((1.5f - c1) * 100.0f) * dyn_size;
            temp_vec.Z = dyn_z;
            pt[c1].Z = temp_vec.Z;
            pt[c1].X = (float)(temp_vec.X * GFCos(scale_factor) + temp_vec.Y * GFSin(scale_factor));
            // Bracket: group the cosine term to retain the x87 sums and final stores at 0x008766CA.
            pt[c1].Y = (float)(-temp_vec.X * GFSin(scale_factor) + (temp_vec.Y * GFCos(scale_factor)));
            pt[c1] += centre;
        }
        info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[0], pt[1], col, 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0));
        info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[1], pt[2], col, 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0));
        info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[2], pt[3], col, 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0));
        info.Engine.AddPrimitive(CEnginePrimitiveLine(0, pt[3], pt[0], col, 0.05f, (EEnginePrimitiveBlendMode)0, 0, true, (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0));
    }
}
}

#include "engine/CRGBFloatColour.h"
#include <stdio.h>
#include <string.h>

// Fixture implementations for unused CIEngine device and scene methods.
void CIEngine::Validate() const { }
bool CIEngine::Initialise(const CEngineInit&) { return false; }
void CIEngine::Release() { }
void CIEngine::SetEnvironmentalEffectsAlwaysUpdate(bool) { }
void CIEngine::SetWorld(const CIDrawWorld*) { }
void CIEngine::WorldFrameUpdate(bool) { }
void CIEngine::PausedWorldFrameUpdate() { }
bool CIEngine::SetVideoOptions(const CEngineVideoOptions&) { return false; }
void CIEngine::Refresh() { }
bool CIEngine::CheckPrimitiveVisibility(unsigned __int64) { return false; }
float CIEngine::GetPrimitiveDrawDistanceFactor() const { return 0; }
bool CIEngine::ClipSphere(const C3DBoundingSphere&) const { return false; }
bool CIEngine::ClipBox(const C3DBoundingBox&) const { return false; }
void CIEngine::SetPrimitiveAnimation(CEnginePrimitiveHandle&, const CEnginePrimitiveAnimation&) { }
void CIEngine::SetPrimitivePosition(CEnginePrimitiveHandle&, const CEnginePrimitivePosition&) { }
unsigned long CIEngine::AddChildPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) { return 0; }
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, const CEnginePrimitive&, unsigned long) { }
void CIEngine::SetChildPrimitive(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) { }
void CIEngine::GetChildPrimitiveHandle(CEnginePrimitiveHandle&, unsigned long, CEnginePrimitiveHandle&) { }
void CIEngine::ClearChildPrimitive(CEnginePrimitiveHandle&, unsigned long) { }
void CIEngine::ClearAllChildPrimitives(CEnginePrimitiveHandle&) { }
void CIEngine::SetAttachedPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, const CEnginePrimitiveAttachmentInfo&, const CEnginePrimitiveHandle&, long, unsigned long) { }
void CIEngine::CollectPrimitiveStatsForArea(std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > >&, const C2DBoxF&, EEnginePrimitiveStatIndex, unsigned long) const { }
void CIEngine::ProcessPrimitiveStats(const std::map<unsigned long, unsigned long, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, unsigned long> > >&, EEnginePrimitiveStatIndex, unsigned long&, CArray<CCharString>&) const { }
const C3DAnimationInfo* CIEngine::GetEditorAnimation(const CWideString&) const { return 0; }
unsigned long CIEngine::AddCustomMorphTexture(CTexture&, const CCharString&) const { return 0; }
long CIEngine::GetStaticMapUpdatePassCount() { return 0; }
void CIEngine::StartStaticMapUpdatePass(const CWideString&, long, const CStaticMapGenerationVisibiilityInfo&, const CArray<CCharString>&) { }
bool CIEngine::IsStaticMapNeedingUpdate(long, const CCharString&, unsigned long) const { return false; }
void CIEngine::UpdateStaticMap(long, const CCharString&, unsigned long, long) { }
void CIEngine::CloseStaticMapFile() { }
void CIEngine::AddMap(long, long) { }
void CIEngine::RemoveMap(long) { }
void CIEngine::SetMapAsLoaded(long) { }
void CIEngine::SetMapAsUnloaded(long) { }
void CIEngine::SetMapVisibilityAllMaps(bool) { }
void CIEngine::SetMapVisibility(long, bool) { }
void CIEngine::AreaChanged(const C2DBoxI&) { }
void CIEngine::OnActivate() { }
void CIEngine::OnDeactivate() { }
bool CIEngine::IsActive() const { return false; }
void CIEngine::SetStaticMapFileForUse(const CWideString&) { }
void CIEngine::SetLandscapeDebugTintColour(long, CRGBColour) { }
void CIEngine::ReloadLocalDetailDefs() { }
void CIEngine::StartNewScene(unsigned long, float) { }
void CIEngine::CutscenePreload(bool, bool) { }
void CIEngine::CutsceneAddCameraAngle(const CCamera&) { }
void CIEngine::ChangeCameraAngle() { }
bool CIEngine::IsPreloaded() const { return false; }
void CIEngine::UpdateTextureMorphCache(const CArray<CEngineTextureMorph>&, bool, float, const std::set<long, std::less<long>, std::allocator<long> >*) { }
void CIEngine::SetScreenColourFilter(const CEngineColourFilterSettings&) { }
void CIEngine::SetSpotlightShadowMode(bool) { }
void CIEngine::SetSeaBodyToRender(CCharString) { }
void CIEngine::UpdateDebugOverlay(long, const CRGBColour*, const C2DBoxI&) { }
void CIEngine::EnableDebugOverlay(long, bool, bool) { }
void CIEngine::SetGlobalLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&) { }
void CIEngine::Set2DPrimitiveLightingParameters(const C3DVector&, const CRGBFloatColour&, const CRGBFloatColour&, const CRGBFloatColour&) { }
void CIEngine::SetRenderTargetTexture(const CTexture*) { }
CTexture* CIEngine::AllocateTextureFromEngineResourcePools(unsigned long, const C2DExtentsI&, long, const CPixelFormat&) { return 0; }
void CIEngine::FreeTextureFromEngineResourcePools(CTexture*) { }
CMovableResourceMemoryPool* CIEngine::GetLoadingMemoryPool() { return 0; }
CMovableResourceMemoryPool* CIEngine::GetAnimationMemoryPool() { return 0; }
CGPUScratchBufferTextureManager* CIEngine::GetScratchBufferTextureManager() { return 0; }
bool CIEngine::DeallocateShadowBuffer() { return false; }
void CIEngine::ReallocateShadowBuffer() { }
void CIEngine::Enable3DSceneRendering(bool) { }
bool CIEngine::Is3DSceneRenderingEnabled() { return false; }
void CIEngine::Enable2DSceneRendering(bool) { }
bool CIEngine::Is2DSceneRenderingEnabled() { return false; }
void CIEngine::OverrideRain(float) { }
void CIEngine::OverrideSnow(float) { }
void CIEngine::RenderColorFilteredQuad(float, float, const C4DVector&, float, float, CTexture*) { }
const CRGBFloatColour CIEngine::PeekGlobalDiffuseColour() const { return CRGBFloatColour(0.0f,0.0f,0.0f,1.0f); }
const CRGBFloatColour CIEngine::PeekGlobalAmbientColour() const { return CRGBFloatColour(0.0f,0.0f,0.0f,1.0f); }

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CInitBaseClass::CInitBaseClass() {}
CInitBaseClass::~CInitBaseClass() {}
CIEngine::~CIEngine() {}
CCharString::~CCharString() {}
long CMainGameComponent::ConstantFPS = 30;
const double TWO_PI = (double)6.2831855f;

static long Frame;
long __fastcall CWorld::GetFrame() { return Frame; }

CInterpolationInfo::CInterpolationInfo()
{
    memset(this, 0, sizeof(*this));
}

CEnginePrimitivePrepareInfo::CEnginePrimitivePrepareInfo(const CInterpolationInfo& interpolation, CIEngine& engine,
                                                         bool editor, const CCamera& camera)
    : Interpolate(interpolation), Engine(engine), Edit2DEngine(editor), Camera(camera)
{
}

class PyramidTestEngine : public CIEngine {
public:
    PyramidTestEngine() : LineCount(0), ValidLines(true) {}
    virtual void SetCamera(const CCamera*) {}
    virtual void SetWindow(const C2DBoxF*) {}
    virtual void Virtual08_00b27d90() {}
    virtual bool ConvertScreenPosToWorldPos(const C2DVector&, C3DVector&) const { return false; }
    virtual bool ConvertScreenPosToWorldPosAtViewDistance(float, const C2DVector&, C3DVector&) const { return false; }
    virtual bool ConvertWorldPosToScreenPos(const C3DVector&, C2DVector&) const { return false; }
    virtual bool PreChangeResolution() { return false; }
    virtual bool PostChangeResolution(unsigned long, unsigned long, unsigned long) { return true; }
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle&, const CEnginePrimitive&, unsigned long) {}

    virtual void AddPrimitive(const CEnginePrimitive& primitive)
    {
        if (primitive.Type != (EEnginePrimitiveType)6 || LineCount >= 12) {
            ValidLines = false;
            return;
        }
        const CEnginePrimitiveLine& line = static_cast<const CEnginePrimitiveLine&>(primitive);
        Starts[LineCount] = line.Start;
        Ends[LineCount] = line.End;
        Colours[LineCount] = line.Colour;
        ValidLines = ValidLines && line.Width == 0.05f && line.BankIndex == 0 && line.UseZBuffer
            && line.BlendMode == 0 && line.BlendOp == 0 && line.InterpolationSet == 0;
        ++LineCount;
    }
    unsigned int LineCount;
    bool ValidLines;
    C3DVector Starts[12];
    C3DVector Ends[12];
    CRGBColour Colours[12];
};

void CIEngine::AddPrimitive(const CEnginePrimitive&) {}

static bool near(float a, float b)
{
    return fabs(a-b) < 0.001f;
}
static bool MatchesPoint(const C3DVector& a, const C3DVector& b)
{
    return near(a.X, b.X) && near(a.Y, b.Y) && near(a.Z, b.Z);
}

int main()
{
    PyramidTestEngine engine;
    CInterpolationInfo interpolation;
    CCamera camera = CCamera();
    const CEnginePrimitivePrepareInfo info(interpolation, engine, false, camera);
    const C3DVector centre(1.0f, -2.0f, 6.0f);
    const float sizes[] = {2.0f, 4.0f, 0.0f};
    const long frames[] = {0, 135, 90};
    const float phases[][3] = { {0.0f, 1.0f/3.0f, 2.0f/3.0f}, {0.5f, 5.0f/6.0f, 1.0f/6.0f}, {0.0f, 1.0f/3.0f, 2.0f/3.0f} };
    for (unsigned int run = 0; run < 3; ++run) {
        Frame = frames[run];
        engine.LineCount = 0;
        engine.ValidLines = true;
        const float size = sizes[run];
        NCameraModes::DrawPyramid(info, centre, size);
        if (engine.LineCount != 12 || !engine.ValidLines)
            return 1;
        for (unsigned int ring = 0; ring < 3; ++ring) {
            const unsigned int first = ring*4;
            const float phase = phases[run][ring];
            const float z = centre.Z + phase * 1.5f * size;
            const float half_side = (1.05f - phase) * size;
            const float side = 2.0f * half_side;
            const double angle = (double)phase * TWO_PI;
            const double cosine = cos(angle);
            const double sine = sin(angle);
            const int x_sign[] = { -1, 1, 1, -1 };
            const int y_sign[] = { 1, 1, -1, -1 };
            for (unsigned int line = 0; line < 4; ++line) {
                const C3DVector& start = engine.Starts[first + line];
                const C3DVector& end = engine.Ends[first + line];
                const C3DVector delta = end - start;
                const float start_x = centre.X + (float)(half_side * (x_sign[line] * cosine + y_sign[line] * sine));
                const float start_y = centre.Y + (float)(half_side * (-x_sign[line] * sine + y_sign[line] * cosine));
                if (!near(start.X, start_x) || !near(start.Y, start_y) || !near(start.Z, z) || !MatchesPoint(end, engine.Starts[first + (line+1) % 4])
                    || !near(delta.X*delta.X + delta.Y*delta.Y, side*side))
                    return 2;
                if (size == 0.0f && !MatchesPoint(start, centre))
                    return 3;
                if (engine.Colours[first].GetR() != engine.Colours[first+line].GetR()
                    || engine.Colours[first].GetG() != engine.Colours[first+line].GetG()
                    || engine.Colours[first].GetB() != engine.Colours[first+line].GetB())
                    return 4;
            }
        }
        if (run == 0 && engine.Colours[0].GetR() != 255)
            return 5;
    }
    puts("008764e0_TEST PASS");
    return 0;
}