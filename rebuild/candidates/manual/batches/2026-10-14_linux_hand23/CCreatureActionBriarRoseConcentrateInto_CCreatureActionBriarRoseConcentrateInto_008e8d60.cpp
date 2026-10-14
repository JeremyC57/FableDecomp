// CCreatureAction_BriarRoseConcentrateInto::CCreatureAction_BriarRoseConcentrateInto(const CCreatureAction_BriarRoseConcentrateInto&) -- copy constructor: base CCombatActionBase(other), own vtable (0x01287904), Imposter copied, PTarget copy-constructed via GetPItem/SetPItem
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E8D60, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0, CBaseIntelligentPointer::CBaseIntelligentPointer
// @ 0x00A01B10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (the inline
// CIntelligentPointer copy constructor, as in the landed 0x008C3D10 shape; its vtable 0x01238C6C)
// Shape: the implicit memberwise copy: base copy out of line, vtable, the Imposter byte (`mov al`), then the
// pointer member's inline copy constructor (its address in edi). The manifest label "CCreatureAction_ActivateQuest"
// is rejected: the vtable stored is CCreatureAction_BriarRoseConcentrateInto's (rtti-vtables.tsv).
// The retail layout is byte-packed (bool members followed by unaligned dwords/vectors: +0x115, +0x125...), as the
// PDB-derived engine headers also declare (#pragma pack(push, 1)); the pragma here reproduces that real layout.
struct CThingCreatureBase;
struct CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                   // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {         // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

#pragma pack(push, 1)
struct CCreatureAction_BriarRoseConcentrateInto : CCombatActionBase {   // vtable 0x01287904; retail sizeof 0x11d
    unsigned char       Imposter;           // +0x114 (engine header: bool at PDB +0x12c)
    CIntelligentPointer PTarget;            // +0x115 CIntelligentPointer<CThing> (byte-packed after the bool)
    virtual void v00();
    CCreatureAction_BriarRoseConcentrateInto(CThingCreatureBase* owner, CBaseObject* target, bool imposter);
    CCreatureAction_BriarRoseConcentrateInto(const CCreatureAction_BriarRoseConcentrateInto& other);
};
#pragma pack(pop)

CCreatureAction_BriarRoseConcentrateInto::CCreatureAction_BriarRoseConcentrateInto(const CCreatureAction_BriarRoseConcentrateInto& other)
    : CCombatActionBase(other)
    , Imposter(other.Imposter)
    , PTarget(other.PTarget)
{
}
