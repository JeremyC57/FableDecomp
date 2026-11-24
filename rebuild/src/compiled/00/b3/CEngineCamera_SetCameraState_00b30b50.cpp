// CEngineCamera::SetCameraState(const CEngineCameraDesc&, bool rebuild_gamut, bool refresh_d3d) -- when desc.StateValid, copy desc into
// CameraDesc and derive the window size / half size / reciprocals / centre, the aspect ratio (the video options' ScreenAspectRatio when both
// components are positive, else the window's) and the FOV-corrected homogenous view scales, build the view transform from the rotation and
// world position (unscaled and scaled copies and both inverses), the z-buffer factor / offset and the projection matrix; then SetupGamut and
// RefreshD3DTransforms when asked.
// Retail: Fable.exe 0x00B30B50, 1499 bytes of code (catalog span 1552), __fastcall, `ret 0Ch`, frame 8, no EH frame; the three tan calls are
// inline `fptan` (#pragma intrinsic(tan)). engine_camera.obj TU: 0x00B2FC50 RefreshD3DTransforms, 0x00B2FD60 SetupGamut, ... this row,
// 0x00B31160 ProcessDebugScript (it carries this body as a same-TU strong TU-mate copy).
// Callees: 0x00B2FD60 ?SetupGamut@CEngineCamera@@AAEXXZ (call at 0x00B3110F), 0x00B2FC50 ?RefreshD3DTransforms@CEngineCamera@@QAEXXZ
// (0x00B3111E). Data: NEngineInternal::GPVideoOptions 0x01436EB0; 0.5f 0x0122F59C, 1.0f 0x0122DED8, 0.0f 0x0122DEDC, 0.75f 0x01238174.
// Identity: Ego_r.pdb and FableWin.pdb ?SetCameraState@CEngineCamera@@QAEXABUCEngineCameraDesc@@_N1@Z (exact string in both name lists).
// Text: Codex pass 26 (in 0x00B31160's TU), verbatim; header opt-ins as that landed row.
// Leaf: ?SetCameraState@CEngineCamera@@QAEXABUCEngineCameraDesc@@_N1@Z
// Flags: (none)
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
#include "engine/CEngineCamera.h"
#include "engine/Direct3D9Types.h"

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
