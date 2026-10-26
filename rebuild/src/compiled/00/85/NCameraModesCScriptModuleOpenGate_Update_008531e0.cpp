// NCameraModes::CScriptModuleOpenGate::Update() -- put the owning camera at Pos, aim it at the gate's helper point, then advance Pos by DeltaPos
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008531E0, length 128 bytes (117 + int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the graphic appearance's helper-point getter
// @ 0x004C1060 (manifest "CTCGraphicAppearance::GetHelperPointOr"; C3DVector by hidden pointer, float arg).
// Shape: plain implicit copies to the owner's Position (+0x54) and, from the returned temporary (read
// through eax, no named local), LookAt (+0x10); then `Pos += DeltaPos` through an inline FREE Add(a, b) helper (both operands by
// reference: the X term loads Pos first, `fld [edi] ; fadd [esi+68h]`; a member operator+= commutes it).
// Layout: PDB NCameraModes::CScriptModuleOpenGate (0x74 bytes): PGate +0x54, Pos +0x5c, DeltaPos +0x68,
// over CScriptModuleSelfTerminateBase (0x54 bytes; POwner +0x38). The gate's graphic appearance is
// cached at CThing +0x64.
class CBaseObject;

class C3DVector { public:
    float X, Y, Z;
};

struct CTCGraphicAppearance {
    C3DVector GetHelperPointOrCentre(float t) const; // 0x004C1060
};

class CThing { public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* m_pGraphicAppearance;      // +0x64
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
    CBaseObject* GetPItem() const;                   // 0x00A01B50
};

struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {   // vtable 0x01237F1C
    virtual void v00();
    const CThing* operator->() const { return (const CThing*)GetPItem(); }
};

static inline void Add(C3DVector& a, const C3DVector& b) { a.X += b.X; a.Y += b.Y; a.Z += b.Z; }
namespace NCameraModes {

struct CScriptModuleOpenGate {
    virtual void v00();
    unsigned char                   _pad04[0x34];
    CCameraState*                   POwner;          // +0x38
    unsigned char                   _pad3c[0x18];    // +0x3c base state, WFDuration, EndFrame
    CIntelligentPointer_CThingConst PGate;           // +0x54
    C3DVector                       Pos;             // +0x5c
    C3DVector                       DeltaPos;        // +0x68
    void Update();
};

void CScriptModuleOpenGate::Update()
{
    POwner->Position = Pos;
    CTCGraphicAppearance* appearance = PGate->m_pGraphicAppearance;
    CCameraState* owner = POwner;
    owner->LookAt = appearance->GetHelperPointOrCentre(1.0f);
    Add(Pos, DeltaPos);
}

}