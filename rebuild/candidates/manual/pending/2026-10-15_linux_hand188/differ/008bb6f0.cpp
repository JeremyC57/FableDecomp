// CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase*, long, bool, const CCharString&, long, float) -- base CCombatActionBase(owner, 150, zeroed sound), the cast fields from the arguments; a non-empty animation name is a one-anim cast with that animation, else "ST_RELEASE_PHYSICAL_SHIELD"
// Grade: DIFFER(215v212) -- the inline CCharString::operator==("") null-block strcmp intrinsic materialises the bool (`sete al ; test al,al`) where retail branches on the `repe cmpsb` flags directly | tried: if/else either way, __forceinline operator== | next: lane gap: inline bool-return branch threading (same in 008BB130 / 008BB200)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BB6F0, length 212 bytes (+ int3 padding), `ret 18h`. Vtable 0x0127BDCC.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) 0x00858030, CBasicString<char>::operator==
// (const char*) 0x004115A0 (the inline CCharString::operator== on a non-null block; a null block is the strcmp intrinsic against
// the pooled "" @ 0x0122D70E, `repe cmpsb`), SetAnimationName 0x008574D0, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0.
// Shape: init-list members in declaration order (ImmediateCast, LevelToCast, TimeLimit, ShieldHealth -- the float moved as a dword);
// the empty-name test is the landed CCharString::operator== inline (0x0099E960 family); the custom-name branch falls through, the
// default branch builds the scoped named literal string.
// Layout: as the landed sibling constructor 0x008BB670 (PDB header + 0x18 = retail offsets).
#include <string.h>

struct CThingCreatureBase;

#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {
    CBasicStringChar Data;                      // +0x00
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
};

struct CSoundPair {                             // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_CastPhysicalShieldSpell : CCombatActionBase {   // vtable 0x0127BDCC
    bool  ImmediateCast;                        // +0x114 (PDB +0x12c)
    bool  OneAnimCast;                          // +0x115 (PDB +0x12d)
    unsigned char _pad116[2];
    long  LevelToCast;                          // +0x118 (PDB +0x130)
    long  TimeLimit;                            // +0x11c (PDB +0x134)
    float ShieldHealth;                         // +0x120 (PDB +0x138)
    virtual void v00();
    CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase* owner, long levelToCast, bool immediateCast, const CCharString& animationName, long timeLimit, float shieldHealth);
};

CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase* owner, long levelToCast, bool immediateCast,
                                                                                 const CCharString& animationName, long timeLimit, float shieldHealth)
    : CCombatActionBase(owner, 150, CSoundPair())
    , ImmediateCast(immediateCast)
    , LevelToCast(levelToCast)
    , TimeLimit(timeLimit)
    , ShieldHealth(shieldHealth)
{
    if (!(animationName == "")) {
        OneAnimCast = true;
        SetAnimationName(animationName);
    } else {
        OneAnimCast = false;
        CCharString name("ST_RELEASE_PHYSICAL_SHIELD");
        SetAnimationName(name);
    }
}
