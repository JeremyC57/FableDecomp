#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static const char* g_text = 0; static long g_len = 0; static long g_live = 0; class CCharString { public: void* m_p; CCharString(const char* t, long n) : m_p(0) { g_text = t; g_len = n; ++g_live; } ~CCharString() { --g_live; } CCharString& operator=(const char* t) { g_text = t; return *this; } };
static long g_a = 0, g_deed = 0; class CScriptThing { public: unsigned char m_body[0xc]; }; class CGame { public: virtual void Slot125(const CCharString& n) { if (g_live != 1) std::strcat(g_seq, "X"); std::strcat(g_seq, "N"); } virtual void Slot126(long a) { g_a = a; std::strcat(g_seq, "1"); } };
struct CQuest { unsigned char _pad[0x70]; bool m_70; unsigned char _p[0x20]; bool m_91; void AddBadDeed(long n) { g_deed = n; std::strcat(g_seq, "B"); } };
struct CBully { long m_0; CGame* PGame; CScriptThing Thing; CQuest* PQuest; void GivenTeddy() { PGame->Slot126(1); { CCharString name("OBJECT_TEDDY_BEAR_UNGIVEABLE", -1); PGame->Slot125(name); } PQuest->m_70 = true; PQuest->m_91 = true; PQuest->AddBadDeed(3); } };

int main() {
    CGame g; CQuest q; q.m_70 = false; q.m_91 = false; CBully b; b.PGame = &g; b.PQuest = &q; b.GivenTeddy(); if (std::strcmp(g_seq, "1NB") || g_a != 1 || std::strcmp(g_text, "OBJECT_TEDDY_BEAR_UNGIVEABLE") || g_len != -1 || g_live || !q.m_70 || !q.m_91 || g_deed != 3) { std::printf("00dbcd00_TEST FAIL\n"); return 1; }
    std::printf("00dbcd00_TEST PASS\n"); return 0;
}