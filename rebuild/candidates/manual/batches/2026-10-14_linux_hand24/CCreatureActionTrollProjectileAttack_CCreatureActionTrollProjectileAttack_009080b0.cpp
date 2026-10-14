// CCreatureAction_TrollProjectileAttack::CCreatureAction_TrollProjectileAttack(CThingCreatureBase*, CThing*, const CCharString&) -- base CCombatActionBase(owner, target, 150, zeroed CSoundPair), OverriddenAnim set, own vtable, AnimName copy-constructed from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009080B0, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30,
// CCharString::CCharString(const CCharString&) @ 0x0099EC30 (the member at +0x118 built in the init list).
// Label: the manifest says CCreatureAction_DragonFlight (propagated); the stored vtable 0x0128E3AC is
// CCreatureAction_TrollProjectileAttack's (rtti-vtables.tsv). OverriddenAnim / AnimName are the engine-header names
// (PDB +0x12c/+0x130; retail CCombatActionBase is 0x114 bytes). The string is modelled locally because fable_string.h
// declares no copy constructor. `ret 0Ch`: three arguments.
struct CThingCreatureBase;
struct CThing;

struct CCharString {                        // one pointer to the shared string data
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
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
};

struct CCreatureAction_TrollProjectileAttack : CCombatActionBase {   // vtable 0x0128E3AC
    unsigned char OverriddenAnim;           // +0x114 (bool)
    CCharString   AnimName;                 // +0x118
    virtual void v00();
    CCreatureAction_TrollProjectileAttack(CThingCreatureBase* owner, CThing* target, const CCharString& animName);
};

CCreatureAction_TrollProjectileAttack::CCreatureAction_TrollProjectileAttack(CThingCreatureBase* owner, CThing* target, const CCharString& animName)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , OverriddenAnim(1)
    , AnimName(animName)
{
}
