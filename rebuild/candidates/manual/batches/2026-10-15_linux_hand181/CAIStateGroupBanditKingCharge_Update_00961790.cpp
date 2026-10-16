// CAIStateGroup_BanditKingCharge::Update() -- with a live target: run at it (MoveToThingProximity, 4m), and on arrival with the target still alive perform the "BK_CHARGE_ATTACK" attack action; terminate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00961790, length 239 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer ctor/GetPItem/SetPItem @ 0x00A01B10/0x00A01B50/0x00A01B90, CAIStateGroupBase::
// AIAction_MoveToThingProximity @ 0x0097E380 (CIntelligentPointer<const CThing> by value, vtable 0x01237F1C),
// CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CCreatureAction_BanditKingAttack
// ctor @ 0x008C9410 (creature, target, const CCharString& anim, bool, bool), CAIStateGroupBase::
// AIAction_CombatPerformActionAndWait @ 0x0097DA40, CCombatActionBase::~CCombatActionBase @ 0x0062E900,
// CAIStateGroupBase::Terminate @ 0x008FCF40.
// Shape: the intelligent pointer argument is built in the argument area through an inline converting copy ctor
// from PTargetThing (base ctor, vtable store, then GetPItem -- a T* ctor evaluates GetPItem first); a one-byte option
// temporary is passed by reference; the animation name is a named local built before the creature is read.
struct CThing
{
    unsigned char _pad[0x91];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
};

struct CBaseIntelligentPointer
{
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer
{
    virtual void v00();
    __forceinline CIntelligentPointer(T* item) { SetPItem((CThing*)item); }
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
    __forceinline CIntelligentPointer(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }   // converting copy
};

__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0)
    {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

struct CCharString
{
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

struct CMoveOptions                                 // one-byte option object passed by reference
{
    CMoveOptions() { }
};

struct CCreatureActionBase
{
    virtual void v00();
    unsigned char _body[0xa4];                      // +0x04..+0xa7
};

struct CCombatActionBase : CCreatureActionBase
{
    unsigned char _combat[0x6c];                    // +0xa8..+0x113
    ~CCombatActionBase();                           // 0x0062E900
};

struct CCreatureAction_BanditKingAttack : CCombatActionBase
{
    unsigned char _own[0x04];
    CCreatureAction_BanditKingAttack(CThing* creature, CThing* target, const CCharString& anim, bool a, bool b);   // 0x008C9410
};

struct CAIBrain
{
    unsigned char _pad[0x18];
    CThing* Creature;                               // +0x18
};

struct CAIStateGroup_BanditKingCharge
{
    virtual void v00();
    CAIBrain* PBrain;                               // +0x04
    unsigned char _pad08[0x0c];                     // +0x08
    CBaseIntelligentPointer PTargetThing;           // +0x14
    long AIAction_MoveToThingProximity(long mode, CIntelligentPointer<const CThing> target, float proximity,
                                       const CMoveOptions& options, long anim, bool a, bool b, bool c, bool d,
                                       bool e);     // 0x0097E380
    long AIAction_CombatPerformActionAndWait(CCreatureActionBase* action, bool wait);   // 0x0097DA40
    void Terminate();                               // 0x008FCF40
    void Update();
};

void CAIStateGroup_BanditKingCharge::Update()
{
    if (IS_THING_ALIVE(PTargetThing)
        && AIAction_MoveToThingProximity(1, CIntelligentPointer<const CThing>(PTargetThing), 4.0f,
                                         CMoveOptions(), 5, true, false, true, true, false) == 2
        && IS_THING_ALIVE(PTargetThing))
    {
        CCharString anim("BK_CHARGE_ATTACK", -1);
        CThing* creature = PBrain->Creature;
        AIAction_CombatPerformActionAndWait(
            &CCreatureAction_BanditKingAttack(creature, PTargetThing.GetPItem(), anim, true, false), false);
    }
    Terminate();
}
