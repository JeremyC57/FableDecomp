// Behaviour test for NScript::CV_FishermanScript::CFisherman::Init 0x00E7BD60
#include <cstdio>
#include <string>
static std::string g_log;
static void Log(const std::string& s) { g_log += s; g_log += ';'; }
class CScriptThing { public: std::string Name; float Angle; float GetAngleXY() const { return Angle; } };
class CGame { public:
    void EntitySetAsKillable(const CScriptThing& t, bool killable, bool) { Log(t.Name + (killable ? ":killable" : ":unkillable")); }
    CScriptThing GetThingWithScriptName(const char* name) { CScriptThing t; t.Name = name; t.Angle = 1.5f; return t; }
    void EntityTeleportToThing(const CScriptThing& t, const CScriptThing& to, bool) { Log(t.Name + ":teleport:" + to.Name); }
    void EntitySetFacingAngle(const CScriptThing& t, float a, bool) { Log(t.Name + (a == 1.5f ? ":face" : ":badface")); }
    void CloseDoor(const CScriptThing& t) { Log("close:" + t.Name); }
};
struct CV_FishermanScript {
    bool HeroHasSavedFishermanFromWasps;
    bool FishermanMustRunAway;
    std::string SpawnedName;
    CScriptThing SpawnedArg;
    void SpawnFunction(const std::string& name, const CScriptThing& arg, const std::string& region) { SpawnedName = name; SpawnedArg = arg; Log("spawn:" + name + "@" + region); }
};
struct CFisherman {
    CGame* Game;
    CScriptThing Me;
    CV_FishermanScript* ParentClass;
    void Init()
    {
        ParentClass->FishermanMustRunAway = false;
        Game->EntitySetAsKillable(Me, false, true);
        if (!ParentClass->HeroHasSavedFishermanFromWasps) {
            Game->EntityTeleportToThing(Me, Game->GetThingWithScriptName("MK_FI_FISHER"), false);
            Game->EntitySetFacingAngle(Me, Game->GetThingWithScriptName("MK_FI_FISHER").GetAngleXY(), true);
            Game->CloseDoor(Game->GetThingWithScriptName("FishermanHutDoor"));
        }
        ParentClass->SpawnFunction(std::string("ParentClass.") + std::string("NPCAttitudeProcess"), Me, "");
    }
};
int main()
{
    CGame g; CV_FishermanScript p; CFisherman f;
    p.HeroHasSavedFishermanFromWasps = false; p.FishermanMustRunAway = true;
    f.Game = &g; f.Me.Name = "Fisher"; f.Me.Angle = 0.0f; f.ParentClass = &p;
    f.Init();
    if (p.FishermanMustRunAway || g_log != "Fisher:unkillable;Fisher:teleport:MK_FI_FISHER;Fisher:face;close:FishermanHutDoor;spawn:ParentClass.NPCAttitudeProcess@;"
        || p.SpawnedArg.Name != "Fisher") { std::printf("00e7bd60_TEST FAIL\n"); return 1; }
    g_log = ""; p.HeroHasSavedFishermanFromWasps = true;
    f.Init();
    if (g_log != "Fisher:unkillable;spawn:ParentClass.NPCAttitudeProcess@;") { std::printf("00e7bd60_TEST FAIL\n"); return 1; }
    std::printf("00e7bd60_TEST PASS\n");
    return 0;
}