#include <cstdio>
struct CModeBase { unsigned long m_type; virtual void v00() { } virtual unsigned long GetType() const { return m_type; } };
struct SModeNode { SModeNode* m_pNext; SModeNode* m_pPrev; CModeBase* m_pMode; void* m_pModeInfo; };
struct CTCCreatureModeManager {
    SModeNode* PModes;
    bool IsModeActive(unsigned long type) const
    {
        SModeNode* end = PModes;
        SModeNode* node = end->m_pNext;
        for (; node != end; node = node->m_pNext) {
            if (node->m_pMode->GetType() == type)
                break;
        }
        return node != PModes;
    }
};
int main()
{
    SModeNode head; CModeBase m1, m2; m1.m_type = 4; m2.m_type = 9;
    SModeNode n1, n2;
    head.m_pNext = &head; head.m_pPrev = &head;
    CTCCreatureModeManager mgr; mgr.PModes = &head;
    if (mgr.IsModeActive(4)) { std::printf("00811340_TEST FAIL\n"); return 1; }
    head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &head; n1.m_pMode = &m1; n2.m_pMode = &m2;
    if (!mgr.IsModeActive(4) || !mgr.IsModeActive(9) || mgr.IsModeActive(5)) { std::printf("00811340_TEST FAIL\n"); return 1; }
    std::printf("00811340_TEST PASS\n");
    return 0;
}
