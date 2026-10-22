// Grade: DIFFER(693v693) -- one scheduling slot at the first IsPosInViewingFrustrum call (retail interleaves fld/fmul of perp.y before push ebp); everything else matches
// CGameCameraManager::IsVolumeInViewingFrustrum(const C3DVector& pos, float radius) const -- true when the four points pos +- radius along two axes perpendicular to the view direction all lie in the camera's viewing cone
// Leaf: IsVolumeInViewingFrustrum
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    void Normalise();                                  // 0x00A14440
};
static inline C3DVector CrossProduct(const C3DVector& a, const C3DVector& b)
{
    return C3DVector(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - b.Z * a.X, a.X * b.Y - a.Y * b.X);
}
struct CCamera {
    float GetHorizontalFOV() const;                    // 0x00A0BEB0
};
struct CCameraModeBase {                               // NCameraModes::CBase
    unsigned char _pad[0x10];
    C3DVector     LookAt;                              // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                            // +0x54
};
struct CTools {
    bool IsPosInViewingFrustrum(const C3DVector& pos, const C3DVector& lookAt, const C3DVector& eye, float fov);   // 0x00875120
};
struct CGameCameraManager {
    void*            __vftable;
    CCamera          Camera;                           // +0x04
    unsigned char    _pad05[0x73];
    CTools           CamTools;                         // +0x78
    unsigned char    _pad79[0x1f];
    CCameraModeBase* CurrentMode;                      // +0x98
    bool IsVolumeInViewingFrustrum(const C3DVector& pos, float radius);
};

bool CGameCameraManager::IsVolumeInViewingFrustrum(const C3DVector& pos, float radius)
{
    float fov = Camera.GetHorizontalFOV();
    C3DVector eye = CurrentMode->Position;
    C3DVector lookAt = CurrentMode->LookAt;
    C3DVector dir = lookAt - eye;
    C3DVector perp;
    if ((float)fabs(dir.Y) > 0.0001f) {
        perp.X = 1.0f;
        perp.Z = 1.0f;
        perp.Y = -((dir.X + dir.Z) / dir.Y);
    } else if ((float)fabs(dir.Z) > 0.0001f) {
        perp.X = 1.0f;
        perp.Y = 1.0f;
        perp.Z = -((dir.X + dir.Y) / dir.Z);
    } else {
        return false;
    }
    perp.Normalise();
    C3DVector up = CrossProduct(perp, dir);
    up.Normalise();
    bool side = CamTools.IsPosInViewingFrustrum(pos + perp * radius, lookAt, eye, fov) && CamTools.IsPosInViewingFrustrum(pos - perp * radius, lookAt, eye, fov);
    bool vert = CamTools.IsPosInViewingFrustrum(pos + up * radius, lookAt, eye, fov) && CamTools.IsPosInViewingFrustrum(pos - up * radius, lookAt, eye, fov);
    return side && vert;
}
