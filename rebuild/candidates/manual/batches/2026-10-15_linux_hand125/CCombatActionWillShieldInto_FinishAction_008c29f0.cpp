// CCombatAction_WillShieldInto::FinishAction() -- base FinishAction, then hand the creature a temporary CCreatureAction_CastPhysicalShieldSpell(creature, 2, true, "", shield frames, ShieldHealth) as its current action
// Leaf: ?FinishAction@CCombatAction_WillShieldInto
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C29F0, 144-byte slot (143 bytes of code). FinishAction is vtable slot 4.
// Callees: __ftol2 @ 0x00BFEA70, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, a named local
//          fetched before the base call), CCombatActionBase::FinishAction @ 0x00859A20 (direct base call),
//          CCharString::CCharString(const char*, long) @ 0x0099EBF0 (empty literal),
//          CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase*, long, bool,
//          const CCharString&, long, float) @ 0x008BB6F0, CThingCreatureBase::SetCurrentAction @ 0x006644F0,
//          the action temporary's destructor @ 0x0062E900, CCharString::~CCharString @ 0x0099EAE0.
// Globals: the game frames-per-second long @ 0x01375550.
// Shape: ShieldUpTime (seconds, retail +0x114 / PDB +0x12c) converts to frames first, kept in ebx across the calls:
// -1.0 (forever) maps to -1 (`or ebx,-1`), anything else to (long)(ShieldUpTime * fps) (`fild fps ; fmul`).
// The -1 test is an integer compare of the bit pattern (`cmp dword ptr [esi+114h],0BF800000h`, no FPU), so the
// source compares the float's bits. ShieldHealth (+0x118) is the float pushed last by value. The 0x128-byte frame
// holds the string word below the 0x124-byte action temporary (the cast-spell temporaries of 0x008C2A80 /
// 0x008C2980 are 0x11c bytes; the shield spell adds its two arguments as members).
struct CCreatureActionBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CThingCreatureBase_Calls {
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};

struct CCreatureAction_CastPhysicalShieldSpell_Retail {  // the retail temporary is 0x124 bytes: the 0x11c-byte cast-spell base + two words
    unsigned char _body[0x124];
    CCreatureAction_CastPhysicalShieldSpell_Retail(CThingCreatureBase_Calls* creature, long level, bool release,
                                                   const CCharString& animName, long shieldFrames, float shieldHealth);   // 0x008BB6F0
    ~CCreatureAction_CastPhysicalShieldSpell_Retail();                                                                // 0x0062E900
    operator const CCreatureActionBase&() const { return *reinterpret_cast<const CCreatureActionBase*>(this); }
};

extern long g_framesPerSecond;                  // 0x01375550

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThingCreatureBase_Calls* GetPItem() const; // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char _pad10[0x114 - 0x10];         // +0x10
    void FinishAction();                        // 0x00859A20
};

struct CCombatAction_WillShieldInto : CCombatActionBase {
    float ShieldUpTime;                         // +0x114 seconds, -1 = until broken
    float ShieldHealth;                         // +0x118
    void FinishAction();
};

void CCombatAction_WillShieldInto::FinishAction()
{
    long shieldFrames;
    if (*(const long*)&ShieldUpTime == 0xBF800000)
        shieldFrames = -1;
    else
        shieldFrames = (long)(ShieldUpTime * g_framesPerSecond);
    CThingCreatureBase_Calls* creature = PCreature.GetPItem();
    CCombatActionBase::FinishAction();
    CCharString animName("", -1);
    creature->SetCurrentAction(CCreatureAction_CastPhysicalShieldSpell_Retail(creature, 2, true, animName, shieldFrames, ShieldHealth));
}
