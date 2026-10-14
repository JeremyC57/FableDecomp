#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CScriptThing { unsigned char m_body[0xc]; }; static CScriptThing* g_thing = 0; static long g_args[4];
struct CGame { virtual void Slot516(CScriptThing* t, long a) { g_thing = t; g_args[0] = a; std::strcat(g_seq, "a"); } virtual void Slot517(CScriptThing* t, long a, long b) { g_thing = t; g_args[0] = a; g_args[1] = b; std::strcat(g_seq, "b"); } virtual void Slot526(CScriptThing* t, long a) { g_thing = t; g_args[0] = a; std::strcat(g_seq, "c"); }
    virtual void Slot360(CScriptThing* t, long a, long b, long c) { g_thing = t; g_args[0] = a; g_args[1] = b; g_args[2] = c; std::strcat(g_seq, "d"); } virtual void* GetHero() { std::strcat(g_seq, "h"); return (void*)7; } virtual void Slot599(CScriptThing* t, void* h) { g_thing = t; g_args[0] = (long)(h == (void*)7); std::strcat(g_seq, "e"); }
    virtual float Slot264(CScriptThing* t) { g_thing = t; std::strcat(g_seq, "f"); return 12.75f; } virtual void Slot512(CScriptThing* t, long a) { g_thing = t; g_args[0] = a; std::strcat(g_seq, "g"); } };
struct SQuest { unsigned char _pad[0x58]; long m_58; }; struct CScript { long m_0; CGame* PGame; CScriptThing Thing; SQuest* PQuest; long m_18; long m_value; long m_20; void Init() { long v = (long)PGame->Slot264(&Thing); m_value = v; m_20 = 0; PQuest->m_58 = v; PGame->Slot517(&Thing, 0, 1); PGame->Slot512(&Thing, 1); } };

int main() {
    CGame g; SQuest q; CScript s; s.PGame = &g; s.PQuest = &q; s.m_20 = 5; s.Init(); if (s.m_value != 12 || q.m_58 != 12 || s.m_20 || std::strcmp(g_seq, "fbg") || g_args[0] != 1) { std::printf("00e99760_TEST FAIL\n"); return 1; }
    std::printf("00e99760_TEST PASS\n"); return 0;
}
