#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SNode { SNode* volatile m_pNext; SNode* m_pPrev; long m_change; }; struct CBar { unsigned char _pad[0x28]; long m_state; unsigned char _p[0x3c]; SNode* m_pChanges; long GetCurrentChange() { if (m_state != 1) { SNode* head = m_pChanges; if (head->m_pNext != head) return head->m_pNext->m_change; } return 6; } };

int main() {
    SNode head, n; head.m_pNext = &head; n.m_change = 42; n.m_pNext = &head; CBar* b = new CBar; b->m_state = 0; b->m_pChanges = &head; if (b->GetCurrentChange() != 6) { std::printf("00646d90_TEST FAIL\n"); return 1; } head.m_pNext = &n; if (b->GetCurrentChange() != 42) { std::printf("00646d90_TEST FAIL\n"); return 1; } b->m_state = 1; if (b->GetCurrentChange() != 6) { std::printf("00646d90_TEST FAIL\n"); return 1; } delete b;
    std::printf("00646d90_TEST PASS\n"); return 0;
}