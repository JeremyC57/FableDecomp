// CGameCameraManager::Is2DAreaInViewingFrustrum(const C3DVector& centre, float radius) const -- sample the circle of the given radius around the centre (in the plane spanned by the camera's flattened forward and side directions) in tenth-turn steps and report whether any sample is inside the viewing frustum
// Grade: DIFFER(492v490) -- x87 stack allocation only: retail consumes the cos value in the forward.Z product (fxch ; fmul [z]), ours keeps it until after the radius multiply (fstp st(1))
// Leaf: Is2DAreaInViewingFrustrum
// Retail: Fable.exe (TLC, VC7.1) @ 0x00697F80, 488 bytes, `ret 8`.
// Callees: CCamera's near-clip accessor 0x00A0BEB0 (catalog label DrawGetSeaLevel), C3DVector::Normalise 0x00A14440,
//   CTools::IsPosInViewingFrustrum 0x00875120; constants 2*pi (double) 0x0124F2B8, 0.1f 0x0122EE58, 1.0f 0x0122DED8.
// Layout: local projection -- Camera +0x04, CamTools +0x78, the current camera mode pointer +0x98 (its focus +0x10 and
//   position +0x54).
#include <math.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    void Normalise();                                    // 0x00A14440
};
struct CCamera {
    unsigned char Data[0x38];
    float GetNearClip() const;                           // 0x00A0BEB0
};
struct CCameraModeBase {
    unsigned char Data[0x10];
    C3DVector     Focus;                                 // +0x10
    unsigned char Data2[0x38];
    C3DVector     Position;                              // +0x54
};
struct CTools {
    unsigned char Data[0x20];
    bool IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& focus, const C3DVector& camPos, float nearClip) const;   // 0x00875120
};
struct CGameCameraManager {
    void*            VTable;
    CCamera          Camera;                             // +0x04
    CCamera          OldCamera;                          // +0x3c
    void*            World;                              // +0x74
    CTools           CamTools;                           // +0x78
    CCameraModeBase* CurrentMode;                        // +0x98
    bool Is2DAreaInViewingFrustrum(const C3DVector& centre, float radius) const;
};

bool CGameCameraManager::Is2DAreaInViewingFrustrum(const C3DVector& centre, float radius) const
{
    float nearClip = Camera.GetNearClip();
    C3DVector camPos = CurrentMode->Position;
    C3DVector focus = CurrentMode->Focus;
    C3DVector forward(focus.X - camPos.X, focus.Y - camPos.Y, 0.0f);
    forward.Normalise();
    C3DVector side(forward.Y, -forward.X, 0.0f);
    side.Normalise();
    for (float t = 0.0f; t <= 1.0f; t += 0.1f) {
        double angle = t * 6.2831854820251465;
        C3DVector pos = centre + forward * (float)cos(angle) * radius + side * (float)sin(angle) * radius;
        if (CamTools.IsPosInViewingFrustrum(pos, focus, camPos, nearClip))
            return true;
    }
    return false;
}
