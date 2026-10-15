// CCreatureAction_BeStunned::CCreatureAction_BeStunned(CThingCreatureBase*, bool) -- base CCreatureActionBase(owner, -1.0, 0.0, the owner's random loaded "KNOCKDOWN_IDLE" animation, zeroed CSoundPair, 0, 0), own vtable (0x01276E24), the flag byte at +0xa8 from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00865940, length 114 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the random-loaded-animation lookup @ 0x006924B0
// (`__fastcall`: ecx owner, edx name; a thunk onto CThingCreatureBase::GetRandomLoadedAnimFromName(name, true) @ 0x00662FA0),
// CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*, const CSoundPair&, long, long)
// @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the animation name is an implicit conversion temporary built first and re-addressed with `lea`; base arguments
// right to left (0, 0, the zeroed sound pair, the lookup, 0.0f as `push 0`, -1.0f, the owner); the temporary dies after
// the base constructor; the argument byte is loaded (`mov cl`) before `pop edi` and stored after the vtable. `ret 8`.
// Layout: retail CCreatureActionBase is 0xa8 bytes; no engine header exists for this class, so `this` is modelled locally.
struct CThingCreatureBase;
struct CAnimationEntry;

struct CCharString {                        // one pointer; the defaulted length lets the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (frame stores before the base call)
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CAnimationEntry* __fastcall GetRandomLoadedAnimation(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

struct CCreatureAction_BeStunned : CCreatureActionBase {   // vtable 0x01276E24
    bool m_stunnedByHero;                   // +0xa8: the flag argument (read back by Initialise @ 0x008EDA10's sibling)
    virtual void v00();
    CCreatureAction_BeStunned(CThingCreatureBase* owner, bool stunnedByHero);
};

CCreatureAction_BeStunned::CCreatureAction_BeStunned(CThingCreatureBase* owner, bool stunnedByHero)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetRandomLoadedAnimation(owner, "KNOCKDOWN_IDLE"), CSoundPair(), 0, 0)
    , m_stunnedByHero(stunnedByHero)
{
}
