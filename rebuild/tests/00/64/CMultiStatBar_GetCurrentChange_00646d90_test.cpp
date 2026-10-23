#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SNode { SNode* m_pNext; SNode* m_pPrev; long m_change; };
struct CChangeList { SNode* m_pHead; bool empty() const { return m_pHead->m_pNext == m_pHead; } long& front() const { return m_pHead->m_pNext->m_change; } };
struct CBar { unsigned char _pad[0x28]; long m_state; unsigned char _p[0x3c]; CChangeList m_changes; long GetCurrentChange() { if (m_state != 1 && !m_changes.empty()) return m_changes.front(); return 6; } };

int main() {
    SNode head, n; head.m_pNext = &head; n.m_change = 42; n.m_pNext = &head; CBar* b = new CBar; b->m_state = 0; b->m_changes.m_pHead = &head; if (b->GetCurrentChange() != 6) { std::printf("00646d90_TEST FAIL\n"); return 1; } head.m_pNext = &n; if (b->GetCurrentChange() != 42) { std::printf("00646d90_TEST FAIL\n"); return 1; } b->m_state = 1; if (b->GetCurrentChange() != 6) { std::printf("00646d90_TEST FAIL\n"); return 1; } delete b;
    std::printf("00646d90_TEST PASS\n"); return 0;
}