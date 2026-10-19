// CGameCameraBase::GetRenderCamera(float interpolant) const -- the camera to render this frame: the current camera
// Grade: DIFFER(212v212) -- per-component Lerp lines on the member paths fix the Y/Z addressing ([esi+8]/[esi+0Ch]); left: the copy store into camera.WorldPosition is scheduled before the fstp st(0) pop and the lea/push of the PointAt argument move (scheduling)
// moved to the position interpolated from the last frame's camera and pointed at the interpolated look-at point
// Leaf: ?GetRenderCamera@CGameCameraBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FD5E0, length 224 bytes, `ret 8` (hidden result, interpolant).
// Callees: CCamera::CCamera(const CCamera&) @ 0x0049D380, CCamera::PointAt(const C3DVector&) @ 0x00A0C2B0.
// Layout (engine/CGameCameraBase.h agrees: InternalCam +0x04, LastInternalCam +0x3C, LastPointingAt +0x74,
// PointingAt +0x80; the header flattens the vptr and the CCamera members to bytes, so the class is modelled locally
// with those names to declare the virtual and reach CCamera::WorldPosition). CCamera (PDB, 0x38 bytes):
// WorldPosition +0, RHSet +0xC, locks, FOVFlags, FOVs, Zoom.
// Shape: a scalar Lerp over by-value floats (the start value stays in st(1)) inside an inline vector Lerp that builds a named C3DVector member by member (a + (b - a) * t, X then Y then Z) and returns it
// through C3DVector's memberwise copy ctor, so each component is stored and copied before the next is computed; the
// local camera is returned by copy construction (no NRVO).

inline float Lerp(float from, float to, float t) { return from + (to - from) * t; }

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    void SetInterpolated(const C3DVector& from, const C3DVector& to, float t)
    {
        X = Lerp(from.X, to.X, t);
        Y = Lerp(from.Y, to.Y, t);
        Z = Lerp(from.Z, to.Z, t);
    }
};


class CCamera {
public:
    C3DVector WorldPosition;              // +0x00
    unsigned char RHSet[0x18];            // +0x0C CRightHandedSet
    bool HeightLocked;                    // +0x24
    bool ViewVecZLocked;                  // +0x25
    unsigned long FOVFlags;               // +0x28
    float HorizontalFOV;                  // +0x2C
    float VerticalFOV;                    // +0x30
    float Zoom;                           // +0x34
    CCamera(const CCamera& other);        // 0x0049D380
    void PointAt(const C3DVector& point); // 0x00A0C2B0
};

class CGameCameraBase {
public:
    virtual CCamera GetRenderCamera(float interpolant) const;
    CCamera InternalCam;                  // +0x04
    CCamera LastInternalCam;              // +0x3C
    C3DVector LastPointingAt;             // +0x74
    C3DVector PointingAt;                 // +0x80
};

CCamera CGameCameraBase::GetRenderCamera(float interpolant) const
{
    CCamera camera(InternalCam);
    C3DVector v;
    v.X = Lerp(LastInternalCam.WorldPosition.X, InternalCam.WorldPosition.X, interpolant);
    v.Y = Lerp(LastInternalCam.WorldPosition.Y, InternalCam.WorldPosition.Y, interpolant);
    v.Z = Lerp(LastInternalCam.WorldPosition.Z, InternalCam.WorldPosition.Z, interpolant);
    camera.WorldPosition = v;
    v.SetInterpolated(LastPointingAt, PointingAt, interpolant);
    camera.PointAt(v);
    return camera;
}
