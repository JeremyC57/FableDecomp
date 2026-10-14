#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct SArea { SArea* m_pNext; long m_4; SArea* m_pPrev; }; static SArea* g_inserted = 0; static SArea* g_consolidated = 0;
struct CMemoryAreaChunk { unsigned char _pad[8]; long m_noAreas; unsigned char _p[0x24]; SArea* m_pFirstFreeArea; void InsertAreaIntoFreeList(SArea* a) { g_inserted = a; std::strcat(g_seq, "I"); } void ConsolidateAdjacentFreeAreas(SArea* a) { g_consolidated = a; std::strcat(g_seq, "C"); }
    void RemoveAreaFromFreeList(SArea* area) { if (area->m_pPrev) area->m_pPrev->m_pNext = area->m_pNext; else m_pFirstFreeArea = area->m_pNext; if (area->m_pNext) area->m_pNext->m_pPrev = area->m_pPrev; area->m_pNext = 0; area->m_pPrev = 0; --m_noAreas; InsertAreaIntoFreeList(area); ConsolidateAdjacentFreeAreas(area); } };

int main() {
    SArea a, b, c; std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b); std::memset(&c, 0, sizeof c); a.m_pNext = &b; b.m_pPrev = &a; b.m_pNext = &c; c.m_pPrev = &b; CMemoryAreaChunk* ch = new CMemoryAreaChunk; std::memset(ch, 0, sizeof *ch); ch->m_pFirstFreeArea = &a; ch->m_noAreas = 3;
    ch->RemoveAreaFromFreeList(&b); if (a.m_pNext != &c || c.m_pPrev != &a || b.m_pNext || b.m_pPrev || ch->m_noAreas != 2 || std::strcmp(g_seq, "IC") || g_inserted != &b || g_consolidated != &b) { std::printf("00a658e0_TEST FAIL\n"); return 1; } ch->RemoveAreaFromFreeList(&a); if (ch->m_pFirstFreeArea != &c || c.m_pPrev || ch->m_noAreas != 1) { std::printf("00a658e0_TEST FAIL\n"); return 1; } delete ch;
    std::printf("00a658e0_TEST PASS\n"); return 0;
}
