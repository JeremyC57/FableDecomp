// NCameraModes::CStringAndCageBase::Update() -- step the string-and-cage camera: move the go-to point, ease both Z heights and commit go-to and look-at
// Leaf: ?Update@CStringAndCageBase@NCameraModes@@
// Retail 0x008728C0, 352 bytes, `ret`. Callees: NCameraModes::CBase::GetObservedThing 0x0084F1B0,
// CTCGraphicAppearance::GetCameraFocalPos 0x004C12D0 (hidden C3DVector result, float), the class's private const
// UpdateStringLength 0x008722D0 / UpdateCageRadius 0x00871D50 (both bool: `mov al,1` / `xor al,al`; Update ignores the
// second result) / UpdateAutoGoBehind 0x00872110 (all C3DVector&; Ego_r.pdb ?UpdateStringLength@ / ?UpdateCageRadius@
// CStringAndCageBase@NCameraModes@@ABE_NAAVC3DVector@@@Z, ?UpdateAutoGoBehind@...@@ABEXAAVC3DVector@@@Z, in the TU order
// UpdateAutoGoBehind, UpdateStringLength, UpdateCageRadius), private UpdateXYVars 0x00871FD0, UpdateZVars 0x00872050, CGameCameraManager::GetNoControlMovementTime 0x00697710,
// CThing::GetPosition 0x004C73D0, NCameraModes::CBase::GetCamTools 0x0084D080 (result unused), virtual slots 28
// (+0x70, auto-go-behind enabled), 29 (+0x74, auto-go-behind time) and 30 (+0x78, SetGoToPosition).
// Pools: 0.0f 0x0122DEDC, 10.0f 0x01237F0C, 2.0f 0x01277538, 1.0f 0x0122DED8.
// Layout (retail = PDB - 0x10 at LookAt's tail, - 0x14 after the 12-byte vector, as the landed UpdateZVars 0x00872050):
// LookAt position +0x10, CameraManager +0x40, GoTo position +0x54, CurrentHeightOffset +0xB0; the camera manager's
// "no smoothing" byte is at +0xB0.
// Shape: the go-to position is copied into a local and handed to the position updaters; the observed thing's camera
// focal point is the look-at target; each Z is eased as `(old * rate + target) / (rate + 1)` with rate 0 when the
// manager disables smoothing, the result parked across a GetCamTools call. Shape lessons: C3DVector needs a memberwise user
// copy ctor (retail copies Y first, straight from the member; the implicit copy goes through `lea eax,[esi+54h]`), and the
// observed thing is a named local so GetObservedThing runs before the GetCameraFocalPos argument pushes.
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
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
struct CGameCameraManager {
    unsigned char _pad000[0xb0];
    bool          NoSmoothing;                                     // +0xb0
    float GetNoControlMovementTime();                              // 0x00697710
};
namespace NCameraModes {
class CTools { unsigned char _body[0x24]; };
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual bool  IsAutoGoBehindEnabled() const;                   // slot 28 (+0x70)
    virtual float GetAutoGoBehindTime() const;                     // slot 29 (+0x74)
    virtual void  SetGoToPosition(const C3DVector& pos);           // slot 30 (+0x78)
    unsigned char       _pad04[0x10 - 0x04];
    C3DVector           LookAtPosition;                            // +0x10
    unsigned char       _pad1c[0x40 - 0x1c];
    CGameCameraManager* CameraManager;                             // +0x40
    unsigned char       _pad44[0x54 - 0x44];
    C3DVector           GoToPosition;                              // +0x54
    unsigned char       _pad60[0xa4 - 0x60];
    const CThingGameObject* GetObservedThing() const;              // 0x0084F1B0
    const CTools& GetCamTools() const;                             // 0x0084D080
};
class CStringAndCageBase : public CBase {
public:
    float DesiredHeightOffset;                                     // +0xa4
    float DesiredStringLength;                                     // +0xa8
    float DesiredCageRadius;                                       // +0xac
    float CurrentHeightOffset;                                     // +0xb0
    void Update();
private:
    void UpdateXYVars();                                           // 0x00871FD0
    void UpdateZVars();                                            // 0x00872050
    void UpdateAutoGoBehind(C3DVector& goTo) const;                // 0x00872110
    bool UpdateStringLength(C3DVector& goTo) const;                // 0x008722D0
    bool UpdateCageRadius(C3DVector& goTo) const;                  // 0x00871D50
};
void CStringAndCageBase::Update()
{
    C3DVector goTo = GoToPosition;
    const CThingGameObject* thing = GetObservedThing();
    C3DVector lookAt = thing->Appearance->GetCameraFocalPos(1.0f);
    if (!UpdateStringLength(goTo))
        UpdateCageRadius(goTo);
    if (IsAutoGoBehindEnabled()) {
        float idle = CameraManager->GetNoControlMovementTime();
        if (GetAutoGoBehindTime() < idle)
            UpdateAutoGoBehind(goTo);
    }
    UpdateXYVars();
    UpdateZVars();
    goTo.Z = GetObservedThing()->GetPosition().Z + CurrentHeightOffset;
    bool noSmoothing = CameraManager->NoSmoothing;
    float rate = noSmoothing ? 0.0f : 10.0f;
    float z = (GoToPosition.Z * rate + goTo.Z) / (rate + 1.0f);
    GetCamTools();
    goTo.Z = z;
    rate = noSmoothing ? 0.0f : 2.0f;
    z = (LookAtPosition.Z * rate + lookAt.Z) / (rate + 1.0f);
    GetCamTools();
    lookAt.Z = z;
    SetGoToPosition(goTo);
    LookAtPosition = lookAt;
}
}