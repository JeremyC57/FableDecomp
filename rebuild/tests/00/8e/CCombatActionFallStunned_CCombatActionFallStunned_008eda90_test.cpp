// Behaviour test for 0x008EDA90 CCombatAction_FallStunned::CCombatAction_FallStunned: builds the base action (code 250, target = thing
// or else the owner) and sets the combat animation to the given name, or to "RESPONSE_GENERIC_HIT_BACK_KNOCKDOWN" (retail 0x0127717C)
// when the name is empty.
// NF420 (2026-11-25): LINKED test -- it links the row object (the constructor is declared here, not defined); the callees are defined
// under the row's own spellings and record what they receive, so the test asserts the literal; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
#include <new>
#pragma pack(push, 1)
template <class T> class CBasicString { public: char* PData; unsigned long StringLength; unsigned long DataLength : 31; bool UseFastExtend : 1; bool operator==(const char* text) const; };
class CCharStringData { public: CBasicString<char> Data; long NoRefs; };
#pragma pack(pop)
class CCharString { public: CCharStringData* PStringData; CCharString(const char* text, long length); ~CCharString(); };
class CThing { public: };
class CThingCreatureBase : public CThing { public: };
class CSoundPair { public: long FirstSound; long LastSound; };
class CCombatActionBase { public:
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);
    void SetCombatAnimation(const CCharString&);
};
class CCombatAction_FallStunned : public CCombatActionBase { public:
    virtual void v00();
    CCombatAction_FallStunned(CThingCreatureBase* owner, CThing* thing, const CCharString& animName);
};
static char g_anim[64]; static CThing* g_target = 0; static long g_code = 0; static int g_bad = 0; static CCharStringData g_data[4]; static int g_n = 0;
template <> bool CBasicString<char>::operator==(const char* text) const { return strcmp(PData, text) == 0; }
CCharString::CCharString(const char* text, long length) { CCharStringData* d = &g_data[g_n++ & 3]; d->Data.PData = (char*)text; PStringData = d; if (length != -1) g_bad = 1; }
CCharString::~CCharString() { }
void CCombatActionBase::v00() { }
void CCombatAction_FallStunned::v00() { }
CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing* target, long actionCode, const CSoundPair& sound)
{ g_target = target; g_code = actionCode; if (sound.FirstSound || sound.LastSound) g_bad = 1; }
void CCombatActionBase::SetCombatAnimation(const CCharString& name) { strcpy(g_anim, name.PStringData->Data.PData); }
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
// (none)
int main()
{
    static double raw[0x118 / 8]; CThingCreatureBase owner; CThing other;
    CCharString empty("", -1); empty.PStringData = 0;                          // a null-data (empty) name
    new (raw) CCombatAction_FallStunned(&owner, 0, empty);
    bool ok = strcmp(g_anim, "RESPONSE_GENERIC_HIT_BACK_KNOCKDOWN") == 0 && g_target == &owner && g_code == 250;
    CCharString given("RESPONSE_CUSTOM", -1);
    new (raw) CCombatAction_FallStunned(&owner, &other, given);
    ok = ok && strcmp(g_anim, "RESPONSE_CUSTOM") == 0 && g_target == &other && !g_bad;
    if (ok) printf("008eda90_TEST PASS\n");
    else printf("FAIL anim=%s code=%ld bad=%d\n", g_anim, g_code, g_bad);
    return ok ? 0 : 1;
}
