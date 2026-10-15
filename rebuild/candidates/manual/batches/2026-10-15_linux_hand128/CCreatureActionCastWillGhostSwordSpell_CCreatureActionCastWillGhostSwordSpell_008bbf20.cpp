// CCreatureAction_CastWillGhostSwordSpell::CCreatureAction_CastWillGhostSwordSpell(CThingCreatureBase*) -- will-spell combat action (action code 150, silent sound pair), spell level 0, animation "ST_CAST_SUMMON_GHOST_SWORD"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BBF20, length 106 bytes, `ret 4`. Vtable 0x0127CE7C.
// Callees: CCombatActionBase::CCombatActionBase(owner, actionCode, const CSoundPair&) @ 0x00858030,
//          CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0
//          (manifest label "CTCInGameMenu::SetPDef" is propagated), CCharString::~CCharString @ 0x0099EAE0.
// Shape: the CSoundPair temporary zeroed by its inline constructor (immediate stores: no second zero use shares a
//   register before the call), SpellLevel initialised to 0 after the vtable store, then the scoped named animation
//   string (constructor pushes scheduled above the vtable store) and the setter.
// Layout: engine/CCreatureAction_CastWillGhostSwordSpell.h is the PDB build (SpellLevel at +0x12c); the retail
//   CCombatActionBase is 0x114 bytes, so SpellLevel sits at +0x114 and the header is not used.
struct CThingCreatureBase;

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
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);                                           // 0x008574D0
};

struct CCreatureAction_CastWillGhostSwordSpell : CCombatActionBase {   // vtable 0x0127CE7C
    long SpellLevel;                        // +0x114 (PDB +0x12c)
    virtual void v00();
    CCreatureAction_CastWillGhostSwordSpell(CThingCreatureBase* owner);
};

CCreatureAction_CastWillGhostSwordSpell::CCreatureAction_CastWillGhostSwordSpell(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 150, CSoundPair())
    , SpellLevel(0)
{
    {
        CCharString name("ST_CAST_SUMMON_GHOST_SWORD", -1);
        SetAnimationName(name);
    }
}
