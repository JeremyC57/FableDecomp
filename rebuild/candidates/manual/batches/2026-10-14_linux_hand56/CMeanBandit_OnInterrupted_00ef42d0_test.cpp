#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CScriptThing { unsigned char m_body[8]; }; static CScriptThing* g_stopped = 0; static CScriptThing* g_released = 0; static long g_reason = -1;
struct CCreature { virtual void OnScriptThingStopped(CScriptThing* t) { g_stopped = t; std::strcat(g_seq, "S"); } virtual void OnScriptThingReleased(CScriptThing* t, long r) { g_released = t; g_reason = r; std::strcat(g_seq, "R"); } };
struct CMeanBandit { long m_0; CCreature* m_pCreature; CScriptThing m_thing; void OnInterrupted() { m_pCreature->OnScriptThingStopped(&m_thing); m_pCreature->OnScriptThingReleased(&m_thing, 0); } };

int main() {
    CCreature c; CMeanBandit b; b.m_pCreature = &c; b.OnInterrupted(); if (g_stopped != &b.m_thing || g_released != &b.m_thing || g_reason != 0 || std::strcmp(g_seq, "SR")) { std::printf("00ef42d0_TEST FAIL\n"); return 1; }
    std::printf("00ef42d0_TEST PASS\n"); return 0;
}
