// CCreatureAction_AlertAllNearbyToThing::CCreatureAction_AlertAllNearbyToThing(CThingCreatureBase*, long, long) -- base CCreatureActionBase(owner, 0.0, 0.0, the owner's random loaded "ALARM" animation, zeroed CSoundPair, 0, 1), own vtable (0x012736A4), the two words at +0xa8/+0xac from the arguments
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00842370, length 121 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the random-loaded-animation lookup @ 0x006924B0
// (`__fastcall`: ecx owner, edx name; a thunk onto CThingCreatureBase::GetRandomLoadedAnimFromName(name, true) @ 0x00662FA0),
// CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*, const CSoundPair&, long, long)
// @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the animation name is an implicit conversion temporary built first and re-addressed with `lea`; base arguments
// right to left (`push 1 ; push 0`, the zeroed sound pair, the lookup, 0.0f, 0.0f, the owner); the temporary dies after
// the base constructor; the two argument words are loaded before `pop edi`, stored after the vtable. `ret 0Ch`.
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

struct CCreatureAction_AlertAllNearbyToThing : CCreatureActionBase {   // vtable 0x012736A4
    long m_thingToAlertTo;                  // +0xa8: the thing (UID word) the nearby creatures are alerted to
    long m_alertRadius;                     // +0xac: the second argument word
    virtual void v00();
    CCreatureAction_AlertAllNearbyToThing(CThingCreatureBase* owner, long thingToAlertTo, long alertRadius);
};

CCreatureAction_AlertAllNearbyToThing::CCreatureAction_AlertAllNearbyToThing(CThingCreatureBase* owner, long thingToAlertTo, long alertRadius)
    : CCreatureActionBase(owner, 0.0f, 0.0f, GetRandomLoadedAnimation(owner, "ALARM"), CSoundPair(), 0, 1)
    , m_thingToAlertTo(thingToAlertTo)
    , m_alertRadius(alertRadius)
{
}
