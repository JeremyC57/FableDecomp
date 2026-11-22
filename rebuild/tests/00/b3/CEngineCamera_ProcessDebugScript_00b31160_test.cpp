#include <cmath>
#ifndef _MSC_VER
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_STLP_ISO_SCALAR_TRAITS_PASS26
#define __fastcall
#define __cdecl
#endif
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
// Inline-budget: retail expands the two full inverse operations at 00B30D51 and in the following matrix block.
#define FABLE_RETAIL_CMATRIX3X4_SETINVERSE_DONOR_PASS26
#define FABLE_RETAIL_CENGINECAMERA_SETSTATE_CONTEXT_PASS26
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_POSTSCALE3
#define FABLE_RETAIL_CMATRIX3X4_BILLBOARD
#define FABLE_RETAIL_CMATRIX4X4_CAMERA_INITIALISE_PASS26
#define FABLE_RETAIL_C2DBOXF
#include "engine/CEngineVideoOptions.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(tan)
#endif
namespace NEngineInternal { extern const CEngineVideoOptions* GPVideoOptions; }

#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#define FABLE_RETAIL_GFGETINTERPOLATEDVALUE_PRODUCT_PASS26
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINECAMERADESC_DEFAULT_CTOR_PASS26
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#include "engine/CArray.h"
#include "engine/C2DExtentsI.h"
#include "engine/CEngineCameraDesc.h"
#include "engine/CMatrix4x4.h"
#include "engine/GFGetInterpolatedValue.h"
#include "engine/GFGetTime.h"
#include <cstdio>

static unsigned failures;
#define CHECK(condition) do { if (!(condition)) { \
    std::printf("line %d: %s\n", __LINE__, #condition); ++failures; \
} } while (0)

static bool Near(float a, float b)
{
    return std::fabs(a - b) <= 0.00001f;
}

struct FixtureDisplayManager {
    C2DExtentsI resolution;
    const C2DExtentsI& PeekBackBufferResolution() const { return resolution; }
};

namespace NEngineInternal {
    FixtureDisplayManager* GPDisplayManager;
}

const float ENGINE_NEAR_CLIP_PLANE = 0.1f;
const float ENGINE_FAR_CLIP_PLANE = 4000.0f;
const float ZBUFFER_PARTITION_MAIN_SCENE_START = 0.0125f;
const float ZBUFFER_PARTITION_MAIN_SCENE_END = 0.99f;

static double clockTime;
static unsigned clockReads;
static const double* clockSequence;
static unsigned clockCount;
double FABLE_FASTCALL GFGetTime()
{
    const unsigned read = clockReads++;
    if (clockSequence) {
        CHECK(read < clockCount);
        if (read < clockCount)
            return clockSequence[read];
    }
    return clockTime;
}

static unsigned normalisations;
C3DVector& C3DVector::AccurateNormalise()
{
    ++normalisations;
    const double length = std::sqrt((double)X * X + (double)Y * Y + (double)Z * Z);
    X = (float)(X / length);
    Y = (float)(Y / length);
    Z = (float)(Z / length);
    return *this;
}

class FixtureCamera {
public:
    struct CDebugScriptElement {
        C3DVector WorldPos, LookVector;
        float FOV, TimeToNextElement;
    };

    CArray<CDebugScriptElement> DebugScript;
    long CurrentDebugScriptElement;
    double DebugScriptElementStartTime;
    long ConsoleOverrideViewPortWidth, ConsoleOverrideViewPortHeight;
    CEngineCameraDesc observed;
    unsigned applications;

