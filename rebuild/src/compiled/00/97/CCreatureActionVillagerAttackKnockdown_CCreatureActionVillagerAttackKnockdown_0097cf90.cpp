// CCreatureAction_VillagerAttackKnockdown::CCreatureAction_VillagerAttackKnockdown(CThingCreatureBase*, CThing*, bool) -- base CCombatActionBase(owner, target, 150, GetSoundPair(owner, null name)), own vtable (0x01298F74), the flag at +0x114 from the argument
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0097CF90, length 96 bytes. The chunk label "CCreatureAction_SummonCreatures" is propagated from the byte-identical body @ 0x008B83E0: the vtable 0x01298F74 is CCreatureAction_VillagerAttackKnockdown (rtti-vtables.tsv).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (null text, default length -1: an implicit conversion of the
// null pointer, re-addressed with `lea`), GetSoundPair(CThingCreatureBase*, const CCharString&) @ 0x008561A0 (__fastcall, 8-byte
// class result through the hidden pointer in ecx), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long,
// const CSoundPair&) @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the landed 10-body GetSoundPair constructor family (0x008C4F50) with a null name; the byte argument is read after the
// string temporary dies and stored after the vtable. `ret 0Ch`: three arguments.
struct CThingCreatureBase;
struct CThing;

struct CCharString {                        // one pointer; the default length makes the null pointer convert implicitly
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
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

struct CCreatureAction_VillagerAttackKnockdown : CCombatActionBase {   // vtable 0x01298F74
    unsigned char m_isFromInventory;        // +0x114
    virtual void v00();
    CCreatureAction_VillagerAttackKnockdown(CThingCreatureBase* owner, CThing* target, bool isFromInventory);
};

CCreatureAction_VillagerAttackKnockdown::CCreatureAction_VillagerAttackKnockdown(CThingCreatureBase* owner, CThing* target, bool isFromInventory)
    : CCombatActionBase(owner, target, 150, GetSoundPair(owner, 0))
    , m_isFromInventory(isFromInventory)
{
}