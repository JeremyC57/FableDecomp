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
class CTeleporterDef;

struct CDefPointer {                                    // CDefPointer<const CTeleporterDef>
    CTeleporterDef* m_p;
    CDefPointer() : m_p(0) {}
};

class CDefClassBase { public:
    bool GetSubDef_007e0540(CDefPointer* out);                   // 0x007E0540
};

struct CThingDefs {
    unsigned char _pad[0x70];
    CDefClassBase* m_pDefinition;                       // +0x70
};

#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {  // vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() {}

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CTCBase { public:
    virtual void v00();
    CThingDefs* m_pParentThing;                         // +0x04
    unsigned char _pad_0x08[0x4];
    CTCBase(CThing* parent);                            // 0x00686800
};

class CTCTeleporter : public CTCBase { public:
    CDefPointer PDef;                                   // +0x0c
    bool Active;                                        // +0x10
    bool Activated;                                     // +0x11
    bool HeroInsideRadius;                              // +0x12
    CIntelligentPointer<CThing> PParticle;                      // +0x13
    bool WaitingForTutorialToFinish;                    // +0x1b
    CTCTeleporter(CThing* parent);
    virtual void v00();
};
#pragma pack(pop)

CTCTeleporter::CTCTeleporter(CThing* parent)
    : CTCBase(parent), Active(false), Activated(false), HeroInsideRadius(true)
{
    WaitingForTutorialToFinish = false;
    m_pParentThing->m_pDefinition->GetSubDef_007e0540(&PDef);
}