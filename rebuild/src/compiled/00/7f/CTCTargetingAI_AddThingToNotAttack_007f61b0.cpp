// CTCTargetingAI::AddThingToNotAttack(CThing* thing) -- append the thing to the "things not to attack" list unless it is already on it
// Leaf: AddThingToNotAttack
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F61B0, length 78 bytes (caller 0x00747D3B).
// Callees: CTCTargetingAI::IsThingAttackable(const CThing*) @ 0x007F6170 (landed: true unless the thing is on the list),
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90,
// list<CIntelligentPointer<CThing> >::push_back(const T&) @ 0x004BA4E0, ~CBaseIntelligentPointer @ 0x00A01C10.
// Vtable 0x01238C6C = CIntelligentPointer (rtti-vtables.tsv).
// Label correction: the chunk calls this CTCSolidToThing::AddThingNotSolidTo (propagated from the byte-identical shape
// at 0x00770FA0); CTCSolidToThing is 0x1c bytes, while the list here is read at +0x48 = PDB CTCTargetingAI
// ThingsToNotAttack, and the guard is the landed CTCTargetingAI::IsThingAttackable.
// Shape: as CTCSolidToThing::AddThingNotSolidTo 0x00770FA0 -- an 8-byte stack CIntelligentPointer built by its inline
// converting constructor, pushed by reference and destroyed through the base destructor.
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                             // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* thing) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)thing); }
};

struct CThingPointerList {                                  // retail std::list: one sentinel pointer
    void* m_pHead;
    void push_back(const CIntelligentPointer& value);       // 0x004BA4E0
};

class CTCTargetingAI { public:
    unsigned char           _base[0x38];                    // CTCTargetingBase
    CBaseIntelligentPointer PBestEnemyCreatureForAttack;    // +0x38
    CBaseIntelligentPointer PScriptTargetOverride;          // +0x40
    CThingPointerList       ThingsToNotAttack;              // +0x48
    bool IsThingAttackable(const CThing* thing) const;      // 0x007F6170
    void AddThingToNotAttack(CThing& thing);
};

void CTCTargetingAI::AddThingToNotAttack(CThing& thing)
{
    if (IsThingAttackable(&thing)) {
        CIntelligentPointer pointer(&thing);
        ThingsToNotAttack.push_back(pointer);
    }
}