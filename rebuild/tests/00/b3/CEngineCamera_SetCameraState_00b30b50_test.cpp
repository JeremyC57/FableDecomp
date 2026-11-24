// Behaviour test for 0x00B30B50 CEngineCamera::SetCameraState: the row text verbatim (self-contained, /Od), members read through
// `private public`; SetupGamut / RefreshD3DTransforms and NEngineInternal::GPVideoOptions are controlled here.
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
// Inline-budget: retail expands both CMatrix3x4::SetInverse sites inside 00B30B50.
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
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_CAMERASTATE
#define FABLE_RETAIL_CENGINECAMERA_DEBUGSCRIPT_PASS26
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINECAMERADESC_DEFAULT_CTOR_PASS26
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#define private public
#include "engine/CEngineCamera.h"
#undef private
#include "engine/Direct3D9Types.h"

#include <cmath>
#include <cstdio>
#include <cstring>
static int errors = 0;
#define CHECK(x) do { if (!(x)) { ++errors; std::printf("FAIL %d %s\n", __LINE__, #x); } } while (0)
static bool Near(double a, double b) { return std::fabs(a - b) <= 1e-4 * (1.0 + std::fabs(b)); }

const CEngineVideoOptions* NEngineInternal::GPVideoOptions = 0;
static int gamut_calls = 0, refresh_calls = 0;
void CEngineCamera::SetupGamut() { ++gamut_calls; }
void CEngineCamera::RefreshD3DTransforms() { ++refresh_calls; }

// Inferred: grouping the two dimension results reproduces the native x87 scheduling.
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


static double storage[sizeof(CEngineCamera) / sizeof(double) + 1];
static double vstorage[sizeof(CEngineVideoOptions) / sizeof(double) + 1];

static CEngineCameraDesc MakeDesc()
{
    CEngineCameraDesc d;
    d.Window.TLX = 10; d.Window.TLY = 20; d.Window.BRX = 650; d.Window.BRY = 500;   // 640 x 480
    CMatrix3x4& r = d.RotationMatrix;
    r.E11 = 1; r.E12 = 0; r.E13 = 0; r.E21 = 0; r.E22 = 0; r.E23 = 1; r.E31 = 0; r.E32 = -1; r.E33 = 0;   // a rotation about X
    r.E41 = 7; r.E42 = 8; r.E43 = 9;   // overwritten by the leaf
    d.WorldPosition.X = 1; d.WorldPosition.Y = 2; d.WorldPosition.Z = 3;
    d.HorizontalFOV = 1.2f; d.VerticalFOV = 0.8f; d.Use2DFOV = false;
    d.NearZPlane = 0.5f; d.FarZPlane = 1000.0f; d.ZBufferStart = 0.0125f; d.ZBufferEnd = 0.99f;
    d.StateValid = true;
    return d;
}

static void CheckCommon(const CEngineCamera& c, const CEngineCameraDesc& d)
{
    CHECK(std::memcmp(&c.CameraDesc, &d, sizeof d) == 0);
    CHECK(c.WindowWidth == 640 && c.WindowHeight == 480 && c.HalfWindowWidth == 320 && c.HalfWindowHeight == 240);
    CHECK(Near(c.OneOverHalfWindowWidth, 1.0 / 320) && Near(c.OneOverHalfWindowHeight, 1.0 / 240));
    CHECK(c.WindowCenterX == 330 && c.WindowCenterY == 260);
    // View transform: rotation rows kept, translation = -(position . rotation column); unscaled copy and its inverse.
    const CMatrix3x4& n = c.NoScaleViewTransform;
    CHECK(n.E11 == 1 && n.E23 == 1 && n.E32 == -1 && n.E22 == 0);
    CHECK(Near(n.E41, -1) && Near(n.E42, 3) && Near(n.E43, -2));
    const CMatrix3x4& v = c.ViewTransform;
    const double sx = c.HomogenousViewScaleX, sy = c.HomogenousViewScaleY;
    CHECK(Near(v.E11, sx) && Near(v.E41, -sx) && Near(v.E32, -sy) && Near(v.E42, 3 * sy) && Near(v.E23, 1) && Near(v.E43, -2));
    // InverseNoScale * NoScale = identity on a point (row-vector convention: p' = p*R + T).
    const CMatrix3x4& in = c.InverseNoScaleViewTransform;
    const double px = 4, py = -5, pz = 6;
    const double qx = px * n.E11 + py * n.E21 + pz * n.E31 + n.E41, qy = px * n.E12 + py * n.E22 + pz * n.E32 + n.E42, qz = px * n.E13 + py * n.E23 + pz * n.E33 + n.E43;
    CHECK(Near(qx * in.E11 + qy * in.E21 + qz * in.E31 + in.E41, px) && Near(qx * in.E12 + qy * in.E22 + qz * in.E32 + in.E42, py) && Near(qx * in.E13 + qy * in.E23 + qz * in.E33 + in.E43, pz));
    const CMatrix3x4& iv = c.InverseViewTransform;
    const double wx = px * v.E11 + py * v.E21 + pz * v.E31 + v.E41, wy = px * v.E12 + py * v.E22 + pz * v.E32 + v.E42, wz = px * v.E13 + py * v.E23 + pz * v.E33 + v.E43;
    CHECK(Near(wx * iv.E11 + wy * iv.E21 + wz * iv.E31 + iv.E41, px) && Near(wx * iv.E12 + wy * iv.E22 + wz * iv.E32 + iv.E42, py) && Near(wx * iv.E13 + wy * iv.E23 + wz * iv.E33 + iv.E43, pz));
    const double factor = (d.ZBufferStart - d.ZBufferEnd) * (d.FarZPlane * d.NearZPlane) / (d.FarZPlane - d.NearZPlane);
    CHECK(Near(c.ZBufferFactor, factor) && Near(c.ZBufferOffset, d.ZBufferStart - factor / d.NearZPlane));
    const CMatrix4x4& p = c.ProjectionTransform;
    CHECK(p.E11 == 1 && p.E22 == 1 && p.E34 == 1 && p.E44 == 0 && p.E12 == 0 && p.E41 == 0);
    CHECK(p.E33 == c.ZBufferOffset && p.E43 == c.ZBufferFactor);
}

