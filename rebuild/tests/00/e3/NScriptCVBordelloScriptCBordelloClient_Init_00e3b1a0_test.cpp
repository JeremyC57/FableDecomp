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
static int g_rand = 13;
static int myrand() { return g_rand; }
struct Thing { int Id; };
struct Parent { int ClientsAlive; bool ClientInUse[3]; };
struct Game {
    void SetThingPersistent(const Thing&, bool p) { LogI("persist", p); }
    void EntitySetOpinionReactionMask(const Thing&, const char* m) { LogS("mask", m); }
};
struct Client { Game* G; Thing Me; Parent* ParentClass; int BrainState, ClientID, SexChance, NextLine;
    void Init()
    {
        ++ParentClass->ClientsAlive;
        BrainState = 1;
        SexChance = myrand() % 5;
        NextLine = 1;
        ClientID = 0;
        for (int i = 0; i < 3; ++i) {
            if (!ParentClass->ClientInUse[i]) {
                ClientID = i;
                ParentClass->ClientInUse[i] = true;
                break;
            }
        }
        G->SetThingPersistent(Me, true);
        G->EntitySetOpinionReactionMask(Me, "OPINION_REACTION_MASK_DONT_SCREAM");
    }
};
int main()
{
    bool ok = true;
    Game g; Parent p; p.ClientsAlive = 1; p.ClientInUse[0] = true; p.ClientInUse[1] = false; p.ClientInUse[2] = false;
    Client c; c.G = &g; c.ParentClass = &p; c.ClientID = 9;
    Reset(1000); c.Init();
    ok = p.ClientsAlive == 2 && c.ClientID == 1 && p.ClientInUse[1] && !p.ClientInUse[2] && c.SexChance == 3 && c.BrainState == 1 && c.NextLine == 1;
    ok = ok && std::strcmp(g_log, "persist1;mask OPINION_REACTION_MASK_DONT_SCREAM;") == 0;
    p.ClientInUse[2] = true; c.ClientID = 9; c.Init();
    ok = ok && c.ClientID == 0 && p.ClientsAlive == 3;
    if (!ok) { std::printf("log: %s\n", g_log); std::printf("00e3b1a0_TEST FAIL\n"); return 1; }
    std::printf("00e3b1a0_TEST PASS\n");
    return 0;
}