#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
class CScriptThing { public: unsigned char m_body[0xc]; }; static CScriptThing* g_thing = 0; struct SQuest { unsigned char _pad[0x50]; long m_state; }; class CGame { public: virtual void Slot108(CScriptThing* t, long, long) { g_thing = t; std::strcat(g_seq, "A"); } virtual void Slot517(CScriptThing* t, long, long) { g_thing = t; std::strcat(g_seq, "B"); } };
struct CScript { long m_0; CGame* PGame; CScriptThing Thing; SQuest* PQuest; long PMasterData; long m_counter; void Init() { m_counter = 0; if (PQuest->m_state != 6) PGame->Slot108(&Thing, 0, 0); PGame->Slot517(&Thing, 0, 1); } };

int main() {
    CGame g; SQuest q; q.m_state = 6; CScript s; s.PGame = &g; s.PQuest = &q; s.m_counter = 9; s.Init(); if (s.m_counter || std::strcmp(g_seq, "B") || g_thing != &s.Thing) { std::printf("00eace90_TEST FAIL\n"); return 1; } g_seq[0] = 0; q.m_state = 7; s.Init(); if (std::strcmp(g_seq, "AB")) { std::printf("00eace90_TEST FAIL\n"); return 1; }
    std::printf("00eace90_TEST PASS\n"); return 0;
}