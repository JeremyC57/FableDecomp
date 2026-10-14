#include <cstdio>
static int g_left = 0, g_cleared = 0, g_adds = 0; static long g_added = -1;
struct CCreatureMode_0073ae70 { virtual void OnLeave() { ++g_left; } };
struct SModeNode { SModeNode* m_pNext; SModeNode* m_pPrev; CCreatureMode_0073ae70* m_pMode; void* m_pCount; };
struct CRetailList_CountedPointer { SModeNode* m_pHead; void clear() { ++g_cleared; } };
struct CTCCreatureModeManager { unsigned char _pad[0xc]; CRetailList_CountedPointer m_modes; void AddMode(long mode) { g_added = mode; ++g_adds; }
    void ClearModes() { if (m_modes.m_pHead->m_pNext != m_modes.m_pHead) m_modes.m_pHead->m_pNext->m_pMode->OnLeave(); m_modes.clear(); AddMode(0); } };
int main() {
    SModeNode head, n1; CCreatureMode_0073ae70 mode; head.m_pNext = &n1; n1.m_pNext = &head; n1.m_pMode = &mode;
    CTCCreatureModeManager m; m.m_modes.m_pHead = &head; m.ClearModes(); if (g_left != 1 || g_cleared != 1 || g_added != 0 || g_adds != 1) { std::printf("0073ae70_TEST FAIL\n"); return 1; }
    head.m_pNext = &head; m.ClearModes(); if (g_left != 1 || g_cleared != 2 || g_adds != 2) { std::printf("0073ae70_TEST FAIL\n"); return 1; }
    std::printf("0073ae70_TEST PASS\n"); return 0;
}