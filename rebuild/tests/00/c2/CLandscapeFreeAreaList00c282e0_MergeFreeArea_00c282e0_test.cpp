#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct SArea { unsigned char _pad[0xe]; char m_state; unsigned char _p[9]; SArea* m_pPrev; SArea* m_pNext; };
static SArea* g_a; static SArea* g_b; static SArea* g_c;
struct CList { SArea* MergeBlocks(SArea* f, SArea* s) { std::strcat(g_seq, "M("); std::strcat(g_seq, f == g_a ? "a" : f == g_b ? "b" : "c"); std::strcat(g_seq, ","); std::strcat(g_seq, s == g_a ? "a" : s == g_b ? "b" : "c"); std::strcat(g_seq, ")"); return f; }
    SArea* MergeFreeArea(SArea* area) { if (area->m_pNext && area->m_pNext->m_state == 'N') MergeBlocks(area, area->m_pNext); if (area->m_pPrev && area->m_pPrev->m_state == 'N') return MergeBlocks(area->m_pPrev, area); return area; } };

int main() {
    CList l; SArea a, b, c; g_a = &a; g_b = &b; g_c = &c; std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b); std::memset(&c, 0, sizeof c); b.m_pPrev = &a; b.m_pNext = &c; a.m_state = 'N'; c.m_state = 'N';
    SArea* r = l.MergeFreeArea(&b); if (r != &a || std::strcmp(g_seq, "M(b,c)M(a,b)")) { std::printf("00c282e0_TEST FAIL\n"); return 1; } g_seq[0] = 0; a.m_state = 'U'; r = l.MergeFreeArea(&b); if (r != &b || std::strcmp(g_seq, "M(b,c)")) { std::printf("00c282e0_TEST FAIL\n"); return 1; } g_seq[0] = 0; c.m_state = 'U'; r = l.MergeFreeArea(&b); if (r != &b || std::strcmp(g_seq, "")) { std::printf("00c282e0_TEST FAIL\n"); return 1; }
    std::printf("00c282e0_TEST PASS\n"); return 0;
}