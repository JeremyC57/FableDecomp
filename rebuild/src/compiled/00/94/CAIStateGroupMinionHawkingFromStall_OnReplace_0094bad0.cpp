// CAIStateGroup_MinionHawkingFromStall::OnReplace() -- drop the stall pointer and stop the creature's speech.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094BAD0, length 77 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice),
// GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10 on the converting temporary (vtable 0x0123E7D8 =
// CIntelligentPointer<CThingCreatureBase>), then the creature's StopSpeaking @ 0x00864580 (manifest: __fastcall
// taking the creature in ecx -- modelled as a no-argument member, which emits the same `mov ecx ; call`).
// `PStall = 0` on the member at +0x18 is the converting-temporary + inline `SetPItem(other.GetPItem())` shape;
// the temporary is destroyed before the creature call. No engine header; layout local (+0x14 is a long
// before the pointer), names descriptive.
class CThing;

class CThingCreatureBase { public:
    void StopSpeaking();                    // 0x00864580
};

class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                          // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    CBaseObject* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
    virtual ~CBaseIntelligentPointer();             // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {    // vtable 0x0123E7D8
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(((CThing*)other.GetPItem())); return *this; }
};

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThingCreatureBase* Creature;           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                       // +0x04
    unsigned char _pad[0xc];                // +0x08..+0x13
};

struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroupBase {
    long m_hawkState;                       // +0x14
    CIntelligentPointer PStall;             // +0x18 CIntelligentPointer<CThingCreatureBase>
    virtual void OnReplace();
};

void CAIStateGroup_MinionHawkingFromStall::OnReplace()
{
    PStall = 0;
    PBrain->Creature->StopSpeaking();
}