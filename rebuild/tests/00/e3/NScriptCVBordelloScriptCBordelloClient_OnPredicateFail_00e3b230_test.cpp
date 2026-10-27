#include <cstdio>
#include <cstring>
// Behaviour test: a copy of the source's logic over a mock model (calls are recorded, not executed).
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static const char* g_str = "";
struct Parent { int ClientsAlive; bool ClientInUse[3]; };
struct Client {
    Parent* ParentClass; int BrainState; int ClientID;
    void OnPredicateFail()
    {
        --ParentClass->ClientsAlive;
        ParentClass->ClientInUse[ClientID] = false;
    }
};
int main()
{
    g_log[0] = 0;
    bool ok = true;
    Parent p; p.ClientsAlive = 3; p.ClientInUse[0] = p.ClientInUse[1] = p.ClientInUse[2] = true;
    Client c; c.ParentClass = &p; c.ClientID = 1;
    c.OnPredicateFail();
    ok = p.ClientsAlive == 2 && p.ClientInUse[0] && !p.ClientInUse[1] && p.ClientInUse[2];
    if (!ok) { std::printf("00e3b230_TEST FAIL\n"); return 1; }
    std::printf("00e3b230_TEST PASS\n");
    return 0;
}