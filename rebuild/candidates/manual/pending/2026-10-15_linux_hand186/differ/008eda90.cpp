// CCombatAction_FallStunned::CCombatAction_FallStunned(CThingCreatureBase*, CThing*, const CCharString&) -- fall-stunned combat action: base (owner, the thing or else the owner, action code 250, empty sound pair), then the animation name, defaulting when empty
// Grade: DIFFER(165v169) -- the inline CCharString::operator==("") materialises a bool (`sete al ; test al,al`) where retail branches straight off the repe cmpsb / the operator== call; the open-coded test branches but rotates ebp/ebx and duplicates the default call | tried: operator== inline, __forceinline, operator!= with swapped arms, open-coded null/else-if test | next: one condition `if (p == 0 ? strcmp("", "") == 0 : p->Data == "")` with a single default arm
// Leaf: ??0CCombatAction_FallStunned
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EDA90, 176 bytes incl. padding (`ret 0Ch`). Own vtable 0x012880E4.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30,
//          CCombatActionBase::SetAnimName(const CCharString&) @ 0x008574D0 (manifest "CTCInGameMenu::SetPDef" is propagated),
//          CBasicString<char>::operator==(const char*) @ 0x004115A0, CCharString(const char*, long) @ 0x0099EBF0 / dtor @ 0x0099EAE0.
// Data: "" @ 0x0122D70E, the default animation name @ 0x0127717C.
// Shape: the thing defaults to the owner (`thing ? thing : owner`); the empty test is the inline
//          CCharString::operator==(const char*) (null block -> the strcmp intrinsic over "", a 1-byte repe cmpsb).
#include <string.h>
// CCharString model (Ego_r.pdb layout, byte-exact with retail, as the landed CCharString::operator!= 0x0099E960): a 4-byte
// handle on a 17-byte shared block {CBasicString<char> PData/StringLength/DataLength:31/UseFastExtend:1, NoRefs +0xd}.
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>, 13 bytes
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    unsigned long DataLength : 31;              // +0x08
    bool          UseFastExtend : 1;            // +0x0c
    bool operator==(const char* text) const;    // 0x004115A0
};
struct CCharStringData {                        // 17 bytes
    CBasicStringChar Data;                      // +0x00
    long             NoRefs;                    // +0x0d
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
    bool operator!=(const char* text) const { return !(*this == text); }
};

struct CThing { };
struct CThingCreatureBase : CThing { };
struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetAnimName(const CCharString& name);   // 0x008574D0
};
extern const char g_DefaultFallStunnedAnim[];  // 0x0127717C

struct CCombatAction_FallStunned : CCombatActionBase {   // vtable 0x012880E4
    virtual void v00();
    CCombatAction_FallStunned(CThingCreatureBase* owner, CThing* thing, const CCharString& animName);
};

CCombatAction_FallStunned::CCombatAction_FallStunned(CThingCreatureBase* owner, CThing* thing, const CCharString& animName)
    : CCombatActionBase(owner, thing ? thing : owner, 250, CSoundPair())
{
    if (animName.PStringData == 0) {
        if (strcmp("", "") == 0) {
            SetAnimName(CCharString(g_DefaultFallStunnedAnim, -1));
            return;
        }
    } else if (animName.PStringData->Data == "") {
        SetAnimName(CCharString(g_DefaultFallStunnedAnim, -1));
        return;
    }
    SetAnimName(animName);
}