    void ProcessDebugScript();
    void SetCameraState(const CEngineCameraDesc& desc, bool updateGamut, bool updateTransforms)
    {
        CHECK(updateGamut && updateTransforms);
        CHECK(!desc.StateValid && !desc.Use2DFOV);
        CHECK(normalisations == 3);
        // The leaf does not initialize VerticalFOV. The callee sees Use2DFOV false.
        observed.Window = desc.Window;
        observed.RotationMatrix = desc.RotationMatrix;
        observed.WorldPosition = desc.WorldPosition;
        observed.HorizontalFOV = desc.HorizontalFOV;
        observed.NearZPlane = desc.NearZPlane;
        observed.FarZPlane = desc.FarZPlane;
        observed.ZBufferStart = desc.ZBufferStart;
        observed.ZBufferEnd = desc.ZBufferEnd;
        ++applications;
    }
};

#define CEngineCamera FixtureCamera
void CEngineCamera::ProcessDebugScript()
{
    CEngineCameraDesc desc;
    desc.NearZPlane = ENGINE_NEAR_CLIP_PLANE;
    desc.FarZPlane = ENGINE_FAR_CLIP_PLANE;
    desc.ZBufferStart = ZBUFFER_PARTITION_MAIN_SCENE_START;
    desc.ZBufferEnd = ZBUFFER_PARTITION_MAIN_SCENE_END;
    desc.Window.TLX = 0.0f;
    desc.Window.TLY = 0.0f;
    if (ConsoleOverrideViewPortWidth > 0) {
        desc.Window.BRX = (float)ConsoleOverrideViewPortWidth;
        desc.Window.BRY = (float)ConsoleOverrideViewPortHeight;
    }
    else {
        desc.Window.BRX = (float)NEngineInternal::GPDisplayManager->PeekBackBufferResolution().X;
        desc.Window.BRY = (float)NEngineInternal::GPDisplayManager->PeekBackBufferResolution().Y;
    }
    CDebugScriptElement* element;
    CDebugScriptElement* next_element;
    float interp;
    for (;;) {
        element = &DebugScript[CurrentDebugScriptElement];
        next_element = &DebugScript[(CurrentDebugScriptElement + 1) % DebugScript.size()];
        interp = (float)((GFGetTime() - DebugScriptElementStartTime) / element->TimeToNextElement);
        if (interp < 1.0f)
            break;
        DebugScriptElementStartTime += element->TimeToNextElement;
        CurrentDebugScriptElement = (CurrentDebugScriptElement + 1) % DebugScript.size();
    }
    desc.WorldPosition = element->WorldPos * (1.0f - interp) + next_element->WorldPos * interp;
    C3DVector look_vector = element->LookVector * (1.0f - interp) + next_element->LookVector * interp;
    look_vector.AccurateNormalise();
    desc.HorizontalFOV = GFGetInterpolatedValue(element->FOV, next_element->FOV, interp);
    desc.Use2DFOV = false;
    C3DVector side_vector = look_vector ^ C3DVector(0.0f, 0.0f, 1.0f);
    side_vector.AccurateNormalise();
    C3DVector up_vector = side_vector ^ look_vector;
    up_vector.AccurateNormalise();
    desc.RotationMatrix.E11 = side_vector.X;
    desc.RotationMatrix.E21 = side_vector.Y;
    desc.RotationMatrix.E31 = side_vector.Z;
    desc.RotationMatrix.E12 = up_vector.X;
    desc.RotationMatrix.E22 = up_vector.Y;
    desc.RotationMatrix.E32 = up_vector.Z;
    desc.RotationMatrix.E13 = look_vector.X;
    desc.RotationMatrix.E23 = look_vector.Y;
    desc.RotationMatrix.E33 = look_vector.Z;
    desc.RotationMatrix.E41 = 0.0f;
    desc.RotationMatrix.E42 = 0.0f;
    desc.RotationMatrix.E43 = 0.0f;
    SetCameraState(desc, true, true);
}
#undef CEngineCamera

