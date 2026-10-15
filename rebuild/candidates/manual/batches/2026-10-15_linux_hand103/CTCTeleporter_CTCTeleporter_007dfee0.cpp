// CTCTeleporter::CTCTeleporter(CThing* parent) -- constructor: CTCBase(parent), own vtable, null PDef, Active/Activated false, HeroInsideRadius true, empty PParticle, WaitingForTutorialToFinish false, then fetches the teleporter definition into PDef
// Retail: Fable.exe @ 0x007DFEE0, 84 bytes (`ret 4`, vtable 0x0126FDA4).
// Callees: CTCBase::CTCBase(CThing*) 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite" is
//          propagated), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (then the
//          CIntelligentPointer<CThing> vtable 0x01238C6C), CDefClassBase::GetSubDef<CTeleporterDef>
//          (CDefPointer*) 0x007E0540 on the parent thing's definition (+4 -> +0x70).
// Layout: Ego_r.pdb CTCTeleporter (0x1C, packed): PDef +0x0C, Active +0x10, Activated +0x11,
// HeroInsideRadius +0x12, PParticle +0x13 (unaligned 8-byte intelligent pointer), 
// WaitingForTutorialToFinish +0x1B -- no engine header, so a local `#pragma pack(1)` model.
// Shape: every store follows the vtable store: PDef and PParticle are member objects with inline
//        constructors, the three flags are init-list bytes (immediates), WaitingForTutorialToFinish
//        is a body assignment (scheduled after the PParticle vtable store, between the two parent
//        loads); `&PDef` is formed once (`lea edi,[esi+0Ch]`) for the trailing GetSubDef call, which
//        re-reads the parent from the CTCBase member.
class CThing;
struct CTeleporterDef;

struct CDefPointer {                                    // CDefPointer<const CTeleporterDef>
    CTeleporterDef* m_p;
    CDefPointer() : m_p(0) {}
};

struct CDefClassBase {
    bool GetSubDef(CDefPointer* out);                   // 0x007E0540
};

struct CThingDefs {
    unsigned char _pad[0x70];
    CDefClassBase* m_pDefinition;                       // +0x70
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                        // 8 bytes: vptr + item
    virtual void v00();
    void* m_pItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {  // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() {}
};

struct CTCBase {
    virtual void v00();
    CThingDefs* m_pParentThing;                         // +0x04
    unsigned char _pad_0x08[0x4];
    CTCBase(CThing* parent);                            // 0x00686800
};

struct CTCTeleporter : CTCBase {
    CDefPointer PDef;                                   // +0x0c
    bool Active;                                        // +0x10
    bool Activated;                                     // +0x11
    bool HeroInsideRadius;                              // +0x12
    CIntelligentPointer PParticle;                      // +0x13
    bool WaitingForTutorialToFinish;                    // +0x1b
    CTCTeleporter(CThing* parent);
    virtual void v00();
};
#pragma pack(pop)

CTCTeleporter::CTCTeleporter(CThing* parent)
    : CTCBase(parent), Active(false), Activated(false), HeroInsideRadius(true)
{
    WaitingForTutorialToFinish = false;
    m_pParentThing->m_pDefinition->GetSubDef(&PDef);
}
