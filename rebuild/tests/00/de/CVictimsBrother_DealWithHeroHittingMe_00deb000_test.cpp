// Behaviour test for CVictimsBrother::DealWithHeroHittingMe (0x00DEB000).
#include <cstdio>
#include <cstring>
static char g_log[1024];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
class CScriptThing { public: int id; };
struct Handle {
    int busy; CScriptThing me;
    CScriptThing GetScriptThing() const { return me; }
    void Speak(const CScriptThing& to, const char* t, int m, bool a, bool b, bool c)
    { Log(to.id == 1 && !std::strcmp(t, "TEXT_QST_B06_BROTHER_ON_HIT") && m == 2 && !a && b && !c ? "speak" : "badspeak"); }
    bool IsPerformingScriptTask() { return busy-- > 0; }
};
struct Parent { long Hits; Handle seh_Brother; };
class CGameScriptInterface { public:
    float health; CScriptThing hero;
    void NewScriptFrame() { Log("frame"); }
    CScriptThing& GetHero() { hero.id = 1; return hero; }
    float GetHealth(const CScriptThing&) { return health; }
    long AddNewConversation(const CScriptThing& t, bool a, bool b) { Log(t.id == 5 && !a && !b ? "conv" : "badconv"); return 3; }
    void AddPersonToConversation(long c, const CScriptThing& t) { Log(c == 3 && t.id == 1 ? "person" : "badperson"); }
    void AddLineToConversation(long c, const char* s, bool b, const CScriptThing& sp, const CScriptThing& to)
    { Log(c == 3 && !std::strcmp(s, "TEXT_QST_B06_BROTHER_ON_RUN_AWAY_10") && !b && sp.id == 5 && to.id == 1 ? "line" : "badline"); }
    void StartMovieSequence(const char* n) { Log(n[0] ? "badmovie" : "movie"); }
    void PauseAllNonScriptedEntities(bool p) { Log(p ? "pause" : "unpause"); }
    void EntitySetThingAsAllyOfThing(const CScriptThing& a, const CScriptThing& b) { Log(a.id == 5 && b.id == 1 ? "ally1" : a.id == 1 && b.id == 5 ? "ally2" : "badally"); }
};
struct CVictimsBrother {
    CGameScriptInterface* Game; CScriptThing Me; Parent* ParentClass; int termAfter;
    bool IsActiveThreadTerminating() { return --termAfter < 0; }
    void DealWithHeroHittingMe()
    {
        ++ParentClass->Hits;
        if (ParentClass->Hits > 1) {
            if (IsActiveThreadTerminating()) return;
            long conversation = Game->AddNewConversation(Me, false, false);
            Game->AddPersonToConversation(conversation, Game->GetHero());
            Game->AddLineToConversation(conversation, "TEXT_QST_B06_BROTHER_ON_RUN_AWAY_10", false, Me, Game->GetHero());
        } else if (Game->GetHealth(Me) > 0.0f) {
            if (IsActiveThreadTerminating()) return;
            Game->StartMovieSequence("");
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            if (Game->GetHealth(ParentClass->seh_Brother.GetScriptThing()) > 0.0f) {
                ParentClass->seh_Brother.Speak(Game->GetHero(), "TEXT_QST_B06_BROTHER_ON_HIT", 2, false, true, false);
                while (ParentClass->seh_Brother.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
                }
                if (IsActiveThreadTerminating()) { game->PauseAllNonScriptedEntities(false); return; }
            }
            Game->EntitySetThingAsAllyOfThing(Me, Game->GetHero());
            Game->EntitySetThingAsAllyOfThing(Game->GetHero(), Me);
            game->PauseAllNonScriptedEntities(false);
        }
    }
};
static bool Run(long hits, float health, int busy, int term, const char* expect, long expectHits)
{
    g_log[0] = 0;
    CGameScriptInterface g; g.health = health;
    Parent p; p.Hits = hits; p.seh_Brother.busy = busy; p.seh_Brother.me.id = 5;
    CVictimsBrother b; b.Game = &g; b.Me.id = 5; b.ParentClass = &p; b.termAfter = term;
    b.DealWithHeroHittingMe();
    if (std::strcmp(g_log, expect) != 0) { std::printf("%s\n", g_log); return false; }
    return p.Hits == expectHits;
}
int main()
{
    bool ok = Run(1, 10.0f, 0, 9, "conv;person;line;", 2)
        && Run(1, 10.0f, 0, 0, "", 2)
        && Run(0, 10.0f, 2, 9, "movie;pause;speak;frame;frame;ally1;ally2;unpause;", 1)
        && Run(0, 10.0f, 5, 2, "movie;pause;speak;frame;frame;unpause;", 1)
        && Run(0, 0.0f, 0, 9, "", 1);
    if (!ok) { std::printf("00deb000_TEST FAIL\n"); return 1; }
    std::printf("00deb000_TEST PASS\n");
    return 0;
}