static void CheckOutput(const CEngineCameraDesc& desc, const FixtureCamera::CDebugScriptElement& a,
                        const FixtureCamera::CDebugScriptElement& b, float interp, float width, float height)
{
    CHECK(desc.Window.TLX == 0.0f && desc.Window.TLY == 0.0f);
    CHECK(desc.Window.BRX == width && desc.Window.BRY == height);
    CHECK(desc.NearZPlane == 0.1f && desc.FarZPlane == 4000.0f);
    CHECK(desc.ZBufferStart == 0.0125f && desc.ZBufferEnd == 0.99f);
    CHECK(Near(desc.HorizontalFOV, (1.0f - interp) * a.FOV + interp * b.FOV));
    CHECK(Near(desc.WorldPosition.X, (1.0f - interp) * a.WorldPos.X + interp * b.WorldPos.X));
    CHECK(Near(desc.WorldPosition.Y, (1.0f - interp) * a.WorldPos.Y + interp * b.WorldPos.Y));
    CHECK(Near(desc.WorldPosition.Z, (1.0f - interp) * a.WorldPos.Z + interp * b.WorldPos.Z));

    const double x = (1.0 - interp) * a.LookVector.X + interp * b.LookVector.X;
    const double y = (1.0 - interp) * a.LookVector.Y + interp * b.LookVector.Y;
    const double z = (1.0 - interp) * a.LookVector.Z + interp * b.LookVector.Z;
    const double horizontal = std::sqrt(x*x + y*y);
    const double length = std::sqrt(x*x + y*y + z*z);
    const CMatrix3x4& m = desc.RotationMatrix;
    CHECK(Near(m.E11, (float)(y / horizontal)) && Near(m.E21, (float)(-x / horizontal)) && Near(m.E31, 0.0f));
    CHECK(Near(m.E12, (float)(-x*z / (horizontal*length))));
    CHECK(Near(m.E22, (float)(-y*z / (horizontal*length))));
    CHECK(Near(m.E32, (float)(horizontal / length)));
    CHECK(Near(m.E13, (float)(x / length)));
    CHECK(Near(m.E23, (float)(y / length)));
    CHECK(Near(m.E33, (float)(z / length)));
    CHECK(m.E41 == 0.0f && m.E42 == 0.0f && m.E43 == 0.0f);
}


namespace NEngineInternal { const CEngineVideoOptions* GPVideoOptions; }

