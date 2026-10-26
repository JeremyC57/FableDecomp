// NCameraModes::CZTargetNew::OnInit() -- start the Z-targeting camera: inherit the current mode's observed-position Z offset, reset the right-stick look-around, build the X axis from the observed thing's reversed, flattened, normalised facing and the Y axis perpendicular to it, then initialise the transition and the coordinate system
// Leaf: OnInit
// Retail: Fable.exe @ 0x00872A20, 206 bytes, `ret`. PDB ?OnInit@CZTargetNew@NCameraModes@@UAEXXZ (vtable 0x0127754C slot 21).
// Callees: CGameCameraManager::ResetRightStickLookAroundVector() 0x006976B0 (manifest "CQ_GuildTrainingSkillScript::Init"),
// NCameraModes::CBase::GetObservedThing() const 0x0084F1B0, the thing physics' virtual slot 70 (+0x118, GetFacingVector,
// C3DVector by hidden pointer), C3DVector::AccurateNormalise @ 0x00A14440, CZTargetNew::InitialiseTransitionVariables 0x00872490,
// virtual slot 24 UpdateCoordSystem (+0x60, 0x00872EC0). Float pool 0x0122DEE0 = -1.0f.
// Retail layout (NCameraModes::CBase is 0xA4 bytes in retail, PDB 0xC8; see the landed CFirstPerson::OnInit 0x008735D0):
// CBase CameraManager +0x40, ObservedPositionZOffset +0x8C (PDB +0xB0); CZTargetNew XAxis +0xD4, YAxis +0xE0 (PDB
// +0xF8/+0x104); CGameCameraManager CurrentMode (CCountedPointer<NCameraModes::CBase>) +0x98 (PDB +0x9C); CThing physics +0x60.
// Shape: C3DVector has a user copy ctor and the implicit operator= (the `lea` pointer-register copies) and a scaling
// operator* written `return C3DVector(X * f, Y * f, Z * f);` (a named result keeps the products on the x87 stack); the X axis's Z is cleared through the member before Normalise;
// the Y axis comes from an inline XAxis member returning (Y, -X, 0) (both reads through the XAxis pointer in edi).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector& AccurateNormalise();                                        // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
    C3DVector GetPerpendicularXY() const { return C3DVector(Y, -X, 0.0f); }   // inline (name descriptive)
};

struct CThingPhysics {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual C3DVector GetFacingVector() const;               // slot 70 (+0x118)
};
class CThing { public:
    unsigned char  _pad[0x60];
    CThingPhysics* m_pPhysics;                               // +0x60
    CThingPhysics* GetPhysics() const { return m_pPhysics; }
};
class CThingGameObject : public CThing { public: };

namespace NCameraModes { class CBase; }
template<class T> class CCountedPointer { public:
    T*    Data;
    void* Info;
    T* operator->() const { return Data; }
};
class CGameCameraManager {
public:
    unsigned char                         _pad00[0x98];
    CCountedPointer<NCameraModes::CBase>  CurrentMode;       // +0x98
    void ResetRightStickLookAroundVector();                  // 0x006976B0
};

namespace NCameraModes {
class CBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20();
    virtual void OnInit();                                   // slot 21
    virtual void v22(); virtual void v23();
    virtual void UpdateCoordSystem();                        // slot 24 (+0x60)
    unsigned char       _pad04[0x3c];
    CGameCameraManager* CameraManager;                       // +0x40
    unsigned char       _pad44[0x48];
    float               ObservedPositionZOffset;             // +0x8C
    unsigned char       _pad90[0x14];
    const CThingGameObject* GetObservedThing() const;        // 0x0084F1B0
};
class CZTargetNew : public CBase {
public:
    unsigned char _padA4[0x30];
    C3DVector     XAxis;                                     // +0xD4
    C3DVector     YAxis;                                     // +0xE0
    long          FramesForTransition;                       // +0xEC
    void InitialiseTransitionVariables();                    // 0x00872490
    virtual void OnInit();
    virtual void UpdateCoordSystem();                        // 0x00872EC0
};
}

void NCameraModes::CZTargetNew::OnInit()
{
    ObservedPositionZOffset = CameraManager->CurrentMode->ObservedPositionZOffset;
    CameraManager->ResetRightStickLookAroundVector();
    XAxis = GetObservedThing()->GetPhysics()->GetFacingVector() * -1.0f;
    XAxis.Z = 0.0f;
    XAxis.AccurateNormalise();
    YAxis = XAxis.GetPerpendicularXY();
    InitialiseTransitionVariables();
    UpdateCoordSystem();
}