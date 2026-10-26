// Behaviour test for CQ_GuildTrainingDepartureScript::Init (0x00D506B0): the hero is teleported to the named marker
// (no camera reset), then the screen fades in, then the four result fields are cleared.
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CCharString {
    const char* Text;
    CCharString(const char* text, long = -1) : Text(text) {}
};
struct CScriptThing {
    const char* Name;
    CScriptThing(const char* n) : Name(n) {}
};
static CScriptThing g_hero("HERO");
struct CGameScriptInterface {
    CScriptThing& GetHero() { Log("GetHero"); return g_hero; }
    CScriptThing GetThingWithScriptName(const CCharString& name) { Log(name.Text); return CScriptThing(name.Text); }
    void EntityTeleportToThing(const CScriptThing& who, const CScriptThing& where, bool keepCamera)
    {
        char b[128]; std::sprintf(b, "Teleport(%s,%s,%d)", who.Name, where.Name, keepCamera ? 1 : 0); Log(b);
    }
    void FadeScreenIn() { Log("FadeIn"); }
};
struct CQuestScriptBase { CGameScriptInterface* GSI; };
struct CQ_GuildTrainingDepartureScript : CQuestScriptBase {
    bool Finished; long MeleeGrade; long SkillGrade; long WillGrade;
    void Init();
};
void CQ_GuildTrainingDepartureScript::Init()
{
    GSI->EntityTeleportToThing(GSI->GetHero(), GSI->GetThingWithScriptName("HeroDepartureStartMarker"), false);
    GSI->FadeScreenIn();
    Finished = false;
    MeleeGrade = 0;
    SkillGrade = 0;
    WillGrade = 0;
}
int main()
{
    CGameScriptInterface gsi;
    CQ_GuildTrainingDepartureScript q;
    q.GSI = &gsi; q.Finished = true; q.MeleeGrade = 3; q.SkillGrade = 4; q.WillGrade = 5;
    q.Init();
    const char* want = "Teleport(HERO,HeroDepartureStartMarker,0);FadeIn;";
    const char* tail = std::strstr(g_log, "Teleport");
    if (!tail || std::strcmp(tail, want) != 0 || !std::strstr(g_log, "GetHero") || q.Finished || q.MeleeGrade || q.SkillGrade || q.WillGrade) {
        std::printf("00d506b0_TEST FAIL\n"); return 1;
    }
    std::printf("00d506b0_TEST PASS\n");
    return 0;
}