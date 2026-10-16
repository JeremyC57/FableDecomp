// CView3DBase::UpdateCameraMovementInfo(const CCamera& camera) -- shift the current camera into LastCam, store the new one; after the first few updates record the frame's movement vector (last - current) and mark it valid, otherwise count the update
// Leaf: ?UpdateCameraMovementInfo@CView3DBase@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069E870, length 123 bytes (padded to 128), `ret 4`.
// Callees: none (two `rep movsd` 14-dword CCamera copies).
// Layout (PDB, matches retail): CView3DBase vptr +0, PEngine +4, Window C2DBoxF +8, CameraMovementInfo +0x18:
// NoUpdates +0x18, MovementVectorValid +0x1c, CurrentCam +0x20, LastCam +0x58 (CCamera, 56 bytes), MovementVector +0x90.
// Shape: the difference is a C3DVector temporary built by the 3-float ctor (no user copy ctor) and copied into the member
// through integer registers.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
struct CRightHandedSet {
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};
struct CCamera {                                   // 56 bytes
    C3DVector       WorldPosition;                 // +0x00
    CRightHandedSet RHSet;                         // +0x0c
    bool            HeightLocked;                  // +0x24
    bool            ViewVecZLocked;                // +0x25
    unsigned long   FOVFlags;                      // +0x28
    float           HorizontalFOV;                 // +0x2c
    float           VerticalFOV;                   // +0x30
    float           Zoom;                          // +0x34
};
struct CCameraMovementInfo {
    long      NoUpdates;                           // +0x00
    bool      MovementVectorValid;                 // +0x04
    CCamera   CurrentCam;                          // +0x08
    CCamera   LastCam;                             // +0x40
    C3DVector MovementVector;                      // +0x78
};
struct C2DBoxF {
    float X0, Y0, X1, Y1;
};
struct CIEngine;
struct CView3DBase {
    virtual void v00();
    CIEngine*           PEngine;                   // +0x04
    C2DBoxF             Window;                    // +0x08
    CCameraMovementInfo CameraMovementInfo;        // +0x18
    void UpdateCameraMovementInfo(const CCamera& camera);
};
void CView3DBase::UpdateCameraMovementInfo(const CCamera& camera)
{
    CameraMovementInfo.LastCam = CameraMovementInfo.CurrentCam;
    CameraMovementInfo.CurrentCam = camera;
    if (CameraMovementInfo.NoUpdates > 2) {
        CameraMovementInfo.MovementVector = CameraMovementInfo.LastCam.WorldPosition - CameraMovementInfo.CurrentCam.WorldPosition;
        CameraMovementInfo.MovementVectorValid = true;
    }
    else
        ++CameraMovementInfo.NoUpdates;
}
