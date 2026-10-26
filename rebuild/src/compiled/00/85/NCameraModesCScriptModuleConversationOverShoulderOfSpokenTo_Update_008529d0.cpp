// NCameraModes::CScriptModuleConversationOverShoulderOfSpokenTo::Update() -- with two distinct conversation things, put the camera over the spoken-to's shoulder (2 units back along the speaker-to-spoken-to XY direction, sideways by 2 x RandomDirection, 4 up) looking at the speaker's focal point
// Leaf: ?Update@CScriptModuleConversationOverShoulderOfSpokenTo
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector& AccurateNormalise();                                // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

class CTCGraphicAppearance {
public:
    C3DVector GetCameraFocalPos(float height);       // 0x004C12D0
};
class CBaseObject;
class CThing {
public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64 (retail)
    const C3DVector& GetPosition() const;            // 0x004C73D0
    CTCGraphicAppearance* GetPTCGraphicAppearance() const { return PTCGraphicAppearance; }
    C3DVector GetXYDirectionTo(const C3DVector& pos) const
    {
        const C3DVector& here = GetPosition();
        return C3DVector(here.X - pos.X, here.Y - pos.Y, 0.0f);
    }
};
struct CIntelligentPointer_CThingConst {
    virtual void v00();
    void* m_pItem;
    CThing* GetPItem() const;                        // 0x00A01B50 (CBaseIntelligentPointer::GetPItem)
    CThing* operator->() const { return GetPItem(); }
};
struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
};

namespace NCameraModes {

class CScriptModuleBase { public:
    virtual void v00();
    CIntelligentPointer_CThingConst PCamPoint;       // +0x04
    CIntelligentPointer_CThingConst PThingOrigin;    // +0x0c
    C3DVector           CoordOrigin;                 // +0x14
    CRightHandedSet     CoordAxis;                   // +0x20
    CCameraState*       POwner;                      // +0x38
    void*               PCameraManager;              // +0x3c
    bool                SelfTerminate;               // +0x40
    bool                ShortenOnCollision;          // +0x41
    long                CutOutOverride;              // +0x44
    float               FOVOverride;                 // +0x48
};
struct CScriptModuleConversationBase : CScriptModuleBase {
    CIntelligentPointer_CThingConst PSpeaker;        // +0x4c
    CIntelligentPointer_CThingConst PSpokenTo;       // +0x54
    float RandomDirection;                           // +0x5c
    bool IsThingsOk() const
    {
        const CThing* speaker = PSpeaker.GetPItem();
        const CThing* spokenTo = PSpokenTo.GetPItem();
        return speaker && spokenTo && speaker != spokenTo;
    }
};
struct CScriptModuleConversationOverShoulderOfSpokenTo : CScriptModuleConversationBase {
    void Update();
};

void CScriptModuleConversationOverShoulderOfSpokenTo::Update()
{
    if (IsThingsOk()) {
        C3DVector direction = PSpeaker->GetXYDirectionTo(PSpokenTo->GetPosition());
        direction.AccurateNormalise();
        C3DVector side(direction.Y, -direction.X, 0.0f);
        C3DVector spokenToPos = PSpokenTo->GetPosition();
        float scale = RandomDirection * 2.0f;
        C3DVector focal = PSpeaker->GetPTCGraphicAppearance()->GetCameraFocalPos(1.0f);
        POwner->Position = spokenToPos - direction * 2.0f + side * scale + C3DVector(0.0f, 0.0f, 4.0f);
        POwner->LookAt = focal;
    }
}

}