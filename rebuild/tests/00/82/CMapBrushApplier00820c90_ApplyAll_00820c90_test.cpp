#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_filters[4]; static long g_nFilters = 0; static long g_flag = 0;
struct CModeBlock { int x; void* GetAnimFilter(long flag) { std::strcat(g_seq, "G"); g_flag = flag; return this; } };
struct SBankNode { SBankNode* m_pNext; SBankNode* m_pPrev; CModeBlock m_block; };
struct CMapBrushApplier { unsigned char _pad[0xc]; SBankNode* m_pHead; void ApplyScriptedMapBrush(void* filter) { std::strcat(g_seq, "A"); g_filters[g_nFilters++] = filter; }
    void ApplyAll() { for (SBankNode* node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) ApplyScriptedMapBrush(node->m_block.GetAnimFilter(1)); } };

int main() {
    SBankNode head, n1, n2; head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &head; CMapBrushApplier a; a.m_pHead = &head; a.ApplyAll();
    if (std::strcmp(g_seq, "GAGA") || g_filters[0] != &n1.m_block || g_filters[1] != &n2.m_block || g_flag != 1) { std::printf("00820c90_TEST FAIL\n"); return 1; } head.m_pNext = &head; g_seq[0] = 0; a.ApplyAll(); if (g_seq[0]) { std::printf("00820c90_TEST FAIL\n"); return 1; }
    std::printf("00820c90_TEST PASS\n"); return 0;
}