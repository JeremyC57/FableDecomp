// Behaviour test for 00a17480 (STLport list<SEntry>::insert(where), labelled
// CCombatSequence_WillForcePush::OnCreate): a default entry is copied into a malloc'd node linked
// just before `where`, and the returned iterator names that node.
#include <cstdio>
#include <cstdlib>

struct SEntry {
    void* m_pThing; void* m_pCount; long m_values[5]; const char* m_name;
    SEntry() : m_pThing(0), m_pCount(0), m_name("") { for (int i = 0; i < 5; ++i) m_values[i] = 0; }
};
struct SListNode { SListNode* m_pNext; SListNode* m_pPrev; SEntry m_data; };
struct SListIterator { SListNode* m_pNode; SListIterator(SListNode* n) : m_pNode(n) { } };

static SListIterator Insert(SListIterator where, const SEntry& value)
{
    SListNode* node = (SListNode*)std::malloc(sizeof(SListNode));
    node->m_data = value;
    SListNode* next = where.m_pNode;
    SListNode* prev = next->m_pPrev;
    node->m_pNext = next;
    node->m_pPrev = prev;
    prev->m_pNext = node;
    next->m_pPrev = node;
    return SListIterator(node);
}
static SListIterator Insert(SListIterator where) { return Insert(where, SEntry()); }

int main()
{
    SListNode head; head.m_pNext = &head; head.m_pPrev = &head;
    SListIterator a = Insert(SListIterator(&head));          // push_back on an empty list
    SListIterator b = Insert(SListIterator(a.m_pNode));      // before the first element
    bool ok = head.m_pNext == b.m_pNode && b.m_pNode->m_pNext == a.m_pNode && a.m_pNode->m_pNext == &head
        && head.m_pPrev == a.m_pNode && a.m_pNode->m_pPrev == b.m_pNode && b.m_pNode->m_pPrev == &head
        && a.m_pNode->m_data.m_pThing == 0 && a.m_pNode->m_data.m_pCount == 0 && a.m_pNode->m_data.m_name[0] == 0;
    std::free(a.m_pNode); std::free(b.m_pNode);
    if (!ok) { std::printf("00a17480_TEST FAIL\n"); return 1; }
    std::printf("00a17480_TEST PASS\n");
    return 0;
}