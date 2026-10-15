#include <cstdio>
#include <cstring>
#include <cstdlib>

static char g_seq[64] = ""; static long g_entry = -1, g_word = -1; static unsigned long g_flags = 0; static long g_mapped = 500;
struct SInfo { unsigned long flags; };
struct CMap { long& operator[](const long& k) { std::strcat(g_seq, "M"); g_mapped += k; return g_mapped; } };
struct CBase { virtual ~CBase() { } virtual const SInfo* GetStateInfo(long s) { std::strcat(g_seq, "I"); static SInfo i; i.flags = g_flags; return &i; } virtual void ApplyMorph(long e, long w) { std::strcat(g_seq, "A"); g_entry = e; g_word = w; }
    void ProcessChangeState() { std::strcat(g_seq, "P"); } };
struct CSprite : CBase { unsigned char pad[0x144]; long state; long x; long word; unsigned char pad2[0x38]; CMap map;
    void ProcessChangeState() { CBase::ProcessChangeState(); if (GetStateInfo(state)->flags & 8) ApplyMorph(map[state], word); } };
int main()
{
    CSprite s; s.state = 3; s.word = 77;
    s.ProcessChangeState(); if (std::strcmp(g_seq, "PI") || g_entry != -1) { std::printf("005546f0_TEST FAIL\n"); return 1; }
    g_flags = 8; g_seq[0] = 0; s.ProcessChangeState(); if (std::strcmp(g_seq, "PIMA") || g_entry != 503 || g_word != 77) { std::printf("005546f0_TEST FAIL\n"); return 1; }
    g_flags = 7; g_seq[0] = 0; s.ProcessChangeState(); if (std::strcmp(g_seq, "PI")) { std::printf("005546f0_TEST FAIL\n"); return 1; }
    std::printf("005546f0_TEST PASS\n");
    return 0;
}