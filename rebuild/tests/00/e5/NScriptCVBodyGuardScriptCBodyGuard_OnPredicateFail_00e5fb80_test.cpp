#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (interface calls are logged, not executed).
static char g_log[4096];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static void LogI(const char* s, int v) { char b[128]; std::sprintf(b, "%s%d", s, v); Log(b); }
static void LogS(const char* s, const char* v) { char b[256]; std::sprintf(b, "%s %s", s, v); Log(b); }
static int g_termAfter = 1000;   // IsActiveThreadTerminating() turns true after this many calls
static int g_termCalls = 0;
static bool Term() { return ++g_termCalls > g_termAfter; }
static void Reset(int termAfter) { g_log[0] = 0; g_termCalls = 0; g_termAfter = termAfter; }
struct Thing { int Id; bool killed;
    bool operator==(const Thing& o) const { return Id == o.Id; }
    bool MsgIsKilledBy(const char* who) const { return who[0] == 0 && killed; } };
struct Vec { Thing items[4]; int n;
    Thing* begin() { return items; } Thing* end() { return items + n; }
    Thing* erase(Thing* p) { for (Thing* q = p; q + 1 < end(); ++q) *q = q[1]; --n; return p; } };
struct Game { void RemoveQuestInfoElement(int id) { LogI("removeinfo", id); } void SetTimer(int t, int v) { LogI("timer", t * 1000 + v); } };
static Game g_game;
class CTimer { public: int TimerIndex; CTimer& operator=(int t) { g_game.SetTimer(TimerIndex, t); return *this; } };
struct Parent { int TotalWages; Vec HenchmanList; CTimer HenchmanDeadRespawnTimer[4]; int HenchmanGlobalStatus[4]; };
struct MasterData { bool HenchmanSacrificedBodge; };
struct BodyGuard { Game* G; Thing Me; Parent* ParentClass; MasterData* PMasterData; int HenchManType, BrainState, MyHealthBarID;
    int GetGoldCost() { return HenchManType == 1 ? 500 : 100; }
    void OnPredicateFail()
    {
        for (Thing* it = ParentClass->HenchmanList.begin(); it != ParentClass->HenchmanList.end(); ++it) {
            if (Me == *it) {
                ParentClass->HenchmanList.erase(it);
                break;
            }
        }
        G->RemoveQuestInfoElement(MyHealthBarID);
        if (BrainState == 2)
            ParentClass->TotalWages -= GetGoldCost();
        BrainState = 1;
        if (Me.MsgIsKilledBy("") || PMasterData->HenchmanSacrificedBodge) {
            ParentClass->HenchmanGlobalStatus[HenchManType] = 1;
            ParentClass->HenchmanDeadRespawnTimer[HenchManType] = 240;
        }
    }
};
int main()
{
    bool ok = true;
    Parent p; p.TotalWages = 1000; p.HenchmanList.n = 3;
    for (int i = 0; i < 4; ++i) { p.HenchmanList.items[i].Id = i + 10; p.HenchmanList.items[i].killed = false; p.HenchmanDeadRespawnTimer[i].TimerIndex = i; p.HenchmanGlobalStatus[i] = 0; }
    MasterData md; md.HenchmanSacrificedBodge = false;
    BodyGuard b; b.G = &g_game; b.ParentClass = &p; b.PMasterData = &md; b.Me.Id = 11; b.Me.killed = true;
    b.HenchManType = 1; b.BrainState = 2; b.MyHealthBarID = 7;
    Reset(1000); b.OnPredicateFail();
    ok = p.HenchmanList.n == 2 && p.HenchmanList.items[1].Id == 12 && p.TotalWages == 500 && b.BrainState == 1 && p.HenchmanGlobalStatus[1] == 1;
    ok = ok && std::strcmp(g_log, "removeinfo7;timer1240;") == 0;
    b.Me.killed = false; b.Me.Id = 99; b.BrainState = 0; b.HenchManType = 2;
    Reset(1000); b.OnPredicateFail();
    ok = ok && p.HenchmanList.n == 2 && p.TotalWages == 500 && p.HenchmanGlobalStatus[2] == 0 && std::strcmp(g_log, "removeinfo7;") == 0;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e5fb80_TEST FAIL\n"); return 1; }
    std::printf("00e5fb80_TEST PASS\n");
    return 0;
}