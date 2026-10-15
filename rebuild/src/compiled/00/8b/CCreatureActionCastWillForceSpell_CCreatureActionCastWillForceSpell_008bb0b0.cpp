// CCreatureAction_CastWillForceSpell::CCreatureAction_CastWillForceSpell(CThingCreatureBase*, CThing*) -- will-spell combat action on a target (action code 150, silent sound pair): not immediate, not one-anim, level -1, animation "WILL_CAST_FORCE_SPELL_INTO"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BB0B0, length 119 bytes, `ret 8`. Vtable 0x0127B50C.
// Callees: CCombatActionBase::CCombatActionBase(owner, target, actionCode, const CSoundPair&) @ 0x00857F30,
//          CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0
//          (manifest label "CTCInGameMenu::SetPDef" is propagated), CCharString::~CCharString @ 0x0099EAE0.
// Shape: the CSoundPair temporary is zeroed by its inline constructor from ebx, which also serves the two bool
//   initialisers after the vtable store (ImmediateCast, OneAnimCast) -- the landed CastBattleCharge shape; LevelToCast
//   is the -1 immediate; the animation name is a scoped named CCharString whose constructor pushes are scheduled above
//   the vtable store.
// Layout: engine/CCreatureAction_CastWillForceSpell.h is the PDB build (members at +0x12c): the retail CCombatActionBase
//   is 0x114 bytes, so ImmediateCast/OneAnimCast/LevelToCast sit at +0x114/+0x115/+0x118 and the header is not used.
struct CThingCreatureBase;
struct CThing;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetAnimationName(const CCharString& name);                                                            // 0x008574D0
};

struct CCreatureAction_CastWillForceSpell : CCombatActionBase {   // vtable 0x0127B50C
    bool ImmediateCast;                     // +0x114 (PDB +0x12c)
    bool OneAnimCast;                       // +0x115
    long LevelToCast;                       // +0x118
    virtual void v00();
    CCreatureAction_CastWillForceSpell(CThingCreatureBase* owner, CThing* target);
};

CCreatureAction_CastWillForceSpell::CCreatureAction_CastWillForceSpell(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , ImmediateCast(false)
    , OneAnimCast(false)
    , LevelToCast(-1)
{
    {
        CCharString name("WILL_CAST_FORCE_SPELL_INTO", -1);
        SetAnimationName(name);
    }
}