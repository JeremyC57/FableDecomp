// CGameCameraManager::GetInterpolatedCamera(float t, const CCamera& from, const CCamera& to) const -- a copy of `from` whose field of view, position and view vectors are blended from `to` towards `from` by t
// Grade: DIFFER(343v376) -- the FOV blend matches; the vector blends differ: retail keeps `to` on the x87 stack (fadd st(1) ; fstp ; fstp st(0)) per component and copies some RHSet words of both cameras into frame locals first (a by-value CRightHandedSet/vector copy model is the next idea)
// Leaf: GetInterpolatedCamera
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006978D0, length 375 bytes (`ret 10h`, CCamera by hidden pointer).
// Callees: CCamera copy ctor @ 0x0049D380 (the local and the result), CCamera::GetHorizontalFOV @ 0x00A0BEB0 (manifest
//   "CMap::DrawGetSeaLevel" propagated), CCamera::SetFOV(float, float) @ 0x00A0BED0, CCamera::SetView(const C3DVector&,
//   const C3DVector&) @ 0x00A0BE30.
// Layout: Ego_r.pdb CCamera (WorldPosition +0, RHSet Up +0xC / Forward +0x18, HorizontalFOV +0x2C, 0x38 bytes).
// Shape: every blended value is to + (from - to) * t; the position goes through a C3DVector temporary copied memberwise.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CCamera {
    C3DVector       WorldPosition;                      // +0x00
    CRightHandedSet RHSet;                              // +0x0C
    bool            HeightLocked;                       // +0x24
    bool            ViewVecZLocked;                     // +0x25
    unsigned long   FOVFlags;                           // +0x28
    float           HorizontalFOV;                      // +0x2C
    float           VerticalFOV;                        // +0x30
    float           Zoom;                               // +0x34
    CCamera(const CCamera& other);                      // 0x0049D380
    float GetHorizontalFOV() const;                     // 0x00A0BEB0
    void SetFOV(float horizontal, float vertical);      // 0x00A0BED0
    void SetView(const C3DVector& forward, const C3DVector& up);   // 0x00A0BE30
};
__forceinline float Blend(float from, float to, float t) { return to + (from - to) * t; }
__forceinline C3DVector Blend(const C3DVector& from, const C3DVector& to, float t)
{
    C3DVector r;
    r.X = Blend(from.X, to.X, t);
    r.Y = Blend(from.Y, to.Y, t);
    r.Z = Blend(from.Z, to.Z, t);
    return r;
}
struct CGameCameraManager {
    CCamera GetInterpolatedCamera(float t, const CCamera& from, const CCamera& to) const;
};

CCamera CGameCameraManager::GetInterpolatedCamera(float t, const CCamera& from, const CCamera& to) const
{
    CCamera camera(from);
    float fromFOV = from.GetHorizontalFOV();
    camera.SetFOV(Blend(fromFOV, to.GetHorizontalFOV(), t), 0.0f);
    camera.WorldPosition = Blend(from.WorldPosition, to.WorldPosition, t);
    C3DVector up = Blend(from.RHSet.Up, to.RHSet.Up, t);
    C3DVector forward = Blend(from.RHSet.Forward, to.RHSet.Forward, t);
    camera.SetView(forward, up);
    return camera;
}
