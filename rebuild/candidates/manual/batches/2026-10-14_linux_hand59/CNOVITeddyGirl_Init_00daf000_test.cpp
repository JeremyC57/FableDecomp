#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CScriptThing { unsigned char m_body[0xc]; }; static CScriptThing* g_thing = 0; static long g_args[4];
struct CGame { virtual void Slot516(CScriptThing* t, long a) { g_thing = t; g_args[0] = a; std::strcat(g_seq, "a"); } virtual void Slot517(CScriptThing* t, long a, long b) { g_thing = t; g_args[0] = a; g_args[1] = b; std::strcat(g_seq, "b"); } virtual void Slot526(CScriptThing* t, long a) { g_thing = t; g_args[0] = a; std::strcat(g_seq, "c"); }
    virtual void Slot360(CScriptThing* t, long a, long b, long c) { g_thing = t; g_args[0] = a; g_args[1] = b; g_args[2] = c; std::strcat(g_seq, "d"); } virtual void* GetHero() { std::strcat(g_seq, "h"); return (void*)7; } virtual void Slot599(CScriptThing* t, void* h) { g_thing = t; g_args[0] = (long)(h == (void*)7); std::strcat(g_seq, "e"); }
    virtual float Slot264(CScriptThing* t) { g_thing = t; std::strcat(g_seq, "f"); return 12.75f; } virtual void Slot512(CScriptThing* t, long a) { g_thing = t; g_args[0] = a; std::strcat(g_seq, "g"); } };
struct CGirl { long m_0; CGame* PGame; CScriptThing Thing; long m_14, m_18; bool m_1c, m_1d, m_1e, m_1f; void Init() { m_1c = false; m_1d = false; m_1e = false; m_1f = false; PGame->Slot516(&Thing, 0); PGame->Slot517(&Thing, 0, 0); PGame->Slot526(&Thing, 0); PGame->Slot360(&Thing, 0, 1, 0); } };

int main() {
    CGame g; CGirl t; t.PGame = &g; t.m_1c = t.m_1d = t.m_1e = t.m_1f = true; t.Init(); if (t.m_1c || t.m_1d || t.m_1e || t.m_1f || std::strcmp(g_seq, "abcd") || g_args[1] != 1 || g_args[2]) { std::printf("00daf000_TEST FAIL\n"); return 1; }
    std::printf("00daf000_TEST PASS\n"); return 0;
}
