// CCreatureAction_ScreamerDie::CCreatureAction_ScreamerDie() -- base CCombatActionBase(250), own vtable (0x0127B374), the CIntelligentPointer at +0x114 default-constructed (base CBaseIntelligentPointer, its vtable 0x01238C6C, SetPItem(0))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B9110, length 53 bytes.
// Callees: CCombatActionBase::CCombatActionBase(long) @ 0x00857E40, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: no arguments (`ret`); the family-44 intelligent-pointer default constructor is inlined after this class's vtable store.
struct CThingCreatureBase; struct CThing; struct CCharString; struct CBaseObject;
struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (register-zeroed stack stores)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(long actionCode);                                                                         // 0x00857E40
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseIntelligentPointer();                  // 0x00A01B10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { SetPItem(0); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CCreatureAction_ScreamerDie : CCombatActionBase {   // vtable 0x0127B374
    CIntelligentPointer m_target;           // +0x114
    virtual void v00();
    CCreatureAction_ScreamerDie();
};

CCreatureAction_ScreamerDie::CCreatureAction_ScreamerDie()
    : CCombatActionBase(250)
    , m_target()
{
}