struct FixtureStateOwner {
    CEngineCameraDesc CameraDesc;
    CMatrix3x4 ViewTransform, NoScaleViewTransform, InverseViewTransform, InverseNoScaleViewTransform;
    CMatrix4x4 ProjectionTransform;
    float WindowWidth, WindowHeight, HalfWindowWidth, HalfWindowHeight;
    float OneOverHalfWindowWidth, OneOverHalfWindowHeight, WindowCenterX, WindowCenterY;
    float AspectRatio, HomogenousViewScaleX, HomogenousViewScaleY, ZBufferFactor, ZBufferOffset;
    unsigned gamutCalls, refreshCalls;
    void SetupGamut() { ++gamutCalls; }
    void RefreshD3DTransforms() { ++refreshCalls; }
    void SetCameraState(const CEngineCameraDesc&, bool, bool);
};
#define CEngineCamera FixtureStateOwner
void CEngineCamera::SetCameraState(const CEngineCameraDesc& desc, bool rebuild_gamut, bool refresh_d3d)
{
    if (!desc.StateValid)
        return;
    CameraDesc = desc;
    WindowWidth = (CameraDesc.Window.GetWidth());
    WindowHeight = (CameraDesc.Window.GetHeight());
    HalfWindowWidth = WindowWidth * 0.5f;
    HalfWindowHeight = WindowHeight * 0.5f;
    OneOverHalfWindowWidth = 1.0f / HalfWindowWidth;
    OneOverHalfWindowHeight = 1.0f / HalfWindowHeight;
    WindowCenterX = CameraDesc.Window.TLX + HalfWindowWidth;
    WindowCenterY = CameraDesc.Window.TLY + HalfWindowHeight;
    C2DExtentsF aspect_size(WindowWidth, WindowHeight);
    float horiz_fov = desc.HorizontalFOV;
    if (NEngineInternal::GPVideoOptions &&
        NEngineInternal::GPVideoOptions->ScreenAspectRatio.X > 0.0f &&
        NEngineInternal::GPVideoOptions->ScreenAspectRatio.Y > 0.0f)
        aspect_size = NEngineInternal::GPVideoOptions->ScreenAspectRatio;
    horiz_fov *= 1.0f + (0.75f - aspect_size.Y / aspect_size.X) * 0.5f;
    AspectRatio = aspect_size.X / aspect_size.Y;
    if (desc.Use2DFOV) {
        HomogenousViewScaleX = 1.0f / (float)tan(desc.HorizontalFOV * 0.5f);
        HomogenousViewScaleY = 1.0f / (float)tan(desc.VerticalFOV * 0.5f);
    } else {
        HomogenousViewScaleX = 1.0f / (float)tan(horiz_fov * 0.5f);
        HomogenousViewScaleY = HomogenousViewScaleX * AspectRatio;
    }
    ViewTransform = desc.RotationMatrix;
    ViewTransform.E41 = -(desc.WorldPosition.X * ViewTransform.E11 + desc.WorldPosition.Y * ViewTransform.E21 + desc.WorldPosition.Z * ViewTransform.E31);
    ViewTransform.E42 = -(desc.WorldPosition.X * ViewTransform.E12 + desc.WorldPosition.Y * ViewTransform.E22 + desc.WorldPosition.Z * ViewTransform.E32);
    ViewTransform.E43 = -(desc.WorldPosition.X * ViewTransform.E13 + desc.WorldPosition.Y * ViewTransform.E23 + desc.WorldPosition.Z * ViewTransform.E33);
    NoScaleViewTransform = ViewTransform;
    ViewTransform.PostScale(HomogenousViewScaleX, HomogenousViewScaleY, 1.0f);
    InverseViewTransform.SetInverse(ViewTransform);
    InverseNoScaleViewTransform.SetInverse(NoScaleViewTransform);
    ZBufferFactor = (desc.ZBufferStart - desc.ZBufferEnd) * (desc.FarZPlane * desc.NearZPlane) / (desc.FarZPlane - desc.NearZPlane);
    ZBufferOffset = desc.ZBufferStart - ZBufferFactor / desc.NearZPlane;
    ProjectionTransform.Initialise(1.0f, 0.0f, 0.0f, 0.0f,
                                  0.0f, 1.0f, 0.0f, 0.0f,
                                  0.0f, 0.0f, ZBufferOffset, 1.0f,
                                  0.0f, 0.0f, ZBufferFactor, 0.0f);
    if (rebuild_gamut)
        SetupGamut();
    if (refresh_d3d)
        RefreshD3DTransforms();
}
#undef CEngineCamera

static void VerifyInverse(const CMatrix3x4& forward, const CMatrix3x4& inverse)
{
    const float points[][3] = { {0,0,0}, {1,0,0}, {0,1,0}, {0,0,1}, {2,-4,8} };
    for (unsigned i = 0; i < 5; ++i) {
        const float* p = points[i];
        const float x = forward.E11 * p[0] + forward.E21 * p[1] + forward.E31 * p[2] + forward.E41;
        const float y = forward.E12 * p[0] + forward.E22 * p[1] + forward.E32 * p[2] + forward.E42;
        const float z = forward.E13 * p[0] + forward.E23 * p[1] + forward.E33 * p[2] + forward.E43;
        CHECK(Near(x*inverse.E11+y*inverse.E21+z*inverse.E31+inverse.E41,p[0]));
        CHECK(Near(x*inverse.E12+y*inverse.E22+z*inverse.E32+inverse.E42,p[1]));
        CHECK(Near(x*inverse.E13+y*inverse.E23+z*inverse.E33+inverse.E43,p[2]));
    }
}

