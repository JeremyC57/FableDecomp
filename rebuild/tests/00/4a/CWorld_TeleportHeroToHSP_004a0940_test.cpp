// Behaviour test for 0x004A0940 CWorld::TeleportHeroToHSP(const CCharString& name): the holy sites (the thing list's
// entries with the +0xD4 flag) are copied into a vector, the first whose ScriptName equals name is found, and the hero
// (main player's character) is teleported to it with EntityTeleportToThing(hero, site, false); no match -> false and no
// teleport. The two script-thing temporaries are released (hero's first) before returning.
#include <cstdio>
#include <cstring>

class CThing { public:
    const char* ScriptName;
    bool IsHolySite;                                   // the +0xD4 flag the copy template tests
    int Id;
};
static int g_released = 0, g_releaseOrder[4], g_teleports = 0, g_lastHero = -1, g_lastSite = -1;
static bool g_lastFlag = true;
class CScriptThing { public:
    int Id;
    CScriptThing(int id) : Id(id) { }
    CScriptThing(const CScriptThing& o) : Id(o.Id) { }
    ~CScriptThing() { if (g_released < 4) g_releaseOrder[g_released] = Id; ++g_released; }
};
static CScriptThing GetScriptThing(const CThing* t) { return CScriptThing(t->Id); }
class CGameScriptInterface { public:
    void EntityTeleportToThing(const CScriptThing& thing, const CScriptThing& target, bool a) const
    {
        ++g_teleports; g_lastHero = thing.Id; g_lastSite = target.Id; g_lastFlag = a;
    }
};
static CThing g_hero = { "HERO", false, 99 };
static CThing* GetPlayerCharacter() { return &g_hero; }

static CThing g_things[5] = {
    { "HSP_A", true, 1 }, { "VILLAGER", false, 2 }, { "HSP_B", true, 3 }, { "HSP_B", true, 4 }, { "HSP_C", false, 5 }
};
static CGameScriptInterface g_gsi;

static bool IsThingScriptNamed(CThing* thing, const char* name) { return std::strcmp(thing->ScriptName, name) == 0; }

static bool TeleportHeroToHSP(const char* name)
{
    CThing* sites[5];
    int count = 0;
    for (int i = 0; i < 5; ++i)
        if (g_things[i].IsHolySite)
            sites[count++] = &g_things[i];
    CThing** found = sites + count;
    for (CThing** it = sites; it != sites + count; ++it)
        if (IsThingScriptNamed(*it, name)) { found = it; break; }
    if (found != sites + count) {
        CGameScriptInterface* gsi = &g_gsi;
        gsi->EntityTeleportToThing(GetScriptThing(GetPlayerCharacter()), GetScriptThing(*found), false);
        return true;
    }
    return false;
}

int main()
{
    int ok = 1;
    if (!TeleportHeroToHSP("HSP_B") || g_teleports != 1 || g_lastHero != 99 || g_lastSite != 3 || g_lastFlag) ok = 0;
    if (g_released != 2) ok = 0;
    if (TeleportHeroToHSP("HSP_C") || TeleportHeroToHSP("VILLAGER") || g_teleports != 1) ok = 0;   // not holy sites
    if (!TeleportHeroToHSP("HSP_A") || g_lastSite != 1 || g_released != 4) ok = 0;
    if (!ok) { std::printf("004a0940_TEST FAIL\n"); return 1; }
    std::printf("004a0940_TEST PASS\n");
    return 0;
}