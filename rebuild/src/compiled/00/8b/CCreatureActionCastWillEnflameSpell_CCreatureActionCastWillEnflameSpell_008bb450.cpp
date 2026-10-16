// CCreatureAction_CastWillEnflameSpell::CCreatureAction_CastWillEnflameSpell(CThingCreatureBase*, long) -- untargeted form: base CCombatActionBase(owner, 150, zeroed sound), Level, IsZTargetting clear, target cleared, animation "ST_RELEASE_ENFLAME"
// Leaf: ??0CCreatureAction_CastWillEnflameSpell@@QAE@PAUCThingCreatureBase@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BB450, length 171 bytes (+ int3 padding), `ret 8`. Vtable 0x0127B9DC.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) 0x00858030, CBaseIntelligentPointer ctor
// 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C),
// CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("ST_RELEASE_ENFLAME" @ 0x0127B9C0), SetAnimationName 0x008574D0.
// Shape: init-list Level / IsZTargetting; the base target pointer (+0xa8) is assigned in the body from a converting temporary
// (`PTarget = CIntelligentPointerThing(x)`: temporary in the dead sound-pair slot); the scoped named animation string lands in
// the owner's dead argument slot.
// Layout: Level / IsZTargetting at retail +0x114 / +0x118 (PDB +0x12c / +0x130).
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

struct CCreatureAction_CastWillEnflameSpell : CCombatActionBase {   // vtable 0x0127B9DC
    long Level;                                 // +0x114 (PDB +0x12c)
    bool IsZTargetting;                         // +0x118 (PDB +0x130)
    virtual void v00();
    CCreatureAction_CastWillEnflameSpell(CThingCreatureBase* owner, long level);
    CCreatureAction_CastWillEnflameSpell(CThingCreatureBase* owner, CThing* target, bool isZTargetting, long level);
};

CCreatureAction_CastWillEnflameSpell::CCreatureAction_CastWillEnflameSpell(CThingCreatureBase* owner, long level)
    : CCombatActionBase(owner, 150, CSoundPair())
    , Level(level)
    , IsZTargetting(false)
{
    PTarget = CIntelligentPointerThing(0);
    {
        CCharString name("ST_RELEASE_ENFLAME");
        SetAnimationName(name);
    }
}