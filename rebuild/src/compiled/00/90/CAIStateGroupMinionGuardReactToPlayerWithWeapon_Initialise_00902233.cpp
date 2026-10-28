// CAIStateGroup_MinionGuardReactToPlayerWithWeapon::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const CCharString&) -- clear the warning record and the player pointer, then the base initialisation.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00902233, length 97 bytes, `ret 14h`, frame pointer kept
// (`push ebp ; mov ebp,esp` with `leave`) and the size-optimised `and dword ptr [esi+14h],0` zero stores:
// this translation unit was built with frame pointers and /Os, reproduced in-source with
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)` (the landed 0x00902045 sibling).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice),
// GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10 on the converting temporary (vtable 0x01231C4C
// = CIntelligentPointer<CThingPlayerCreature>), then CAIStateGroupBase::Initialise @ 0x008FCEC0 with the five
// arguments pushed straight from the frame (`push dword ptr [ebp+..]`).
// Shape: three plain longs zeroed first (+0x14 warning source, +0x18, +0x1c last warning id -- the members
// the landed CheckForOtherGuardsNewWarning @ 0x00939D70 writes), then `PPlayer = 0` at +0x20 through the
// converting temporary + inline `SetPItem(other.GetPItem())` assignment. No engine header for this class.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CThing;
class CAIBrain;
class CThingAICreature;
class CCharString;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                          // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {    // vtable 0x01231C4C
    // __forceinline: under optimize("s") VC7.1 otherwise calls these small inline members out of line
    __forceinline CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    unsigned char _pad[0x10];               // +0x04..+0x13
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);  // 0x008FCEC0
};

struct CAIStateGroup_MinionGuardReactToPlayerWithWeapon : CAIStateGroupBase {
    void* m_pWarningSource;                 // +0x14
    long m_frameWarned;                     // +0x18
    long m_lastWarningId;                   // +0x1c
    CIntelligentPointer PPlayer;            // +0x20 CIntelligentPointer<CThingPlayerCreature>
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name);
};

void CAIStateGroup_MinionGuardReactToPlayerWithWeapon::Initialise(CAIBrain& brain, CThingAICreature& creature, long id, long priority, const CCharString& name)
{
    m_pWarningSource = 0;
    m_frameWarned = 0;
    m_lastWarningId = 0;
    PPlayer = 0;
    CAIStateGroupBase::Initialise(brain, creature, id, priority, name);
}