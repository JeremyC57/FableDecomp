// NCameraModes::CScriptModuleGoToLookAt::Update() -- put the owning camera at GoTo; while the tracked thing is alive refresh LookAt from its camera focal point; aim the camera at LookAt
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008534F0, length 144 bytes (141 + int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times, re-fetched per test),
// CTCGraphicAppearance::GetCameraFocalPos(float) @ 0x004C12D0 (C3DVector by hidden pointer).
// Shape: position copy first (implicit copy through re-based pointers); nested null / dead-bit (+0x91)
// tests; then the ctor 0x00852D10 idiom: the thing in a named local (GetPItem before the pushes), the
// focal point in a named local re-read from the frame, LookAt assigned; finally the owner's look-at.
// Layout: PDB NCameraModes::CScriptModuleGoToLookAt (0x6c bytes): PThing +0x4c, GoTo +0x54, LookAt +0x60,
// over the 0x4c-byte CScriptModuleBase (POwner +0x38).
struct CBaseObject;

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

struct CTCGraphicAppearance {
    C3DVector GetCameraFocalPos(float t) const;      // 0x004C12D0
};

struct CThing {
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* m_pGraphicAppearance;      // +0x64
    unsigned char         _pad68[0x29];
    unsigned char         m_dead : 1;                // +0x91 bit 0
    unsigned char         m_91_1 : 7;
};

struct CCameraState {
    unsigned char _pad00[0x10];
    C3DVector     LookAt;                            // +0x10
    unsigned char _pad1c[0x38];
    C3DVector     Position;                          // +0x54
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                       // 0x00A01B10
    CBaseObject* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CBaseObject* item);          // 0x00A01B90
};

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    const CThing* operator->() const { return (const CThing*)GetPItem(); }
};

namespace NCameraModes {

struct CScriptModuleGoToLookAt {
    virtual void v00();
    unsigned char                   _pad04[0x34];
    CCameraState*                   POwner;          // +0x38
    unsigned char                   _pad3c[0x10];
    CIntelligentPointer_CThingConst PThing;          // +0x4c
    C3DVector                       GoTo;            // +0x54
    C3DVector                       LookAt;          // +0x60
    void Update();
};

void CScriptModuleGoToLookAt::Update()
{
    POwner->Position = GoTo;
    if (PThing.GetPItem()) {
        if (!PThing->m_dead) {
            const CThing* target = PThing.operator->();
            C3DVector focal = target->m_pGraphicAppearance->GetCameraFocalPos(1.0f);
            LookAt = focal;
        }
    }
    POwner->LookAt = LookAt;
}

}