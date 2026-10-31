// CEngineCamera::SetCameraState(const CEngineCameraDesc& desc, bool refreshA, bool refreshB) -- PARTIAL (prefix + tail): when the description is valid copy it, derive the window size, half size, reciprocal half size, centre and aspect ratio (the video options' display size overrides the window size when both are positive) and, for a 2D field of view, the homogenous view scales; then run the two optional refreshes. The view / projection matrix block in between is not reconstructed yet.
// Grade: DIFFER(319v1499) 408 diff lines (parked skeleton 3v1499, 430 lines): PARTIAL -- the description copy (rep movsd 0x1b), window size / half / reciprocal / centre, the video-options aspect override, the 2D-FOV tan scales and the two tail refresh calls are written with the Ego_r.pdb CEngineCameraDesc; retail reads the window through the copied desc pointer (eax) and keeps the derived floats in frame copies, and the 1 KB view/projection matrix block (353 x87 ops) between the FOV and the tail is not reconstructed
// Leaf: ?SetCameraState@CEngineCamera@@QAEXABUCEngineCameraDesc@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B30B50, length 1499 bytes, `ret 0Ch`.
// Callees: 0x00B2FD60 and 0x00B2FC50 on this (names not recovered; one reloc spelling ?RefreshD3DTransforms@CameraMethods for 0x00B2FC50).
// Globals: video options 0x01436EB0 (display width / height +0x08 / +0x0c, three spellings in reloc_bindings). Floats: 0.5f 0x0122F59C, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout: engine/CEngineCamera.h (CameraDesc +0x14 as Ego_r.pdb CEngineCameraDesc 0x6c; WindowWidth +0xb0 .. HomogenousViewScaleY +0xd8).
#include <math.h>
#define CEngineCamera CEngineCameraLayout
#include "engine/CEngineCamera.h"
#undef CEngineCamera
class C2DVector { public: float X, Y; };
class C2DBoxF { public: C2DVector TopLeft, BottomRight; };
class C3DVector { public: float X, Y, Z; };
class CMatrix3x4 { public: float M[12]; };
struct CEngineCameraDesc {
    C2DBoxF    Window;                                  // +0x00
    CMatrix3x4 RotationMatrix;                          // +0x10
    C3DVector  WorldPosition;                           // +0x40
    float      HorizontalFOV;                           // +0x4c
    float      VerticalFOV;                             // +0x50
    bool       Use2DFOV;                                // +0x54
    float      NearZPlane, FarZPlane, ZBufferStart, ZBufferEnd;   // +0x58 .. +0x64
    bool       StateValid;                              // +0x68
};
class CEngineVideoOptions { public:
    unsigned char _pad00[8];
    float         DisplayWidth;                         // +0x08 (descriptive)
    float         DisplayHeight;                        // +0x0c (descriptive)
};
extern CEngineVideoOptions* g_pVideoOptions;            // 0x01436EB0
class CEngineCamera : public CEngineCameraLayout { public:
    CEngineCameraDesc& Desc() { return *(CEngineCameraDesc*)CameraDesc; }
    void RefreshA();                                    // 0x00B2FD60 (descriptive)
    void RefreshB();                                    // 0x00B2FC50 (descriptive)
    void SetCameraState(const CEngineCameraDesc& desc, bool refreshA, bool refreshB);
};

void CEngineCamera::SetCameraState(const CEngineCameraDesc& desc, bool refreshA, bool refreshB)
{
    if (!desc.StateValid)
        return;
    Desc() = desc;
    WindowWidth = Desc().Window.BottomRight.X - Desc().Window.TopLeft.X;
    WindowHeight = Desc().Window.BottomRight.Y - Desc().Window.TopLeft.Y;
    float height = WindowHeight;
    HalfWindowWidth = WindowWidth * 0.5f;
    HalfWindowHeight = WindowHeight * 0.5f;
    OneOverHalfWindowWidth = 1.0f / HalfWindowWidth;
    OneOverHalfWindowHeight = 1.0f / HalfWindowHeight;
    float width = WindowWidth;
    WindowCenterX = HalfWindowWidth + Desc().Window.TopLeft.X;
    WindowCenterY = HalfWindowHeight + Desc().Window.TopLeft.Y;
    float horizontalFOV = desc.HorizontalFOV;
    if (g_pVideoOptions && g_pVideoOptions->DisplayWidth > 0.0f && g_pVideoOptions->DisplayHeight > 0.0f) {
        width = g_pVideoOptions->DisplayWidth;
        height = g_pVideoOptions->DisplayHeight;
    }
    AspectRatio = width / height;
    if (desc.Use2DFOV) {
        HomogenousViewScaleX = 1.0f / (float)tan(desc.HorizontalFOV * 0.5f);
        HomogenousViewScaleY = 1.0f / (float)tan(desc.VerticalFOV * 0.5f);
    }
    if (refreshA)
        RefreshA();
    if (refreshB)
        RefreshB();
}
