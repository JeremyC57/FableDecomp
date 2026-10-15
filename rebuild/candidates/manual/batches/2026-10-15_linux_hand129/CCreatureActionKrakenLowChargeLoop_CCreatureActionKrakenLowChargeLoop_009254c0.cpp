// CCreatureAction_KrakenLowChargeLoop::CCreatureAction_KrakenLowChargeLoop(CThingCreatureBase*, CThing*, long) -- base CCombatActionBase(owner, thing, 150, CSoundPair()), the argument and a word copied from the owner, animation "ATTACK_LOW_CHARGE_LOOP"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009254C0, length 119 bytes (+ int3 padding). Vtable 0x012909C4 (RTTI
// `?A0x29922355::CCreatureAction_KrakenLowChargeLoop`, anonymous namespace of the kraken TU).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the owner rides in edi for the base call and the later read of its word at +0xb4; both members are initialiser-list
// entries (stored after the vtable here); the name is a scoped local.
struct CThingCreatureBase {
    unsigned char _pad00[0xb4];
    long m_b4;                              // +0xb4 the owner word copied into the action (meaning not pinned down)
};
struct CThing;

struct CCharString {                        // one pointer; the default length makes a literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
struct CSoundPair {                         // a user constructor makes the 8-byte result return through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                  // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};
struct CCreatureAction_KrakenLowChargeLoop : CCombatActionBase {   // vtable 0x012909C4
    long m_value;                           // +0x114 (descriptive: the third argument)
    long m_ownerValue;                      // +0x118 copied from the owner's +0xb4
    virtual void v00();
    CCreatureAction_KrakenLowChargeLoop(CThingCreatureBase* owner, CThing* thing, long value);
};

CCreatureAction_KrakenLowChargeLoop::CCreatureAction_KrakenLowChargeLoop(CThingCreatureBase* owner, CThing* thing, long value)
    : CCombatActionBase(owner, thing, 150, CSoundPair())
    , m_value(value)
    , m_ownerValue(owner->m_b4)
{
    {
        CCharString name("ATTACK_LOW_CHARGE_LOOP", -1);
        SetAnimationName(name);
    }
}
