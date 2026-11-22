// 00b31160 CEngineCamera::ProcessDebugScript
// Leaf: ?ProcessDebugScript@CEngineCamera@@AAEXXZ
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
#define FABLE_RETAIL_CARRAY_INDEX_ASSERT
#define FABLE_RETAIL_GFGETINTERPOLATEDVALUE_PRODUCT_PASS26
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_CAMERASTATE
#define FABLE_RETAIL_CENGINECAMERA_DEBUGSCRIPT_PASS26
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CENGINECAMERADESC_DEFAULT_CTOR_PASS26
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_PEEKBACKBUFFERRESOLUTION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22
#include "engine/CEngineCamera.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/GFGetTime.h"
#include "engine/GFGetInterpolatedValue.h"

namespace NEngineInternal {
    extern CDisplayManager* GPDisplayManager;
}

const float ENGINE_NEAR_CLIP_PLANE = 0.1f;
const float ENGINE_FAR_CLIP_PLANE = 4000.0f;
extern const float ZBUFFER_PARTITION_MAIN_SCENE_START;
const float ZBUFFER_PARTITION_MAIN_SCENE_END = 0.99f;

// Fidelity: same-TU strong definition of 0x00B30B50 (retail TU-mate: engine_camera.obj defines it alongside ProcessDebugScript), the
// owner row's body verbatim. Declaration-only measured DIFFER for this leaf (886v886: the `mov byte ptr [esp+0ACh],0` / `fld [edi+18h]` pair at 0x00B313A4 swaps).
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