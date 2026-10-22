// CEngineScreenEffectRadialBlurRenderer::UpdateWorldPositionTracking() -- for each centre that tracks a 3D position (blur, then area), project the world position through the engine camera; a point behind the near clip fails the update, otherwise the centre becomes the screen position divided by the render-target dimensions
// Leaf: UpdateWorldPositionTracking
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B5C120, length 580 bytes (body 0x244), plain `ret`, returns bool.
// Callees: CDisplayManager::GetRenderTargetDimensions() const @ 0x009BEDC0 (C2DExtentsI by value, global @ 0x01436E1C),
//   called once per component.
// Camera: the engine camera @ 0x01436EA0 (engine/CEngineCamera.h: NearClip +0x6c (CameraDesc), ViewTransform +0x80,
//   HalfWindowWidth/Height +0xb8/+0xbc, WindowCenterX/Y +0xc8/+0xcc); the header keeps CameraDesc/ViewTransform opaque, so
//   the camera is a local view at the same offsets (landed 0x00B900B0 model). The projection is an inline
//   ProjectToScreen; each view coordinate is `x*R + y*U + z*F + P` (VC7.1 emits the z product first).
// Layout: engine/CEngineScreenEffectRadialBlurRenderer.h (BlurCenter +0x170, AreaCenter +0x178, BlurCenterWorldTrackPosition
//   +0x180, AreaCenterWorldTrackPosition +0x18c, AreaTrack3DPos +0x1c0, BlurTrack3DPos +0x1c1).
#include "engine/CEngineScreenEffectRadialBlurRenderer.h"

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct C2DVector { float X, Y; float _unused; };
struct CMatrix3x4 {
    C3DVector Right;                               // +0x00
    C3DVector Up;                                  // +0x0c
    C3DVector Forward;                             // +0x18
    C3DVector Pos;                                 // +0x24
};

__forceinline C3DVector Transform(const CMatrix3x4& v, const C3DVector& p)
{
    return C3DVector((p.Z * v.Forward.X + p.Y * v.Up.X) + p.X * v.Right.X + v.Pos.X,
                     (p.Z * v.Forward.Y + p.X * v.Right.Y) + p.Y * v.Up.Y + v.Pos.Y,
                     (p.Z * v.Forward.Z + p.X * v.Right.Z) + p.Y * v.Up.Z + v.Pos.Z);
}

struct CEngineCameraView {
    unsigned char _pad000[0x6c];
    float         NearClip;                        // +0x6c (CameraDesc +0x58)
    unsigned char _pad070[0x80 - 0x70];
    CMatrix3x4    ViewTransform;                   // +0x80
    unsigned char _pad0b0[0xb8 - 0xb0];
    float         HalfWindowWidth;                 // +0xb8
    float         HalfWindowHeight;                // +0xbc
    unsigned char _pad0c0[0xc8 - 0xc0];
    float         WindowCenterX;                   // +0xc8
    float         WindowCenterY;                   // +0xcc

    __forceinline bool ProjectToScreen(const C3DVector& p, C2DVector& out) const
    {
        C3DVector c = Transform(ViewTransform, p);
        if (c.Z < NearClip)
            return false;
        float inv = 1.0f / c.Z;
        out.X = c.X * (inv * HalfWindowWidth) + WindowCenterX;
        out.Y = WindowCenterY - c.Y * HalfWindowHeight * inv;
        return true;
    }
};
extern CEngineCameraView* g_pEngineCamera;          // 0x01436EA0

struct C2DExtentsI { long Width; long Height; };
struct CDisplayManager {
    C2DExtentsI GetRenderTargetDimensions() const; // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayManager;         // 0x01436E1C

struct CEngineScreenEffectRadialBlurRenderer_Methods : CEngineScreenEffectRadialBlurRenderer {
    bool UpdateWorldPositionTracking();
};

bool CEngineScreenEffectRadialBlurRenderer_Methods::UpdateWorldPositionTracking()
{
    if (BlurTrack3DPos) {
        C2DVector screen;
        if (!g_pEngineCamera->ProjectToScreen(*(const C3DVector*)&BlurCenterWorldTrackPosition_X, screen))
            return false;
        BlurCenter_X = screen.X / g_pDisplayManager->GetRenderTargetDimensions().Width;
        BlurCenter_Y = screen.Y / g_pDisplayManager->GetRenderTargetDimensions().Height;
    }
    if (AreaTrack3DPos) {
        C2DVector screen;
        if (!g_pEngineCamera->ProjectToScreen(*(const C3DVector*)&AreaCenterWorldTrackPosition_X, screen))
            return false;
        AreaCenter_X = screen.X / g_pDisplayManager->GetRenderTargetDimensions().Width;
        AreaCenter_Y = screen.Y / g_pDisplayManager->GetRenderTargetDimensions().Height;
    }
    return true;
}
