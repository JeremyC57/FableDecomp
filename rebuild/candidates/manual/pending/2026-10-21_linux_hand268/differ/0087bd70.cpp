// NCameraModes::CRotateAround::Update() -- turn Theta by the (optionally inverted) rotation speed x the manager's look-around delta, place the camera on the XAxis/YAxis circle of radius StringLengthXY around the observed thing's focal point (at the thing's height + HeightOffset, clamped to the world height limits), looking at the focal point
// Grade: DIFFER(451v451) -- frame-slot allocation only: retail frame is 0x40 (thing Z at [esp+40h], the HeightOffset vector's Z parked in eax and stored into the dead XAxis*cos Z word [esp+28h]); ours is 0x44 with the thing Z at [esp+8] and the HeightOffset temp in its own word, which shifts every temp; the `+ C3DVector(0,0,HeightOffset)` form (x/y adds of 0 fold) and the Z-only thing-position read are right
// Leaf: ?Update@CRotateAround
#include <math.h>
#pragma intrinsic(sin, cos)

struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CThing {
public:
    const C3DVector& GetPosition() const;                         // 0x004C73D0
};
class CTCGraphicAppearance {
public:
    C3DVector GetCameraFocalPos(float t) const;                   // 0x004C12D0
};
class CThingGameObject : public CThing {
public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* Appearance;                              // +0x64
};
struct CUserProfileManager_Retail {
    unsigned char _pad00[0xc];
    bool          UseNewProfileValues;                 // +0x0c
    unsigned char _pad0d[0x2];
    bool          CameraXYInverted;                    // +0x0f (default)
    unsigned char _pad10[0xcf - 0x10];
    bool          ProfileCameraXYInverted;             // +0xcf (active profile)
    static CUserProfileManager_Retail& Get();          // 0x0040D2A0 (CTBaseSingleton<CUserProfileManager>::Get)
    bool IsCameraXYInverted() const { return UseNewProfileValues ? ProfileCameraXYInverted : CameraXYInverted; }
};
struct CGameCameraManager {
    unsigned char _pad000[0xf8];
    float         MinWorldHeight;                      // +0xf8
    float         MaxWorldHeight;                      // +0xfc
    unsigned char _pad100[0x148 - 0x100];
    float         CamRSLookAroundDeltaTheta;           // +0x148
    unsigned char _pad14c[0x152 - 0x14c];
    bool          LimitMinMaxWorldHeight;              // +0x152
};
namespace NCameraModes {
class CBase {
public:
    virtual void v00();
    unsigned char       _pad04[0x10 - 0x04];
    C3DVector           LookAtPosition;                            // +0x10
    unsigned char       _pad1c[0x40 - 0x1c];
    CGameCameraManager* CameraManager;                             // +0x40
    unsigned char       _pad44[0x54 - 0x44];
    C3DVector           GoToPosition;                              // +0x54
    unsigned char       _pad60[0x90 - 0x60];
    float               RotationSpeed;                             // +0x90
    unsigned char       _pad94[0xa0 - 0x94];
    bool                IgnoreWorldHeightLimits;                   // +0xa0
    const CThingGameObject* GetObservedThing() const;              // 0x0084F1B0
};
class CRotateAround : public CBase {
public:
    float     StringLengthXY;                                      // +0xa4
    float     HeightOffset;                                        // +0xa8
    C3DVector XAxis;                                               // +0xac
    C3DVector YAxis;                                               // +0xb8
    float     Theta;                                               // +0xc4
    void Update();
};
void CRotateAround::Update()
{
    float speed = RotationSpeed;
    if (CUserProfileManager_Retail::Get().IsCameraXYInverted())
        speed *= -1.0f;
    const CThingGameObject* thing = GetObservedThing();
    C3DVector focal = thing->Appearance->GetCameraFocalPos(1.0f);
    float thingZ = GetObservedThing()->GetPosition().Z;
    Theta += speed * CameraManager->CamRSLookAroundDeltaTheta;
    double angle = Theta * 6.2831854820251465;
    C3DVector position = (XAxis * (float)cos(angle) + YAxis * (float)sin(angle)) * StringLengthXY
                       + C3DVector(focal.X, focal.Y, thingZ) + C3DVector(0.0f, 0.0f, HeightOffset);
    if (CameraManager->LimitMinMaxWorldHeight && !IgnoreWorldHeightLimits) {
        if (position.Z < CameraManager->MinWorldHeight)
            position.Z = CameraManager->MinWorldHeight;
        else if (position.Z > CameraManager->MaxWorldHeight)
            position.Z = CameraManager->MaxWorldHeight;
    }
    GoToPosition = position;
    LookAtPosition = focal;
}
}
