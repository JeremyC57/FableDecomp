// Behaviour test for 0x00DDDE20 NScript::CQ_PrisonWardenGameScript::Main: four bindings, hero teleported and turned to the start marker, region wait, one spawned mini-game
#include <cstdio>
#include <cstring>
static int gFrames = 0, gBindings = 0, gSpawns = 0, gTeleports = 0;
static float gFacing = 0.0f;
class CThing { public: const char* Name; float Angle; float GetAngleXY() const { return Angle; } };
static CThing gHero = { "Hero", 0.0f }, gStart = { "WardensOfficeStart", 1.5f };
class CGame { public:
    bool IsRegionLoaded(const char* r) { return std::strcmp(r, "PrisonOffice") == 0 && gFrames >= 2; }
    void NewScriptFrame() { ++gFrames; }
    void PostAddScriptedEntities() {}
    CThing& GetHero() { return gHero; }
    CThing GetThingWithScriptName(const char* n) { return std::strcmp(n, gStart.Name) == 0 ? gStart : CThing(); }
    void EntityTeleportToThing(const CThing& a, const CThing& b, bool) { if (&a == &gHero && std::strcmp(b.Name, "WardensOfficeStart") == 0) ++gTeleports; }
    void EntitySetFacingAngle(const CThing& a, float angle, bool snap) { if (&a == &gHero && snap) gFacing = angle; }
};
struct CScript {
    CGame* Game; int TerminateAt;
    bool IsActiveThreadTerminating() { return gFrames >= TerminateAt; }
    void AddEntityScriptBinding(const char*, unsigned long flags) { if (flags == 0) ++gBindings; }
    void AddSpawnedFunction(const char* name) { if (std::strcmp(name, "WardenKeyMiniGame") == 0) ++gSpawns; }
    void PostAddScriptedEntities() {}
    void Main()
    {
        AddEntityScriptBinding("PrisonWarden", 0); AddEntityScriptBinding("SecretBook", 0);
        AddEntityScriptBinding("ExitWardensOffice", 0); AddEntityScriptBinding("DoNotTarget", 0);
        PostAddScriptedEntities(); Game->PostAddScriptedEntities();
        Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("WardensOfficeStart"), false);
        Game->EntitySetFacingAngle(Game->GetHero(), Game->GetThingWithScriptName("WardensOfficeStart").GetAngleXY(), true);
        while (!Game->IsRegionLoaded("PrisonOffice")) { Game->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
        if (IsActiveThreadTerminating()) return;
        AddSpawnedFunction("WardenKeyMiniGame");
    }
};
int main()
{
    CGame g; CScript s; s.Game = &g; s.TerminateAt = 100;
    s.Main();
    bool ok = gBindings == 4 && gTeleports == 1 && gFacing == 1.5f && gFrames == 2 && gSpawns == 1;
    gFrames = 0; gSpawns = 0; s.TerminateAt = 1; s.Main();          // terminated during the region wait: no spawn
    ok = ok && gSpawns == 0;
    std::printf(ok ? "00ddde20_TEST PASS\n" : "00ddde20_TEST FAIL\n");
    return ok ? 0 : 1;
}