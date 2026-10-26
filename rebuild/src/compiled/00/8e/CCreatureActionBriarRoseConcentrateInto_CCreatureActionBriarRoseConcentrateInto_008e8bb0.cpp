// CCreatureAction_BriarRoseConcentrateInto::CCreatureAction_BriarRoseConcentrateInto(CThingCreatureBase*, CThing*, bool) -- base CCombatActionBase(owner, 50, zeroed CSoundPair), own vtable (0x01287904), Imposter from the argument, PTarget built on the target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E8BB0, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90 (the inline
// CIntelligentPointer(item) constructor: base, its vtable 0x01238C6C, SetPItem)
// Shape: init-list members in declaration order: the Imposter byte (third argument's slot +0x1c, `mov dl`) is stored
// between the vtable store and the member's constructor call, the target (second argument, +0x18) is pushed for SetPItem;
// the member address rides in edi. `ret 0Ch`: three arguments.
// The retail layout is byte-packed (bool members followed by unaligned dwords/vectors: +0x115, +0x125...), as the
// PDB-derived engine headers also declare (#pragma pack(push, 1)); the pragma here reproduces that real layout.
class CThingCreatureBase;
class CBaseObject;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

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

class CCombatActionBase { public:                  // retail sizeof 0x114 (the PDB build is 0x12c: three containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

#pragma pack(push, 1)
class CCreatureAction_BriarRoseConcentrateInto : public CCombatActionBase { public:   // vtable 0x01287904; retail sizeof 0x11d
    unsigned char       Imposter;           // +0x114 (engine header: bool at PDB +0x12c)
    CIntelligentPointer PTarget;            // +0x115 CIntelligentPointer<CThing> (byte-packed after the bool)
    virtual void v00();
    CCreatureAction_BriarRoseConcentrateInto(CThingCreatureBase* owner, CBaseObject* target, bool imposter);
    CCreatureAction_BriarRoseConcentrateInto(const CCreatureAction_BriarRoseConcentrateInto& other);
};
#pragma pack(pop)

CCreatureAction_BriarRoseConcentrateInto::CCreatureAction_BriarRoseConcentrateInto(CThingCreatureBase* owner, CBaseObject* target, bool imposter)
    : CCombatActionBase(owner, 50, CSoundPair())
    , Imposter(imposter)
    , PTarget(target)
{
}