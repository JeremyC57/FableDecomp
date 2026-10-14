#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CInputProcessBase; struct SProcessNode { CInputProcessBase* m_pOwner; SProcessNode* m_pNext; SProcessNode* m_pPrev; }; struct CInputProcessBase { unsigned char _pad[8]; SProcessNode m_node; };
struct CInputProcessManager { virtual void v00() { } SProcessNode* m_pFirst; SProcessNode* m_pLast; long m_count;
    void AddInputProcess(CInputProcessBase* process) { SProcessNode* node = &process->m_node; node->m_pNext = 0; node->m_pPrev = m_pLast; node->m_pOwner = process; if (m_pLast) m_pLast->m_pNext = node; m_pLast = node; if (!m_pFirst) m_pFirst = node; ++m_count; } };

int main() {
    CInputProcessManager m; m.m_pFirst = 0; m.m_pLast = 0; m.m_count = 0; CInputProcessBase a, b; m.AddInputProcess(&a);
    if (m.m_pFirst != &a.m_node || m.m_pLast != &a.m_node || m.m_count != 1 || a.m_node.m_pOwner != &a || a.m_node.m_pNext || a.m_node.m_pPrev) { std::printf("00a0d4f0_TEST FAIL\n"); return 1; }
    m.AddInputProcess(&b); if (m.m_pFirst != &a.m_node || m.m_pLast != &b.m_node || m.m_count != 2 || a.m_node.m_pNext != &b.m_node || b.m_node.m_pPrev != &a.m_node || b.m_node.m_pNext) { std::printf("00a0d4f0_TEST FAIL\n"); return 1; }
    std::printf("00a0d4f0_TEST PASS\n"); return 0;
}