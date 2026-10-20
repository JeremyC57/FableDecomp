// CCreatureAction_CastWillForceSpell::CCreatureAction_CastWillForceSpell(CThingCreatureBase*, CThing*, long, bool, const CCharString&) -- targeted form: base CCombatActionBase(owner, target, 150, zeroed sound), ImmediateCast / LevelToCast from the arguments; a non-empty animation name is a one-anim cast with it, else "WILL_CAST_FORCE_SPELL_INTO"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BB130, length 197 bytes (+ int3 padding), `ret 14h`. Vtable 0x0127B50C.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) 0x00857F30,
// CBasicString<char>::operator==(const char*) 0x004115A0 (the inline CCharString compare on a non-null block; a null block is
// the strcmp intrinsic against the pooled "" @ 0x0122D70E, `repe cmpsb`), SetAnimationName 0x008574D0, CCharString ctor
// 0x0099EBF0 / dtor 0x0099EAE0.
// Shape: init-list members in declaration order (ImmediateCast, LevelToCast); the custom-name branch falls through with its own
// epilogue, the default branch builds the scoped named literal string.
// Layout: as the landed sibling constructor 0x008BB0B0 (PDB header + 0x18 = retail offsets).
#include <string.h>
// Retry 10: the `== ""` test is split by hand (strcmp on a null block, the block compare otherwise) with the arms joined by
// goto, as the landed 0x0048F7C0; the default name is an implicit CCharString conversion (homed in a dead argument slot).

struct CThingCreatureBase;
struct CThing;

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
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
    bool operator!=(const char* text) const { return !(*this == text); }
};

struct CSoundPair {                             // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                     // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);     // 0x00857F30
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

struct CCreatureAction_CastWillForceSpell : CCombatActionBase {   // vtable 0x0127B50C
    bool ImmediateCast;                         // +0x114 (PDB +0x12c)
    bool OneAnimCast;                           // +0x115 (PDB +0x12d)
    unsigned char _pad116[2];
    long LevelToCast;                           // +0x118 (PDB +0x130)
    virtual void v00();
    CCreatureAction_CastWillForceSpell(CThingCreatureBase* owner, CThing* target, long levelToCast, bool immediateCast, const CCharString& animationName);
};

CCreatureAction_CastWillForceSpell::CCreatureAction_CastWillForceSpell(CThingCreatureBase* owner, CThing* target, long levelToCast, bool immediateCast,
                                                                       const CCharString& animationName)
    : CCombatActionBase(owner, target, 150, CSoundPair())
    , ImmediateCast(immediateCast)
    , LevelToCast(levelToCast)
{
    if (animationName.PStringData == 0) {
        if (strcmp("", "") == 0)
            goto empty;
    } else if (animationName.PStringData->Data == "")
        goto empty;
    OneAnimCast = true;
    SetAnimationName(animationName);
    return;
empty:
    OneAnimCast = false;
    SetAnimationName("WILL_CAST_FORCE_SPELL_INTO");
}