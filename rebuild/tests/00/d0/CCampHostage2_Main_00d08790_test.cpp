// 00d08790 behaviour test: CCampHostage2::Main (final.cpp body over a scripted mock game interface)
#include <stdio.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct CScriptThing { int Id; };
struct CQ_BanditCampBossBattleScript { bool HostagesRescued, HostageKilled; };
struct CScripted { bool Scripting; int Releases; std::vector<std::string> Anims;
    bool IsScripting() { return Scripting; } void Release() { ++Releases; Scripting = false; }
    void PlayAnimation(const char* a) { Anims.push_back(a); } };
struct CGame {
    int Frame; int StartAfter; long Timers[4]; int NextTimer; float Distance; int RescueAtFrame;
    CQ_BanditCampBossBattleScript* Quest; std::vector<std::string> Lines; int Faces; CScriptThing Hero;
    void NewScriptFrame() { ++Frame; for (int i = 0; i < NextTimer; ++i) --Timers[i]; if (Frame == RescueAtFrame) Quest->HostagesRescued = true; }
    bool StartScriptingEntity(const CScriptThing&, CScripted& h, int) { if (Frame < StartAfter) return false; h.Scripting = true; return true; }
    long RegisterTimer() { Timers[NextTimer] = 0; return NextTimer++; }
    void SetTimer(long t, long v) { Timers[t] = v * 2; }   // two frames per second
    long GetTimer(long t) const { return Timers[t]; }
    CScriptThing& GetHero() { return Hero; }
    void EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) { ++Faces; }
    long AddNewConversation(const CScriptThing&, bool, bool) { return 1; }
    void AddPersonToConversation(long, const CScriptThing&) { }
    void AddLineToConversation(long, const char* text, bool, const CScriptThing&, const CScriptThing&) { Lines.push_back(text); }
};
static CGame* g_game;
struct CTimer {
    long Index;
    CTimer() : Index(g_game->RegisterTimer()) { }
    long GetTime() const { return g_game->GetTimer(Index); }
    void SetTime(long t) { g_game->SetTimer(Index, t); }
};
struct CCampHostage2 {
    CGame* Game; CScriptThing Me; CQ_BanditCampBossBattleScript* ParentClass; int TerminateAtFrame; CScripted* Handle;
    bool IsActiveThreadTerminating() const { return TerminateAtFrame && Game->Frame >= TerminateAtFrame; }
    void Main();
};
static bool IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float d) { return g_game->Distance < d; }
void CCampHostage2::Main()
{
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScripted& scripted = *Handle;
    if (scripted.IsScripting())
        scripted.Release();
    while (!Game->StartScriptingEntity(Me, scripted, 4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (!IsActiveThreadTerminating()) {
        CTimer cryTimer;
        cryTimer.SetTime(0);
        long cry = 0;
        while (!ParentClass->HostagesRescued && !ParentClass->HostageKilled) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            if (cryTimer.GetTime() <= 0 && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 8.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                Game->EntitySetFacingAngleTowardsThing(Me, Game->GetHero(), false);
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                scripted.PlayAnimation("CS_DISAPPOINTED");
                switch (cry) {
                case 0:
                    Game->AddLineToConversation(conversation, "TEXT_QST_009_HOSTAGE_SECOND_CRY_FIRST", false, Me, Game->GetHero());
                    cry = 1;
                    break;
                case 1:
                    Game->AddLineToConversation(conversation, "TEXT_QST_009_HOSTAGE_SECOND_CRY_SECOND", false, Me, Game->GetHero());
                    cry = 0;
                    break;
                }
                cryTimer.SetTime(8);
            }
        }
        if (!IsActiveThreadTerminating())
            scripted.Release();
    }
}
int main()
{
    CQ_BanditCampBossBattleScript q = { false, false };
    CGame g; g.Frame = 0; g.StartAfter = 3; g.NextTimer = 0; g.Distance = 5.0f; g.RescueAtFrame = 40; g.Quest = &q; g.Faces = 0; g.Hero.Id = 1;
    g_game = &g;
    CScripted h = { true, 0 };
    CCampHostage2 s; s.Game = &g; s.Me.Id = 2; s.ParentClass = &q; s.TerminateAtFrame = 0; s.Handle = &h;
    s.Main();
    // starts at frame 3; cries at frames 4, 20, 36 (every 16 frames), alternating lines; released at the end
    CHECK(g.Lines.size() == 3 && g.Faces == 3 && h.Anims.size() == 3);
    CHECK(g.Lines[0] == "TEXT_QST_009_HOSTAGE_SECOND_CRY_FIRST" && g.Lines[1] == "TEXT_QST_009_HOSTAGE_SECOND_CRY_SECOND" && g.Lines[2] == "TEXT_QST_009_HOSTAGE_SECOND_CRY_FIRST");
    CHECK(h.Releases == 2 && g.Frame == 40);
    // far away: no cries; terminating thread: no final release
    CQ_BanditCampBossBattleScript q2 = { false, false };
    CGame g2 = g; g2.Frame = 0; g2.Lines.clear(); g2.Faces = 0; g2.NextTimer = 0; g2.Distance = 20.0f; g2.Quest = &q2; g2.RescueAtFrame = 100;
    g_game = &g2; CScripted h2 = { false, 0 }; s.Game = &g2; s.ParentClass = &q2; s.Handle = &h2; s.TerminateAtFrame = 10;
    s.Main();
    CHECK(g2.Lines.empty() && h2.Releases == 0 && g2.Frame == 10);
    if (!g_fail) printf("00d08790_TEST PASS\n");
    return g_fail;
}