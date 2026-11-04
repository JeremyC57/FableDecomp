#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[32] = ""; static void* g_thing = 0; static void* g_regHandle = 0; static bool g_done = false; static void* g_fadeHandle = 0; static long g_fadeArg = -1;
struct CThingVtable_00ef2150 { virtual void Update() { std::strcat(g_seq, "U"); } virtual void AttachEffect(void* h, long x, long flag) { std::strcat(g_seq, "F"); g_fadeHandle = h; g_fadeArg = flag; } };
class CScriptThing { public: void* m_pThing; CScriptThing(void* t) : m_pThing(t) { g_thing = t; std::strcat(g_seq, "c"); } ~CScriptThing() { std::strcat(g_seq, "d"); } };
struct CIsThingAlive { virtual void v00() { } CScriptThing m_thing; CIsThingAlive(void* t) : m_thing(t) { } ~CIsThingAlive() { } };
struct CFadeinSummoner { long m_0; CThingVtable_00ef2150* m_pThing; unsigned char m_handle[4]; void RegisterFilter(CIsThingAlive& f) { std::strcat(g_seq, "R"); g_regHandle = f.m_thing.m_pThing; } bool IsDone() { std::strcat(g_seq, "I"); return g_done; }
    void Init() { { CIsThingAlive filter(m_handle); RegisterFilter(filter); } m_pThing->Update(); if (!IsDone()) m_pThing->AttachEffect(m_handle, 0, 1); } };
int main() {
    CThingVtable_00ef2150 t; CFadeinSummoner s; s.m_pThing = &t; s.Init(); if (std::strcmp(g_seq, "cRdUIF") || g_thing != s.m_handle || g_regHandle != s.m_handle || g_fadeHandle != s.m_handle || g_fadeArg != 1) { std::printf("00ef2150_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_done = true; s.Init(); if (std::strcmp(g_seq, "cRdUI")) { std::printf("00ef2150_TEST FAIL\n"); return 1; }
    std::printf("00ef2150_TEST PASS\n"); return 0;
}