// Behaviour test for CV_IntroductionToTrophiesScript::WatchForAttack (0x00E9DC80).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing {
    int frame; int dieAt; const int* hits; float health;
    bool IsAlive() const { return frame < dieAt; }
    bool MsgIsHitBy(const char* who) const { return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && hits[frame] == 1; }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const char* who) const { return std::strcmp(who, "SCRIPT_NAME_HERO") == 0 && hits[frame] == 2; }
};
struct CGameScriptInterface {
    CScriptThing* briar;
    void NewScriptFrame() { Log("frame"); ++briar->frame; }
    float GetHealth(const CScriptThing& t) { return t.health; }
    int GetHero() { return 1; }
    int GetNearestWithDefName(int hero, const char* def) { return std::strcmp(def, "VILLAGE_BOWERSTONE_SLUMS") == 0 ? 42 : 0; }
    void AddCrimeCommitted(int village, long crime, bool b, const void* p, const void* q, long deed)
    { Log(village == 42 && crime == 7 && !b && !p && !q && deed == 7 ? "crime" : "badcrime"); }
};
struct CV_IntroductionToTrophiesScript {
    CGameScriptInterface* Game;
    bool AttackedByHero; long HitByHeroCount; bool HitWarningComplete;
    int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void WatchForAttack(CScriptThing& briar)
    {
        for (;;) {
            if (IsActiveThreadTerminating()) return;
            if (!briar.IsAlive()) {
                if (IsActiveThreadTerminating()) return;
                if (IsActiveThreadTerminating()) return;
                break;
            }
            if (briar.MsgIsHitBy("SCRIPT_NAME_HERO") || briar.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                if (Game->GetHealth(briar) > 0.0001f) {
                    if (IsActiveThreadTerminating()) return;
                    ++HitByHeroCount;
                    HitWarningComplete = false;
                    Game->AddCrimeCommitted(Game->GetNearestWithDefName(Game->GetHero(), "VILLAGE_BOWERSTONE_SLUMS"), 7, false, 0, 0, 7);
                    if (HitByHeroCount >= 0 && !AttackedByHero) {
                        if (IsActiveThreadTerminating()) return;
                        AttackedByHero = true;
                    }
                }
            }
            Game->NewScriptFrame();
        }
    }
};
static bool Run(float health, long count, const char* expect, long expectCount, bool expectAttacked)
{
    static const int hits[] = { 0, 1, 0, 2, 0, 0 };
    g_log[0] = 0;
    CScriptThing briar; briar.frame = 0; briar.dieAt = 5; briar.hits = hits; briar.health = health;
    CGameScriptInterface game; game.briar = &briar;
    CV_IntroductionToTrophiesScript s; s.Game = &game; s.AttackedByHero = false; s.HitByHeroCount = count;
    s.HitWarningComplete = true; s.termAfter = 100;
    s.WatchForAttack(briar);
    return std::strcmp(g_log, expect) == 0 && s.HitByHeroCount == expectCount && s.AttackedByHero == expectAttacked
        && (expectCount == count || !s.HitWarningComplete);
}
int main()
{
    bool ok = Run(50.0f, 0, "frame;crime;frame;frame;crime;frame;frame;", 2, true)
        && Run(0.0f, 0, "frame;frame;frame;frame;frame;", 0, false)
        && Run(50.0f, -5, "frame;crime;frame;frame;crime;frame;frame;", -3, false);
    if (!ok) { std::printf("00e9dc80_TEST FAIL\n"); return 1; }
    std::printf("00e9dc80_TEST PASS\n");
    return 0;
}