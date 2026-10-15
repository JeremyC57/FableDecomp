#include <cstdio>
struct CRetailListOd {
    struct SNode { SNode* m_pNext; SNode* m_pPrev; };
    struct iterator { SNode* m_pNode; iterator(SNode* node) : m_pNode(node) { } };
    SNode* m_pHead;
    iterator begin() { SNode* first = m_pHead->m_pNext; return iterator(first); }
};
int main()
{
    CRetailListOd::SNode head, a; head.m_pNext = &a; a.m_pNext = &head; head.m_pPrev = &a; a.m_pPrev = &head;
    CRetailListOd l; l.m_pHead = &head;
    if (l.begin().m_pNode != &a) { std::printf("009f7910_TEST FAIL\n"); return 1; }
    head.m_pNext = &head;
    if (l.begin().m_pNode != &head) { std::printf("009f7910_TEST FAIL\n"); return 1; }
    std::printf("009f7910_TEST PASS\n");
    return 0;
}