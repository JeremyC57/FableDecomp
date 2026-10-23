// Behaviour test for CV_TempleOfLightScript::WatchForAcolyteHit (0x00EDCB80).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CScriptThing {
    int id; const int* hits; int* frame; bool alive;
    bool MsgIsHitBy(const char* w) const { return !std::strcmp(w, "SCRIPT_NAME_HERO") && hits[*frame] == 1; }
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const char* w) const { return !std::strcmp(w, "SCRIPT_NAME_HERO") && hits[*frame] == 2; }
    bool IsAlive() const { return alive; }
};
struct CGameScriptInterface {
    int frame; bool altarAlive; CScriptThing hero;
    void NewScriptFrame() { Log("frame"); ++frame; }
    CScriptThing& GetHero() { hero.id = 1; return hero; }
    CScriptThing GetThingWithScriptName(const char* n) { CScriptThing t; t.id = !std::strcmp(n, "M_SoundAltar") ? 9 : 0; t.alive = altarAlive; return t; }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) { Log(t.id == 9 && !a && !b ? "conv" : "badconv"); return 77; }
    void AddPersonToConversation(long c, const CScriptThing& t) { Log(c == 77 && t.id == 1 ? "person" : "badperson"); }
    void AddLineToConversation(long c, const char* s, bool b, const CScriptThing& sp, const CScriptThing& to)
    { Log(c == 77 && !std::strcmp(s, "TEXT_QST_064_WEAPON_02") && !b && sp.id == 9 && to.id == 1 ? "line" : "badline"); }
};
struct CV_TempleOfLightScript {
    CGameScriptInterface* Game; bool AttackedByHero, AcolyteAttackMessageComplete; int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void SetPredicate(const CScriptThing&) { Log("pred"); }
    void WatchForAcolyteHit(CScriptThing acolyte)
    {
        SetPredicate(acolyte);
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        CScriptThing altar = Game->GetThingWithScriptName("M_SoundAltar");
        AcolyteAttackMessageComplete = false;
        while (!IsActiveThreadTerminating()) {
            if (acolyte.MsgIsHitBy("SCRIPT_NAME_HERO") || acolyte.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) return;
                AttackedByHero = true;
                if (!AcolyteAttackMessageComplete && altar.IsAlive()) {
                    if (IsActiveThreadTerminating()) return;
                    long conversation = Game->AddNewConversation(altar, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, "TEXT_QST_064_WEAPON_02", false, altar, Game->GetHero());
                    AcolyteAttackMessageComplete = true;
                }
            }
            Game->NewScriptFrame();
        }
    }
};
static bool Run(bool altarAlive, const char* expect)
{
    static const int hits[] = { 0, 0, 2, 0, 1, 0, 0, 0, 0, 0 };
    g_log[0] = 0;
    CGameScriptInterface g; g.frame = 0; g.altarAlive = altarAlive;
    CScriptThing a; a.id = 5; a.hits = hits; a.frame = &g.frame; a.alive = true;
    CV_TempleOfLightScript s; s.Game = &g; s.AttackedByHero = false; s.AcolyteAttackMessageComplete = true; s.termAfter = 10;
    s.WatchForAcolyteHit(a);
    if (std::strcmp(g_log, expect) != 0) { std::printf("%s\n", g_log); return false; }
    return s.AttackedByHero && s.AcolyteAttackMessageComplete == altarAlive;
}
int main()
{
    bool ok = Run(true, "pred;frame;frame;conv;person;line;frame;frame;frame;frame;frame;")
        && Run(false, "pred;frame;frame;frame;frame;frame;frame;frame;frame;");
    if (!ok) { std::printf("00edcb80_TEST FAIL\n"); return 1; }
    std::printf("00edcb80_TEST PASS\n");
    return 0;
}
