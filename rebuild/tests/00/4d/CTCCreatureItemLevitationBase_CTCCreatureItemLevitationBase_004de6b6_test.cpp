#include <cstdio>
struct CThing { int id; };
struct CTCBase {
    CThing* m_pParent;
    CTCBase(CThing* parent) : m_pParent(parent) { }
    virtual ~CTCBase() { }
};
struct SAllocator { SAllocator() { } };
struct SNode { SNode* next; SNode* prev; };
struct CLevitatedItemList {
    SNode* m_pNode; SNode m_sentinel;
    CLevitatedItemList(const SAllocator&) { m_pNode = &m_sentinel; m_sentinel.next = m_sentinel.prev = &m_sentinel; }
    bool empty() const { return m_pNode->next == m_pNode; }
};
struct CTCCreatureItemLevitationBase : CTCBase {
    CLevitatedItemList m_items; long m_count; long m_current;
    CTCCreatureItemLevitationBase(CThing* parent) : CTCBase(parent), m_items(SAllocator()), m_count(0), m_current(0) { }
};
int main()
{
    CThing t;
    CTCCreatureItemLevitationBase l(&t);
    if (l.m_pParent != &t || !l.m_items.empty() || l.m_count != 0 || l.m_current != 0) { std::printf("004de6b6_TEST FAIL\n"); return 1; }
    std::printf("004de6b6_TEST PASS\n");
    return 0;
}