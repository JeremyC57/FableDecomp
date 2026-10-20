// Behaviour test for 008bb130: the model below is a copy of src/008bb130.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
struct CThingCreatureBase { int id; };
struct CThing { int id; };
static int g_baseCalls = 0, g_baseCode = 0; static const void* g_baseOwner = 0; static const void* g_baseTarget = 0; static long g_baseSound[2] = {9, 9};
static int g_setNameCalls = 0; static char g_setName[64];
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    bool operator==(const char* text) const { return strcmp(PData, text) == 0; }
};
struct CCharStringData {
    CBasicStringChar Data;                      // +0x00
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;               // +0x00
    CCharString(const char* text, long length = -1) { PStringData = new CCharStringData; PStringData->Data.PData = (char*)text; PStringData->Data.StringLength = (unsigned long)strlen(text); }
    ~CCharString() { delete PStringData; }
    bool operator==(const char* text) const
    {
        if (PStringData == 0)
            return strcmp(text, "") == 0;
        return PStringData->Data == text;
    }
    bool operator!=(const char* text) const { return !(*this == text); }
    const char* Text() const { return PStringData ? PStringData->Data.PData : ""; }
};

struct CSoundPair {                             // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00() {}
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound) { ++g_baseCalls; g_baseOwner = owner; g_baseTarget = 0; g_baseCode = actionCode; g_baseSound[0] = sound.FirstSound; g_baseSound[1] = sound.LastSound; }
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound) { ++g_baseCalls; g_baseOwner = owner; g_baseTarget = target; g_baseCode = actionCode; g_baseSound[0] = sound.FirstSound; g_baseSound[1] = sound.LastSound; }
    void SetAnimationName(const CCharString& name) { ++g_setNameCalls; strncpy(g_setName, name.Text(), 63); g_setName[63] = 0; }
};

struct CCreatureAction_CastWillForceSpell : CCombatActionBase {   // vtable 0x0127B50C
    bool ImmediateCast;                         // +0x114 (PDB +0x12c)
    bool OneAnimCast;                           // +0x115 (PDB +0x12d)
    unsigned char _pad116[2];
    long LevelToCast;                           // +0x118 (PDB +0x130)
    virtual void v00() {}
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

// ---- test doubles and driver ----
static bool Run(const CCharString& name, bool immediate, bool expectOne, const char* expectName)
{
    CThingCreatureBase owner; CThing target; g_baseCalls = g_setNameCalls = 0; g_setName[0] = 0;
    CCreatureAction_CastWillForceSpell a(&owner, &target, 7, immediate, name);
    if (g_baseCalls != 1 || g_baseOwner != &owner || g_baseTarget != &target || g_baseCode != 150 || g_baseSound[0] != 0 || g_baseSound[1] != 0) return false;
    if (a.ImmediateCast != immediate || a.LevelToCast != 7 || a.OneAnimCast != expectOne) return false;
    if (g_setNameCalls != 1 || strcmp(g_setName, expectName) != 0) return false;
    return true;
}
int main()
{
    bool ok = true;
    { CCharString n("CAST_ANIM"); ok = ok && Run(n, true, true, "CAST_ANIM"); }
    { CCharString n(""); ok = ok && Run(n, false, false, "WILL_CAST_FORCE_SPELL_INTO"); }
    { CCharString n(""); delete n.PStringData; n.PStringData = 0; ok = ok && Run(n, true, false, "WILL_CAST_FORCE_SPELL_INTO"); }
    if (!ok) { std::printf("008bb130_TEST FAIL\n"); return 1; }
    std::printf("008bb130_TEST PASS\n");
    return 0;
}