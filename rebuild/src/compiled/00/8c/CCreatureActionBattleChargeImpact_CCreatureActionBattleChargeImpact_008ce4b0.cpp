// CCreatureAction_BattleChargeImpact::CCreatureAction_BattleChargeImpact(CThingCreatureBase*, CIntelligentPointer<CThing>, CIntelligentPointer<CThing>) -- base CCombatActionBase(owner, 145, zeroed sound), copy the two by-value thing pointers into the members, animation "ST_BATTLE_CHARGE_IMPACT"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CE4B0, length 182 bytes (+ int3 padding), `ret 14h` (owner + two 8-byte by-value pointers).
// Vtable 0x01284664 (own class); CIntelligentPointer<CThing> vtable 0x01238C6C.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) 0x00858030, CBaseIntelligentPointer
// ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0,
// SetAnimationName 0x008574D0 (manifest label "CTCInGameMenu::SetPDef" is propagated; named as in the landed 0x008CE630 family).
// Shape: the two members are copy-constructed from the by-value parameters (inline copy ctor: base ctor, derived vtable, the
// source's GetPItem, SetPItem); the scoped named animation string; the by-value parameters are destroyed by the callee at the end.
// Layout: retail CCombatActionBase is 0x114 bytes (the PDB one is 0xc larger), so the two pointers sit at +0x114 / +0x11c.
struct CThingCreatureBase;
struct CBaseObject;

struct CCharString {                        // engine string (4 bytes: data pointer)
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CBaseIntelligentPointer {            // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* PItem;                     // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    ~CBaseIntelligentPointer();             // 0x00A01C10
    CBaseObject* GetPItem() const;          // 0x00A01B50
    void SetPItem(CBaseObject* item);       // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(const CIntelligentPointerThing& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);                                           // 0x008574D0
};

struct CCreatureAction_BattleChargeImpact : CCombatActionBase {   // vtable 0x01284664
    CIntelligentPointerThing Left;    // +0x114 (PDB +0x12c)
    CIntelligentPointerThing Right;   // +0x11c (PDB +0x134)
    virtual void v00();
    CCreatureAction_BattleChargeImpact(CThingCreatureBase* owner, CIntelligentPointerThing left, CIntelligentPointerThing right);
};

CCreatureAction_BattleChargeImpact::CCreatureAction_BattleChargeImpact(CThingCreatureBase* owner, CIntelligentPointerThing left, CIntelligentPointerThing right)
    : CCombatActionBase(owner, 145, CSoundPair())
    , Left(left)
    , Right(right)
{
    {
        CCharString name("ST_BATTLE_CHARGE_IMPACT");
        SetAnimationName(name);
    }
}