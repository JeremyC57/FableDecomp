// Grade: DIFFER(950v966) -- NCameraModes::CBase::FrameUpdate: instruction stream matches up to frame placement; retail packs the zOffset copy, lookAt and the blend float into one frame vector (0x10), previous/blended/direction into another and the operator* temporary with the occlusion hit; ours keeps lookAt and the temporaries in separate slots (frame 0x5c v 0x50) and so differs in the temp-copy stores
// Leaf: FrameUpdate
extern "C" double __cdecl exp(double x);
#pragma intrinsic(exp)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetLengthSquared() const { return Z * Z + Y * Y + X * X; }
    void Normalise();                                   // 0x00A14440
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
};
struct CThing {
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct CBaseIntelligentPointer {
    CThing* GetPItem();                                 // 0x00A01B50
};
struct CCamera {
    C3DVector Position;                                 // +0x00
    void PointAt(const C3DVector& target);              // 0x00A0C2B0
    void SetRollAngle(float angle);                     // 0x0084CD10
    void SetFOV(float horizontal, float vertical);      // 0x00A0BED0
};
struct CCameraManager {
    unsigned char           _pad00[4];
    CCamera                 Camera;                     // +0x04
    unsigned char           _pad10[0xf0];
    CBaseIntelligentPointer m_observedThing;            // +0x100
    CThing* GetObservedThing() { return m_observedThing.GetPItem(); }
};
struct CCameraModeDef {
    unsigned char _pad[0x5c];
    float         LookAtLag;                            // +0x5c
};
struct CPhysicsState {
    C3DVector     Position;                             // +0x00
    unsigned char _pad[0x24];
};
namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    virtual void UpdateMode();                          // slot 1 (+0x04)
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual bool UsesOcclusionPullIn();                 // slot 7 (+0x1c)
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13();
    virtual void v14(); virtual void v15(); virtual void v16();
    virtual void UpdateTargets();                       // slot 17 (+0x44)
    virtual float GetTargetFOV();                       // slot 18 (+0x48)
    virtual float GetTargetRollAngle();                 // slot 19 (+0x4c)
    virtual float GetTargetObservedPositionZOffset();   // slot 20 (+0x50)
    float           DefaultFOV;                         // +0x04
    float           DefaultRollAngle;                   // +0x08
    float           DefaultObservedPositionZOffset;     // +0x0c
    CPhysicsState   LookAt;                             // +0x10
    CCameraManager* CameraManager;                      // +0x40
    const CCameraModeDef* PCameraModeDef;               // +0x44
    unsigned char   SubComponents[0xc];                 // +0x48
    CPhysicsState   GoTo;                               // +0x54
    float           CameraFOV;                          // +0x84
    float           RollAngle;                          // +0x88
    float           ObservedPositionZOffset;            // +0x8c
    C2DVector       RightStickVector;                   // +0x90
    C2DVector       LeftStickVector;                    // +0x98
    bool            ViewOccluded;                       // +0xa0
    bool IsCameraOcclusionFromTo(C3DVector* from, C3DVector* to, C3DVector* hit);   // 0x0084D2F0
    void UpdateFixLookDirection(const C3DVector& pos, const C3DVector& target, C3DVector& lookDir, float angle);   // 0x0084CDF0
    void UpdateCameraHeight(C3DVector& position);       // 0x0084D7E0
    void UpdateCameraStretchAroundHero(C3DVector& position, C3DVector& lookAt);   // 0x0084D910
    void UpdateCameraOcclusion(C3DVector& position);    // 0x0084DB30
    void FrameUpdate();
};
}

void NCameraModes::CBase::FrameUpdate()
{
    C3DVector previousLookAt = LookAt.Position;
    UpdateMode();
    UpdateTargets();
    float fov = CameraFOV;
    float roll = RollAngle;
    float zOffset = ObservedPositionZOffset;
    CameraFOV = fov = (GetTargetFOV() + fov * 3.0f) * 0.25f;
    RollAngle = roll = (GetTargetRollAngle() + roll * 3.0f) * 0.25f;
    ObservedPositionZOffset = zOffset = (GetTargetObservedPositionZOffset() + zOffset * 3.0f) * 0.25f;
    C3DVector position = GoTo.Position;
    C3DVector lookAt = LookAt.Position;
    float lag = 0.0f;
    if (PCameraModeDef)
        lag = PCameraModeDef->LookAtLag;
    C3DVector blended = previousLookAt * lag + lookAt;
    lookAt = blended * (1.0f / (lag + 1.0f));
    lookAt.Z += zOffset;
    UpdateCameraStretchAroundHero(position, lookAt);
    UpdateCameraOcclusion(position);
    C3DVector direction = lookAt - position;
    C3DVector newPosition = position;
    UpdateCameraHeight(newPosition);
    float blend = 0.24f;
    if (UsesOcclusionPullIn() && ViewOccluded) {
        C3DVector hit;
        if (IsCameraOcclusionFromTo(&position, &newPosition, &hit))
            newPosition = hit;
        position = newPosition;
        const C3DVector& heroPos = CameraManager->GetObservedThing()->GetPosition();
        C2DVector flat(position.X - heroPos.X, position.Y - heroPos.Y);
        blend = (float)((1.0 - exp((flat.Y * flat.Y + flat.X * flat.X) * -0.2f)) * 0.24);
    }
    UpdateFixLookDirection(position, lookAt, direction, blend);
    CCamera& camera = CameraManager->Camera;
    camera.Position = position;
    C3DVector look = direction;
    if (direction.GetLengthSquared() > 0.0001f * 0.0001f) {
        look.Normalise();
        camera.PointAt(position + look);
        camera.SetRollAngle(roll);
    }
    camera.SetFOV(fov, 0.0f);
    RightStickVector = C2DVector(0.0f, 0.0f);
    LeftStickVector = C2DVector(0.0f, 0.0f);
}
