// CCreatureAction_CastPhysicalShieldSpell::CCreatureAction_CastPhysicalShieldSpell(CThingCreatureBase*, long, bool, const CCharString&, long, float) -- base CCombatActionBase(owner, 150, zeroed sound), the cast fields from the arguments; a non-empty animation name is a one-anim cast with that animation, else "ST_RELEASE_PHYSICAL_SHIELD"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BB6F0, length 212 bytes (+ int3 padding), `ret 18h`. Vtable 0x0127BDCC.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) 0x00858030, CBasicString<char>::operator==
// (const char*) 0x004115A0 (the inline CCharString::operator== on a non-null block; a null block is the strcmp intrinsic against
// the pooled "" @ 0x0122D70E, `repe cmpsb`), SetAnimationName 0x008574D0, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0.
// Shape: init-list members in declaration order (ImmediateCast, LevelToCast, TimeLimit, ShieldHealth -- the float moved as a dword);
// the empty-name test is the landed CCharString::operator== inline (0x0099E960 family); the custom-name branch falls through, the
// default branch builds the scoped named literal string.
// Layout: as the landed sibling constructor 0x008BB670 (PDB header + 0x18 = retail offsets).
#include <string.h>
// Retry 10: the `== ""` test is split by hand (strcmp on a null block, the block compare otherwise) with the arms joined by
// goto, as the landed 0x0048F7C0; the default name is an implicit CCharString conversion (homed in a dead argument slot).

class CThingCreatureBase;

#pragma pack(push, 1)
template <class T> class CBasicString { public:                       // CBasicString<char>
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    bool operator==(const char* text) const;    // 0x004115A0
};
class CCharStringData { public:
    CBasicString<char> Data;                      // +0x00
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

class CSoundPair { public:                             // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_CastPhysicalShieldSpell : public CCombatActionBase { public:   // vtable 0x0127BDCC
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
    if (animationName.PStringData == 0) {
        if (strcmp("", "") == 0)
            goto empty;
    } else if (animationName.PStringData->Data == "")
        goto empty;
    OneAnimCast = true;
    SetCombatAnimation(animationName);
    return;
empty:
    OneAnimCast = false;
    SetCombatAnimation("ST_RELEASE_PHYSICAL_SHIELD");
}