#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
static void note(const char* s) { std::strcat(g_seq, s); }
struct CQ_OrchardFarmRaidScript { unsigned char _pad[0x78]; void* m_pRaidThing; unsigned char _pad7c[0xc]; bool m_cutsceneDone; void DoMultiplierCutscene() { note("C"); } };
static bool g_aborted = false;
struct CMK_OFI_GWLL_WHIS2 { void* __vftable; unsigned char _pad04[0x10]; CQ_OrchardFarmRaidScript* ParentClass; bool IsAborted() { note("A"); return g_aborted; }
    void Main() { if (!ParentClass->m_cutsceneDone && !ParentClass->m_pRaidThing && !IsAborted()) { ParentClass->m_cutsceneDone = true; ParentClass->DoMultiplierCutscene(); } } };

int main()
{
    CQ_OrchardFarmRaidScript parent; parent.m_pRaidThing = 0; parent.m_cutsceneDone = false; CMK_OFI_GWLL_WHIS2 s; s.ParentClass = &parent;
    g_aborted = true; s.Main(); if (std::strcmp(g_seq, "A") || parent.m_cutsceneDone) { std::printf("00dd1eb0_TEST FAIL\n"); return 1; } g_aborted = false; int thing; parent.m_pRaidThing = &thing; g_seq[0] = 0; s.Main(); if (std::strcmp(g_seq, "") || parent.m_cutsceneDone) { std::printf("00dd1eb0_TEST FAIL\n"); return 1; }
    parent.m_pRaidThing = 0; s.Main(); if (std::strcmp(g_seq, "AC") || !parent.m_cutsceneDone) { std::printf("00dd1eb0_TEST FAIL\n"); return 1; } g_seq[0] = 0; s.Main(); if (std::strcmp(g_seq, "")) { std::printf("00dd1eb0_TEST FAIL\n"); return 1; }
    std::printf("00dd1eb0_TEST PASS\n");
    return 0;
}
