// CCreatureAction_CastWillHealLifeSpell::CCreatureAction_CastWillHealLifeSpell(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, 150, zeroed sound), Level 0, target set, animation "ST_CAST_HEAL" (or "ST_CAST_HEAL_SELF" without a target)
// Leaf: ??0CCreatureAction_CastWillHealLifeSpell@@QAE@PAUCThingCreatureBase@@PAUCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BBCC0, length 179 bytes (+ int3 padding), `ret 8`. Vtable 0x0127CA4C.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) 0x00858030, CBaseIntelligentPointer ctor
// 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C),
// CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("ST_CAST_HEAL" @ 0x0127CA20 / "ST_CAST_HEAL_SELF" @ 0x0127CA30), SetAnimationName 0x008574D0.
// Shape: as the in-chunk CCreatureAction_CastWillEnflameSpell constructors 0x008BB450 / 0x008BB500: init-list Level, the base target
// pointer (+0xa8) assigned in the body from a converting temporary, then the scoped named animation string. The literal is chosen by
// a ternary on the target parameter written null-first (`mov eax,SELF ; je ; mov eax,HEAL`).
// Layout: Level at retail +0x114 (PDB +0x12c).
struct CThingCreatureBase;
struct CThing;

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CSoundPair {                             // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CThing* PItem;                              // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0xa8 - 4];
    CIntelligentPointerThing PTarget;           // +0xa8 (the combat action's target)
    unsigned char _padb0[0x114 - 0xb0];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_CastWillHealLifeSpell : CCombatActionBase {   // vtable 0x0127CA4C
    long Level;                                 // +0x114 (PDB +0x12c)
    virtual void v00();
    CCreatureAction_CastWillHealLifeSpell(CThingCreatureBase* owner, CThing* target);
    CCreatureAction_CastWillHealLifeSpell(CThingCreatureBase* owner, CThing* target, long level);
};

CCreatureAction_CastWillHealLifeSpell::CCreatureAction_CastWillHealLifeSpell(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, 150, CSoundPair())
    , Level(0)
{
    PTarget = CIntelligentPointerThing(target);
    {
        CCharString name(target == 0 ? "ST_CAST_HEAL_SELF" : "ST_CAST_HEAL");
        SetAnimationName(name);
    }
}