// NCameraModes::CScriptModuleGoToLookAt::CScriptModuleGoToLookAt(const C3DVector& goTo, const CThing* thing) -- constructor: track the thing, go to goTo and look at the thing's camera focal point
// Leaf: ??0CScriptModuleGoToLookAt
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00852D10, length 128 bytes (125 + int3 padding).
// Callees: NCameraModes::CScriptModuleBase::CScriptModuleBase() @ 0x005ED070,
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, ::SetPItem @ 0x00A01B90, ::GetPItem @ 0x00A01B50,
// CTCGraphicAppearance::GetCameraFocalPos(float) @ 0x004C12D0 (C3DVector by hidden pointer).
// Vtables: 0x012657D8 (NCameraModes::CScriptModuleGoToLookAt), 0x01237F1C (CIntelligentPointer<const CThing>).
// Shape: initialiser list PThing(thing) (base ctor, vtable, SetPItem) and GoTo(goTo) through the
// memberwise user copy ctor (direct stores to +0x54..); the body takes the thing into a named local
// (retail calls GetPItem BEFORE pushing the focal-point arguments) and the by-value focal point into a
// named local (re-read from the frame, not from eax), then assigns LookAt with the implicit copy through
// `lea edx,[esi+60h]`. The thing's graphic-appearance component is cached at +0x64.
// Layout: PDB NCameraModes::CScriptModuleGoToLookAt (0x6c bytes): PThing +0x4c, GoTo +0x54, LookAt +0x60;
// the landed two-vector overload 0x007187C0 uses the same model.
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
};

struct CScriptModuleBase {
    virtual void v00();
    unsigned char m_state[0x4c - 4];
    CScriptModuleBase();                             // 0x005ED070
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
    CIntelligentPointer_CThingConst(const CThing* thing) : CBaseIntelligentPointer() { SetPItem((const CBaseObject*)thing); }
    const CThing* operator->() const { return (const CThing*)GetPItem(); }
};

namespace NCameraModes {

struct CScriptModuleGoToLookAt : CScriptModuleBase {     // vtable 0x012657D8
    CIntelligentPointer_CThingConst PThing;          // +0x4c
    C3DVector                       GoTo;            // +0x54
    C3DVector                       LookAt;          // +0x60
    virtual void v00();
    CScriptModuleGoToLookAt(const C3DVector& goTo, const CThing* thing);
};

CScriptModuleGoToLookAt::CScriptModuleGoToLookAt(const C3DVector& goTo, const CThing* thing)
    : PThing(thing), GoTo(goTo)
{
    const CThing* target = PThing.operator->();
    C3DVector focal = target->m_pGraphicAppearance->GetCameraFocalPos(1.0f);
    LookAt = focal;
}

}
