// CCreatureAction_DragonRoar::CCreatureAction_DragonRoar(CThingCreatureBase*, CThing*) -- base CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING")), then own vtable (0x012937EC)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00953300, length 87 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair @ 0x008561A0 (__fastcall, 8-byte class
// result through a hidden pointer), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&)
// @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the landed GetSoundPair-constructor family (0x008C4F50) without a name setter: the literal converts implicitly to a
// CCharString temporary (re-addressed with `lea`), the CSoundPair result lands in a frame temporary and is passed by
// reference to the base; the string dies after the base call, then the vtable is stored. The string is modelled locally
// so the literal converts implicitly (fable_string.h has no defaulted length). `ret 8`: two arguments.
struct CThingCreatureBase;
struct CThing;

struct CCharString {                        // one pointer; the default length makes the literal convert implicitly
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

struct CCombatActionBase {
    virtual void v00();
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

struct CCreatureAction_DragonRoar : CCombatActionBase {   // vtable 0x012937EC
    virtual void v00();
    CCreatureAction_DragonRoar(CThingCreatureBase* owner, CThing* target);
};

CCreatureAction_DragonRoar::CCreatureAction_DragonRoar(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING"))
{
}