int main()
{
    CEngineCamera& cam = *reinterpret_cast<CEngineCamera*>(storage);
    // 1. StateValid false: nothing written, no callee.
    std::memset(storage, 0x5a, sizeof storage);
    CEngineCameraDesc d = MakeDesc(); d.StateValid = false;
    cam.SetCameraState(d, true, true);
    { bool untouched = true; const unsigned char* b = (const unsigned char*)storage; for (unsigned k = 0; k < sizeof storage; ++k) if (b[k] != 0x5a) untouched = false; CHECK(untouched); }
    CHECK(gamut_calls == 0 && refresh_calls == 0);

    // 2. No video options: the window's 4:3 aspect; the FOV factor 1 + (0.75 - 480/640) / 2 = 1.
    d = MakeDesc();
    cam.SetCameraState(d, true, false);
    CHECK(gamut_calls == 1 && refresh_calls == 0);
    CHECK(Near(cam.AspectRatio, 640.0 / 480));
    CHECK(Near(cam.HomogenousViewScaleX, 1.0 / std::tan(1.2 * 0.5)) && Near(cam.HomogenousViewScaleY, cam.HomogenousViewScaleX * (640.0 / 480)));
    CheckCommon(cam, d);

    // 3. Video options 16:9 override the window aspect: factor 1 + (0.75 - 9/16) / 2 = 1.09375.
    CEngineVideoOptions& vo = *reinterpret_cast<CEngineVideoOptions*>(vstorage);
    vo.ScreenAspectRatio.X = 16; vo.ScreenAspectRatio.Y = 9;
    NEngineInternal::GPVideoOptions = &vo;
    cam.SetCameraState(d, false, true);
    CHECK(gamut_calls == 1 && refresh_calls == 1);
    CHECK(Near(cam.AspectRatio, 16.0 / 9));
    CHECK(Near(cam.HomogenousViewScaleX, 1.0 / std::tan(1.2 * 1.09375 * 0.5)) && Near(cam.HomogenousViewScaleY, cam.HomogenousViewScaleX * (16.0 / 9)));
    CheckCommon(cam, d);

    // 4. A non-positive component is ignored: back to the window aspect.
    vo.ScreenAspectRatio.X = 16; vo.ScreenAspectRatio.Y = 0;
    cam.SetCameraState(d, false, false);
    CHECK(gamut_calls == 1 && refresh_calls == 1);
    CHECK(Near(cam.AspectRatio, 640.0 / 480) && Near(cam.HomogenousViewScaleX, 1.0 / std::tan(0.6)));

    // 5. Use2DFOV: both scales from their own FOVs (aspect still recorded, from the 16:9 options).
    vo.ScreenAspectRatio.Y = 9;
    d.Use2DFOV = true;
    cam.SetCameraState(d, true, true);
    CHECK(gamut_calls == 2 && refresh_calls == 2);
    CHECK(Near(cam.AspectRatio, 16.0 / 9));
    CHECK(Near(cam.HomogenousViewScaleX, 1.0 / std::tan(0.6)) && Near(cam.HomogenousViewScaleY, 1.0 / std::tan(0.4)));
    CheckCommon(cam, d);

    std::puts(errors ? "00b30b50_TEST FAIL" : "00b30b50_TEST PASS");
    return errors ? 1 : 0;
}