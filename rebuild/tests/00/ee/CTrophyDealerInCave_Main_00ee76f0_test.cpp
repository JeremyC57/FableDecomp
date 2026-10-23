#include <cstdio>
#include <cstring>
#include <string>
#include <map>
// Behaviour test for CTrophyDealerInCave::Main (0x00EE76F0): after the dealer is made unkillable/undamageable and the hero
// is scripted, waits until the dealer can be scripted, plays CS_TROPHY_DEALER with Hero/Trophy cast inside a paused movie,
// then enables the Witchwood2 generators, completes the active quest, fades in and deactivates it.
struct CScriptThing { int Id; };
struct CHandle { int Thing; bool Scripting; CHandle() : Thing(0), Scripting(false) {} bool IsScripting() { return Scripting; } void Release() { Scripting = false; } };
struct CGSI {
    int Frames; int ReadyFrame; bool Killable; bool Damageable; bool Paused; int PauseCalls; char Log[256];
    CScriptThing Hero;
    void Add(const char* s) { std::strcat(Log, s); std::strcat(Log, ";"); }
    void NewScriptFrame() { ++Frames; }
    bool StartScriptingEntity(const CScriptThing& t, CHandle& h, int) { if (t.Id == 2 && Frames < ReadyFrame) return false; h.Thing = t.Id; h.Scripting = true; return true; }
    CScriptThing& GetHero() { return Hero; }
    void EntitySetAsKillable(const CScriptThing&, bool k, bool) { Killable = k; }
    void EntitySetAsDamageable(const CScriptThing&, bool d) { Damageable = d; }
    void StartMovieSequence(const char* name, CHandle&) { if (!*name) Add("movie"); }
    void PauseAllNonScriptedEntities(bool p) { Paused = p; ++PauseCalls; }
    void SetCreatureGeneratorsEnabled(const char* n, bool e) { if (e) Add(n); }
    std::string GetActiveQuestName() { return "Q_Trophy"; }
    void SetQuestAsCompleted(const std::string& q, bool, bool, bool) { Add(("done:" + q).c_str()); }
    void FadeScreenIn() { Add("fade"); }
    void DeactivateQuestLater(const std::string& q, unsigned long) { Add(("off:" + q).c_str()); }
};
static CGSI* g_GSI;
static void RunCutsceneMacro(const char* name, std::map<std::string, CHandle>& cast, void*, void*, bool, bool)
{
    if (g_GSI->Paused && cast["Hero"].Thing == 1 && cast["Trophy"].Thing == 2) g_GSI->Add(name);
}
struct CTrophyDealerInCave {
    CGSI* GSI; CScriptThing Me; int Predicates;
    bool IsActiveThreadTerminating() { return GSI->Frames > 20; }
    void Main()
    {
        CHandle trophy;
        ++Predicates;
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        GSI->EntitySetAsKillable(Me, false, true);
        GSI->EntitySetAsDamageable(Me, false);
        {
            CHandle hero;
            GSI->StartScriptingEntity(GSI->GetHero(), hero, 4);
            if (trophy.IsScripting()) trophy.Release();
            while (!GSI->StartScriptingEntity(Me, trophy, 4)) { GSI->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
            if (IsActiveThreadTerminating()) return;
            std::map<std::string, CHandle> cast;
            cast["Hero"] = hero;
            cast["Trophy"] = trophy;
            {
                CHandle movie;
                GSI->StartMovieSequence("", movie);
                CGSI* game = GSI;
                game->PauseAllNonScriptedEntities(true);
                RunCutsceneMacro("CS_TROPHY_DEALER", cast, 0, 0, false, true);
                game->PauseAllNonScriptedEntities(false);
            }
        }
        GSI->SetCreatureGeneratorsEnabled("Witchwood2", true);
        GSI->SetQuestAsCompleted(GSI->GetActiveQuestName(), true, false, false);
        GSI->FadeScreenIn();
        GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
    }
};
int main()
{
    CGSI gsi; std::memset(&gsi, 0, sizeof gsi); gsi.ReadyFrame = 4; gsi.Killable = gsi.Damageable = true; gsi.Hero.Id = 1;
    g_GSI = &gsi;
    CTrophyDealerInCave s; s.GSI = &gsi; s.Me.Id = 2; s.Predicates = 0;
    s.Main();
    bool ok = gsi.Frames == 4 && !gsi.Killable && !gsi.Damageable && !gsi.Paused && gsi.PauseCalls == 2 && s.Predicates == 1
              && !std::strcmp(gsi.Log, "movie;CS_TROPHY_DEALER;Witchwood2;done:Q_Trophy;fade;off:Q_Trophy;");
    if (!ok) { std::printf("00ee76f0_TEST FAIL %d %s\n", gsi.Frames, gsi.Log); return 1; }
    std::printf("00ee76f0_TEST PASS\n");
    return 0;
}