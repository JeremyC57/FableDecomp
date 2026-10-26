// Behaviour test for 008eda90: src/008eda90.cpp copied verbatim; the fall-stunned action is built on the base with the
// thing (or the owner when null), action code 250 and an empty sound pair, and takes the given animation name unless it
// is empty (null block or ""), in which case the default name is used.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCombatAction_FallStunned::CCombatAction_FallStunned(CThingCreatureBase*, CThing*, const CCharString&) -- fall-stunned combat action: base (owner, the thing or else the owner, action code 250, empty sound pair), then the animation name, defaulting when empty
// Leaf: ??0CCombatAction_FallStunned
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EDA90, 176 bytes incl. padding (`ret 0Ch`). Own vtable 0x012880E4.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30,
//          CCombatActionBase::SetAnimName(const CCharString&) @ 0x008574D0 (manifest "CTCInGameMenu::SetPDef" is propagated),
//          CBasicString<char>::operator==(const char*) @ 0x004115A0, CCharString(const char*, long) @ 0x0099EBF0 / dtor @ 0x0099EAE0.
// Data: "" @ 0x0122D70E, the default animation name @ 0x0127717C.
// Shape: the thing defaults to the owner (`thing ? thing : owner`); the empty test is the natural `if (animName == "")`
//          with the inline CCharString::operator==(const char*) whose null-block arm is `if (strcmp(text, "") == 0)
//          return true; return false;` (VC7.1 branches off the 1-byte repe cmpsb flags); the default name is a named local
//          (its address is re-taken for the SetAnimName push, as retail).
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
class CCharStringData { public:                        // 17 bytes
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
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
    bool operator!=(const char* text) const { return !(*this == text); }
};

class CThing { public: };
class CThingCreatureBase : public CThing { public: };
class CSoundPair { public:
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetAnimName(const CCharString& name);   // 0x008574D0
};
extern const char g_DefaultFallStunnedAnim[];  // 0x0127717C

class CCombatAction_FallStunned : public CCombatActionBase { public:   // vtable 0x012880E4
    virtual void v00();
    CCombatAction_FallStunned(CThingCreatureBase* owner, CThing* thing, const CCharString& animName);
};

CCombatAction_FallStunned::CCombatAction_FallStunned(CThingCreatureBase* owner, CThing* thing, const CCharString& animName)
    : CCombatActionBase(owner, thing ? thing : owner, 250, CSoundPair())
{
    if (animName == "") {
        CCharString name(g_DefaultFallStunnedAnim, -1);
        SetAnimName(name);
    } else
        SetAnimName(animName);
}
// ---- end source ----
const char g_DefaultFallStunnedAnim[] = "FALL_STUNNED_DEFAULT";
static CThingCreatureBase* g_owner; static CThing* g_target; static long g_code, g_s1 = -1, g_s2 = -1;
static const CCharStringData* g_setData; static const char* g_setText; static int g_sets, g_ctors, g_dtors;
static CCharStringData g_defaultBlock;
CCombatActionBase::CCombatActionBase(CThingCreatureBase* owner, CThing* target, long code, const CSoundPair& sound)
{ g_owner = owner; g_target = target; g_code = code; g_s1 = sound.FirstSound; g_s2 = sound.LastSound; }
void CCombatActionBase::SetAnimName(const CCharString& name) { ++g_sets; g_setData = name.PStringData; g_setText = name.PStringData ? name.PStringData->Data.PData : 0; }
void CCombatActionBase::v00() {}
void CCombatAction_FallStunned::v00() {}
CCharString::CCharString(const char* text, long) { ++g_ctors; g_defaultBlock.Data.PData = (char*)text; PStringData = &g_defaultBlock; }
CCharString::~CCharString() { ++g_dtors; }
bool CBasicStringChar::operator==(const char* text) const { return strcmp(PData, text) == 0; }
static CCharStringData MakeData(char* text) { CCharStringData d; memset(&d, 0, sizeof(d)); d.Data.PData = text; d.Data.StringLength = (unsigned long)strlen(text); return d; }
int main()
{
    static char tRoll[] = "ROLL", tEmpty[] = "";
    CCharStringData dRoll = MakeData(tRoll), dEmpty = MakeData(tEmpty);
    CThingCreatureBase owner; CThing target;
    static CCharStringData* handles[3]; handles[0] = &dRoll; handles[1] = &dEmpty; handles[2] = 0;
    const CCharString& roll = *(const CCharString*)&handles[0];
    const CCharString& empty = *(const CCharString*)&handles[1];
    const CCharString& none = *(const CCharString*)&handles[2];
    { CCombatAction_FallStunned a(&owner, &target, roll);
      CHECK(g_owner == &owner && g_target == &target && g_code == 250 && g_s1 == 0 && g_s2 == 0);
      CHECK(g_sets == 1 && g_setData == &dRoll && g_ctors == 0); }
    { CCombatAction_FallStunned a(&owner, 0, empty);
      CHECK(g_target == &owner);
      CHECK(g_sets == 2 && g_setData == &g_defaultBlock && strcmp(g_setText, "FALL_STUNNED_DEFAULT") == 0 && g_ctors == 1 && g_dtors == 1); }
    { CCombatAction_FallStunned a(&owner, &target, none);
      CHECK(g_sets == 3 && g_setData == &g_defaultBlock && g_ctors == 2 && g_dtors == 2); }
    if (fails == 0) printf("008eda90_TEST PASS\n"); else printf("008eda90_TEST FAIL\n");
    return fails;
}