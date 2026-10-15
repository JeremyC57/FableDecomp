// CCreatureAction_KrakenHighAttackLoop::CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase*, CThing*, long, bool) -- base CCombatActionBase(owner, thing, 150, CSoundPair()), the two settings, animation "ATTACK_HIGH_LOOP"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00925780, length 115 bytes (+ int3 padding). Vtable 0x0129107C (RTTI
// `?A0x29922355::CCreatureAction_KrakenHighAttackLoop`: the class lives in an anonymous namespace of the kraken TU).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the zeroed sound pair is built by its inline constructor (register-zeroed frame words, landed 0x008CBD50); both settings are
// initialiser-list members (the long at +0x118 is stored before the vtable, the byte at +0x114 after it); the name is a scoped local.
struct CThingCreatureBase;
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
struct CCreatureAction_KrakenHighAttackLoop : CCombatActionBase {   // vtable 0x0129107C
    bool m_flag;                            // +0x114 (descriptive: the fourth argument)
    long m_value;                           // +0x118 (descriptive: the third argument)
    virtual void v00();
    CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase* owner, CThing* thing, long value, bool flag);
};

CCreatureAction_KrakenHighAttackLoop::CCreatureAction_KrakenHighAttackLoop(CThingCreatureBase* owner, CThing* thing, long value, bool flag)
    : CCombatActionBase(owner, thing, 150, CSoundPair())
    , m_flag(flag)
    , m_value(value)
{
    {
        CCharString name("ATTACK_HIGH_LOOP", -1);
        SetAnimationName(name);
    }
}