static void CheckCameraStates()
{
    CEngineVideoOptions options;
    for (unsigned valid=0; valid<2; ++valid)
    for (unsigned twoDimensional=0; twoDimensional<2; ++twoDimensional)
    for (unsigned aspect=0; aspect<6; ++aspect)
    for (unsigned flags=0; flags<4; ++flags) {
        FixtureStateOwner camera;
        camera.gamutCalls=0; camera.refreshCalls=0; camera.WindowWidth=-7.0f;
        camera.CameraDesc.StateValid=false;
        CEngineCameraDesc state;
        state.StateValid=valid!=0; state.Use2DFOV=twoDimensional!=0;
        state.HorizontalFOV=1.0f; state.VerticalFOV=0.5f;
        state.Window.TLX=8; state.Window.TLY=16; state.Window.BRX=136; state.Window.BRY=80;
        state.WorldPosition=C3DVector(2,-3,4);
        CMatrix3x4& m=state.RotationMatrix;
        m.E11=0; m.E12=1; m.E13=0; m.E21=-1; m.E22=0; m.E23=0;
        m.E31=0; m.E32=0; m.E33=1; m.E41=0; m.E42=0; m.E43=0;
        state.NearZPlane=1.0f; state.FarZPlane=8.0f;
        state.ZBufferStart=0.125f; state.ZBufferEnd=0.875f;
        options.ScreenAspectRatio.X=aspect==2?0.0f:(aspect==3?-1.0f:4.0f);
        options.ScreenAspectRatio.Y=aspect==4?0.0f:(aspect==5?-1.0f:3.0f);
        NEngineInternal::GPVideoOptions=aspect?&options:0;
        camera.SetCameraState(state,(flags&1)!=0,(flags&2)!=0);
        CHECK(camera.gamutCalls==((valid&&(flags&1))?1U:0U));
        CHECK(camera.refreshCalls==((valid&&(flags&2))?1U:0U));
        if (!valid) {
            CHECK(camera.WindowWidth==-7 && !camera.CameraDesc.StateValid);
            continue;
        }
        CHECK(camera.CameraDesc.StateValid && camera.CameraDesc.Use2DFOV==state.Use2DFOV);
        CHECK(camera.CameraDesc.Window.TLX==8 && camera.CameraDesc.WorldPosition.Y==-3);
        CHECK(camera.WindowWidth==128 && camera.WindowHeight==64);
        CHECK(camera.HalfWindowWidth==64 && camera.HalfWindowHeight==32);
        CHECK(camera.OneOverHalfWindowWidth==1.0f/64 && camera.OneOverHalfWindowHeight==1.0f/32);
        CHECK(camera.WindowCenterX==72 && camera.WindowCenterY==48);
        const double ratio=aspect==1 ? 4.0/3.0 : 2.0;
        const double halfAngle=twoDimensional ? 0.5 : (1.0+(0.75-1.0/ratio)*0.5)*0.5;
        const double scaleX=1.0/std::tan(halfAngle);
        const double scaleY=twoDimensional ? 1.0/std::tan(0.25) : scaleX*ratio;
        CHECK(Near(camera.AspectRatio,(float)ratio));
        CHECK(Near(camera.HomogenousViewScaleX,(float)scaleX));
        CHECK(Near(camera.HomogenousViewScaleY,(float)scaleY));
        CHECK(Near(camera.ViewTransform.E41,(float)(-3.0*scaleX)));
        CHECK(Near(camera.ViewTransform.E42,(float)(-2.0*scaleY)));
        CHECK(camera.ViewTransform.E43==-4);
        VerifyInverse(camera.NoScaleViewTransform,camera.InverseNoScaleViewTransform);
        VerifyInverse(camera.ViewTransform,camera.InverseViewTransform);
        CHECK(Near(camera.ZBufferOffset+camera.ZBufferFactor/state.NearZPlane,state.ZBufferStart));
        CHECK(Near(camera.ZBufferOffset+camera.ZBufferFactor/state.FarZPlane,state.ZBufferEnd));
        const CMatrix4x4& proj=camera.ProjectionTransform;
        CHECK(proj.E11==1 && proj.E22==1 && proj.E34==1 && proj.E44==0);
        CHECK(proj.E12==0 && proj.E13==0 && proj.E14==0 && proj.E21==0 && proj.E23==0 && proj.E24==0);
        CHECK(proj.E31==0 && proj.E32==0 && proj.E41==0 && proj.E42==0);
        CHECK(proj.E33==camera.ZBufferOffset && proj.E43==camera.ZBufferFactor);
    }
    NEngineInternal::GPVideoOptions=0;
}

