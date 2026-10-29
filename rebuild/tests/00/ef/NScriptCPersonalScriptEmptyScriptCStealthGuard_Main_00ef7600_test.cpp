// Behaviour test for 0x00EF7600 NScript::CPersonalScript_EmptyScript::CStealthGuard::Main: look up the GameRestart and
// ReachedGoalRestart things, set the predicate "Me, GameRestart and ReachedGoalRestart are all alive", advance one script
// frame and, unless the thread is terminating, read Me's position.
#include <stdio.h>
#include <string.h>

static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); strcat(g_log, ";"); }
static int g_terminating;
class CThing { public: const char* Name; bool Alive; void GetPos() const { Log("GetPos"); } };
static CThing g_restart = { "GameRestart", true };
static CThing g_goal = { "ReachedGoalRestart", true };
class CGame { public:
    CThing GetThingWithScriptName(const char* n) { Log(n); return strcmp(n, "GameRestart") == 0 ? g_restart : g_goal; }
    void NewScriptFrame() { Log("Frame"); }
};
struct CPredicate { const CThing* Things[3]; bool Test() const { return Things[0]->Alive && Things[1]->Alive && Things[2]->Alive; } };
struct CStealthGuard {
    CGame* Game;
    CThing Me;
    CPredicate Predicate;
    bool IsActiveThreadTerminating() { return g_terminating != 0; }
    void SetPredicate(const CPredicate& p) { Predicate = p; Log("SetPredicate"); }
    void Main()
    {
        CThing gameRestart = Game->GetThingWithScriptName("GameRestart");
        CThing goalRestart = Game->GetThingWithScriptName("ReachedGoalRestart");
        CPredicate p; p.Things[0] = &Me; p.Things[1] = &gameRestart; p.Things[2] = &goalRestart;
        SetPredicate(p);
        bool alive = Predicate.Test();
        if (!alive) Log("Dead");
        goalRestart.Alive = false;
        if (Predicate.Test()) Log("PredicateBad");
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Me.GetPos();
    }
};

int main()
{
    CGame g; CStealthGuard s; s.Game = &g; s.Me.Name = "Guard"; s.Me.Alive = true;
    g_log[0] = 0; g_terminating = 0;
    s.Main();
    int ok = !strcmp(g_log, "GameRestart;ReachedGoalRestart;SetPredicate;Frame;GetPos;");
    g_log[0] = 0; g_terminating = 1;
    s.Main();
    ok = ok && !strcmp(g_log, "GameRestart;ReachedGoalRestart;SetPredicate;Frame;");
    if (!ok) { printf("00ef7600_TEST FAIL\n"); return 1; }
    printf("00ef7600_TEST PASS\n");
    return 0;
}