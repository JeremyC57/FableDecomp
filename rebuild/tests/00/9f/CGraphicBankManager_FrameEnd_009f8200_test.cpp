#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_budget = 0; static double g_time = 0; static long g_sizes[4]; static long g_n = 0;
class CResourceList { public: void ProcessLoading(long budget, double time) { std::strcat(g_seq, "P"); g_budget = budget; g_time = time; } };
class CGraphicDataBank { public: unsigned char _pad[0x164]; CResourceList m_resources; long m_size; void FrameEnd() { std::strcat(g_seq, "E"); } void StepLoading() { std::strcat(g_seq, "S"); } long Sizeof() { std::strcat(g_seq, "Z"); return m_size; } };
struct SBankNode { SBankNode* m_pNext; SBankNode* m_pPrev; unsigned char _pad08[0x30]; CGraphicDataBank* m_pBank; };
class CGraphicBankManager { public: unsigned char _pad[4]; SBankNode* m_pHead;
    void FrameEnd() { for (SBankNode* node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) node->m_pBank->FrameEnd(); }
    void ProcessLoading(long budget, double time) { SBankNode* node; for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) node->m_pBank->StepLoading(); for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) node->m_pBank->m_resources.ProcessLoading(budget, time); }
    long Sizeof() { long count = 0; SBankNode* node; for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) ++count; long size = count * 4 + 4; for (node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) { long entry = node->m_pBank ? node->m_pBank->Sizeof() + 8 : 8; size += entry; } return size; } };
static SBankNode g_head, g_n1, g_n2; static CGraphicDataBank g_b1, g_b2;
static CGraphicBankManager* setup() { static CGraphicBankManager m; g_head.m_pNext = &g_n1; g_n1.m_pNext = &g_n2; g_n2.m_pNext = &g_head; g_n1.m_pBank = &g_b1; g_n2.m_pBank = &g_b2; g_b1.m_size = 100; g_b2.m_size = 1000; m.m_pHead = &g_head; g_seq[0] = 0; return &m; }

int main() {
    CGraphicBankManager* m = setup(); m->FrameEnd(); if (std::strcmp(g_seq, "EE")) { std::printf("009f8200_TEST FAIL\n"); return 1; } g_head.m_pNext = &g_head; g_seq[0] = 0; m->FrameEnd(); if (g_seq[0]) { std::printf("009f8200_TEST FAIL\n"); return 1; }
    std::printf("009f8200_TEST PASS\n"); return 0;
}