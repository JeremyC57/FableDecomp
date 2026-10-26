// CCreatureAction_FodderSecondaryAttack::CCreatureAction_FodderSecondaryAttack(CThingCreatureBase&, CThing&) -- combat action on a target: base CCombatActionBase(owner, target, 150, GetSoundPair(owner, null name)), then its own vtable
// Leaf: ??0CCreatureAction_FodderSecondaryAttack@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00907440, 84 bytes (96 with int3 padding), thiscall `ret 8`.
// Identity (proven): CCreatureAction_FodderSecondaryAttack::CCreatureAction_FodderSecondaryAttack; evidence: the body stores vtable 0x0128DE54, whose RTTI is .?AVCCreatureAction_FodderSecondaryAttack@@; the
//   two-reference constructor ??0CCreatureAction_FodderSecondaryAttack@@QAE@AAVCThingCreatureBase@@AAVCThing@@@Z is in FableWin.pdb (the class's other constructor is the copy constructor).
// Label: the manifest's "CCreatureAction_DragonRoar" is propagated across family H030 (seven combat-action constructors that differ only
//   in the vtable).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, GetSoundPair (__fastcall) 0x008561A0,
//   CCombatActionBase::CCombatActionBase(CThingCreatureBase&, CThing&, long, const CSoundPair&) 0x00857F30 (FableWin.pdb
//   ??0CCombatActionBase@@QAE@AAVCThingCreatureBase@@AAVCThing@@JABVCSoundPair@@@Z), CCharString::~CCharString 0x0099EAE0.
// Shape: the landed CCreatureAction_MidAttack360 0x0090A700 model with the sound name a null pointer converted implicitly
//   (`push -1 ; push 0`, as the landed CCreatureAction_NymphSummonOutOf 0x008CA600); the 8-byte CSoundPair comes back through
//   the hidden pointer, the string temporary dies after the base constructor, then the vtable store.
class CThing { };
class CThingCreatureBase : public CThing { };

struct CCharString {                     // one pointer; the default length makes the literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                        // 0x0099EAE0
};

struct CSoundPair {                       // a user constructor makes the 8-byte result return through the hidden pointer
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

class CCombatActionBase {
public:
    virtual void v00();
    CCombatActionBase(CThingCreatureBase& owner, CThing& thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

class CCreatureAction_FodderSecondaryAttack : public CCombatActionBase {
public:
    virtual void v00();
    CCreatureAction_FodderSecondaryAttack(CThingCreatureBase& owner, CThing& target);
};

CCreatureAction_FodderSecondaryAttack::CCreatureAction_FodderSecondaryAttack(CThingCreatureBase& owner, CThing& target)
    : CCombatActionBase(owner, target, 150, GetSoundPair(&owner, (const char*)0))
{
}