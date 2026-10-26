// Behaviour test for 008bb6f0: the model below is a copy of src/008bb6f0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CThingCreatureBase { public: int id; };
class CThing { public: int id; };
static int g_baseCalls = 0, g_baseCode = 0; static const void* g_baseOwner = 0; static const void* g_baseTarget = 0; static long g_baseSound[2] = {9, 9};
static int g_setNameCalls = 0; static char g_setName[64];
#pragma pack(push, 1)
struct CBasicStringChar {                       // CBasicString<char>
    char*         PData;                        // +0x00
    unsigned long StringLength;                 // +0x04
    bool operator==(const char* text) const { return strcmp(PData, text) == 0; }
};
class CCharStringData { public:
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

class CSoundPair { public:                             // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCombatActionBase { public:                      // retail sizeof 0x114
    virtual void v00() {}
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound) { ++g_baseCalls; g_baseOwner = owner; g_baseTarget = 0; g_baseCode = actionCode; g_baseSound[0] = sound.FirstSound; g_baseSound[1] = sound.LastSound; }
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound) { ++g_baseCalls; g_baseOwner = owner; g_baseTarget = target; g_baseCode = actionCode; g_baseSound[0] = sound.FirstSound; g_baseSound[1] = sound.LastSound; }
    void SetAnimationName(const CCharString& name) { ++g_setNameCalls; strncpy(g_setName, name.Text(), 63); g_setName[63] = 0; }
};

class CCreatureAction_CastPhysicalShieldSpell : public CCombatActionBase { public:   // vtable 0x0127BDCC
    bool  ImmediateCast;                        // +0x114 (PDB +0x12c)
    bool  OneAnimCast;                          // +0x115 (PDB +0x12d)
    unsigned char _pad116[2];
    long  LevelToCast;                          // +0x118 (PDB +0x130)
    long  TimeLimit;                            // +0x11c (PDB +0x134)
    float ShieldHealth;                         // +0x120 (PDB +0x138)
    virtual void v00() {}
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
    SetAnimationName(animationName);
    return;
empty:
    OneAnimCast = false;
    SetAnimationName("ST_RELEASE_PHYSICAL_SHIELD");
}

// ---- test doubles and driver ----
static bool Run(const CCharString& name, bool immediate, bool expectOne, const char* expectName)
{
    CThingCreatureBase owner; g_baseCalls = g_setNameCalls = 0; g_setName[0] = 0;
    CCreatureAction_CastPhysicalShieldSpell a(&owner, 4, immediate, name, 600, 25.5f);
    if (g_baseCalls != 1 || g_baseOwner != &owner || g_baseTarget != 0 || g_baseCode != 150 || g_baseSound[0] != 0 || g_baseSound[1] != 0) return false;
    if (a.ImmediateCast != immediate || a.LevelToCast != 4 || a.TimeLimit != 600 || a.ShieldHealth != 25.5f || a.OneAnimCast != expectOne) return false;
    if (g_setNameCalls != 1 || strcmp(g_setName, expectName) != 0) return false;
    return true;
}
int main()
{
    bool ok = true;
    { CCharString n("SHIELD_ANIM"); ok = ok && Run(n, true, true, "SHIELD_ANIM"); }
    { CCharString n(""); ok = ok && Run(n, false, false, "ST_RELEASE_PHYSICAL_SHIELD"); }
    { CCharString n(""); delete n.PStringData; n.PStringData = 0; ok = ok && Run(n, true, false, "ST_RELEASE_PHYSICAL_SHIELD"); }
    if (!ok) { std::printf("008bb6f0_TEST FAIL\n"); return 1; }
    std::printf("008bb6f0_TEST PASS\n");
    return 0;
}