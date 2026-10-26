#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static const char* g_text = 0; static long g_len = 0; static long g_live = 0; class CCharString { public: void* m_p; CCharString(const char* t, long n) : m_p(0) { g_text = t; g_len = n; ++g_live; } ~CCharString() { --g_live; } CCharString& operator=(const char* t) { g_text = t; return *this; } };
class CScriptThing { public: unsigned char m_body[0xc]; }; static CScriptThing* g_thing = 0; class CGame { public: virtual void Slot125(const CCharString& n) { if (g_live != 1 || std::strcmp(g_text, "OBJECT_TEDDY_BEAR_UNGIVEABLE")) std::strcat(g_seq, "X"); std::strcat(g_seq, "N"); } virtual void Slot361(CScriptThing* t) { g_thing = t; std::strcat(g_seq, "T"); } };
struct CQuest { void AddGoodDeed() { std::strcat(g_seq, "G"); } }; struct SRec { unsigned char _pad[0x54]; CCharString m_name; SRec() : m_name("", 0) { } };
struct CGirl { long m_0; CGame* PGame; CScriptThing Thing; CQuest* PQuest; SRec* PRecord; bool m_1c, m_1d; void GivenTeddy() { { CCharString name("OBJECT_TEDDY_BEAR_UNGIVEABLE", -1); PGame->Slot125(name); } m_1d = true; PQuest->AddGoodDeed(); PGame->Slot361(&Thing); PRecord->m_name = "B"; } };

int main() {
    CGame g; CQuest q; SRec r; g_live = 0; g_text = 0; CGirl t; t.PGame = &g; t.PQuest = &q; t.PRecord = &r; t.m_1d = false; t.GivenTeddy(); if (std::strcmp(g_seq, "NGT") || !t.m_1d || g_live || std::strcmp(g_text, "B") || g_thing != &t.Thing) { std::printf("00db0600_TEST FAIL\n"); return 1; }
    std::printf("00db0600_TEST PASS\n"); return 0;
}