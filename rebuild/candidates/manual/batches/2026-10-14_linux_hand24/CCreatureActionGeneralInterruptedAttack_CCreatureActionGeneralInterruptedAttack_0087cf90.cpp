// CCreatureAction_GeneralInterruptedAttack::CCreatureAction_GeneralInterruptedAttack(CThingCreatureBase*, CThing*, CCharString) -- base CCombatActionBase(owner, target, 200, zeroed CSoundPair), own vtable, the by-value name handed to the base setter, then its destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087CF90, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30,
// CCombatActionBase::SetAnimationName @ 0x008574D0 (propagated manifest label), CCharString::~CCharString @ 0x0099EAE0.
// Shape: the third argument is a CCharString BY VALUE -- its address is taken in the argument area (`lea eax,[esp+18h]`)
// for the setter and the destructor runs on that slot before the return (a reference would be pushed from the slot
// instead). The class adds no data (vtable 0x01277A14). `ret 0Ch`: three argument slots.
struct CThingCreatureBase;
struct CThing;

struct CCharString {                        // one pointer to the shared string data; a user destructor makes the
    void* m_pData;                          //   by-value parameter a real object in its slot
    CCharString(const CCharString& other);  // 0x0099EC30 (the caller's copy, not part of this body)
    ~CCharString();                         // 0x0099EAE0
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_GeneralInterruptedAttack : CCombatActionBase {   // vtable 0x01277A14
    virtual void v00();
    CCreatureAction_GeneralInterruptedAttack(CThingCreatureBase* owner, CThing* target, CCharString animName);
};

CCreatureAction_GeneralInterruptedAttack::CCreatureAction_GeneralInterruptedAttack(CThingCreatureBase* owner, CThing* target, CCharString animName)
    : CCombatActionBase(owner, target, 200, CSoundPair())
{
    SetAnimationName(animName);
}