int main()
{
    CheckCameraStates();
    FixtureDisplayManager display;
    display.resolution = C2DExtentsI(1600, 900);

    const double advance[] = {12.0, 14.25};
    const double rewind[] = {13.0, 11.25};
    const double wrap[] = {12.0, 15.0, 19.0, 20.0};
    const struct Scenario {
        unsigned count;
        long initialIndex, width;
        double now, expectedStart;
        long expectedIndex;
        unsigned reads;
        float interp;
        const double* sequence;
    } scenarios[] = {
        {3, 0, 101,  9.5, 10.0, 0, 1, -0.25f},
        {3, 0, 101, 10.0, 10.0, 0, 1,  0.0f},
        {3, 0,   0, 11.0, 10.0, 0, 1,  0.5f},
        {3, 0,  -1, 12.0, 12.0, 1, 2,  0.0f},
        {3, 0,   0, 15.5, 15.0, 2, 3,  0.125f},
        {3, 2, 101, 14.0, 14.0, 0, 2,  0.0f},
        {3, 0, 101, 30.0, 30.0, 1, 8,  0.0f},
        {1, 0,   0, 14.5, 14.0, 0, 3,  0.25f},
        {3, 0,   0, 14.25, 12.0, 1, 2,  0.75f, advance},
        {3, 0, 101, 11.25, 12.0, 1, 2, -0.25f, rewind},
        {3, 0, 101, 20.0, 19.0, 0, 4, 0.5f, wrap},
    };

    for (unsigned i = 0; i != sizeof(scenarios) / sizeof(scenarios[0]); ++i) {
        const Scenario& c = scenarios[i];
        FixtureCamera camera;
        for (unsigned key = 0; key != c.count; ++key) {
            FixtureCamera::CDebugScriptElement element;
            element.WorldPos = C3DVector((float)(key*10), (float)(9-key*2), (float)key);
            element.LookVector = C3DVector((float)(key+1), (float)(5-key), (float)(2*key-1.0));
            element.FOV = 1.0f + (float)key * 0.25f;
            element.TimeToNextElement = (float)(key+2);
            camera.DebugScript.push_back(element);
        }
        camera.CurrentDebugScriptElement = c.initialIndex;
        camera.DebugScriptElementStartTime = 10.0;
        camera.ConsoleOverrideViewPortWidth = c.width;
        camera.ConsoleOverrideViewPortHeight = 181;
        camera.applications = 0;
        NEngineInternal::GPDisplayManager = c.width > 0 ? 0 : &display;
        clockTime = c.now;
        clockSequence = c.sequence;
        clockCount = c.reads;
        normalisations = clockReads = 0;

        camera.ProcessDebugScript();

        CHECK(camera.applications == 1 && clockReads == c.reads);
        CHECK(camera.CurrentDebugScriptElement == c.expectedIndex);
        CHECK(camera.DebugScriptElementStartTime == c.expectedStart);
        CheckOutput(camera.observed, camera.DebugScript[c.expectedIndex],
                    camera.DebugScript[(c.expectedIndex + 1) % c.count], c.interp,
                    c.width > 0 ? (float)c.width : 1600.0f, c.width > 0 ? 181.0f : 900.0f);
    }
    std::puts(failures ? "00b31160_TEST FAIL" : "00b31160_TEST PASS");
    return failures ? 1 : 0;
}