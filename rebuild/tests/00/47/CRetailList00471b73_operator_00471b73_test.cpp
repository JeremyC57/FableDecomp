// Behaviour test for 0x00471B73 (see src/00471b73.cpp): the STLport list assignment (shrink via erase, grow via insert, self-assignment).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#define CHECK(c) do { if (!(c)) { std::printf("00471b73_TEST FAIL\n"); return 1; } } while (0)

struct SRecord24 { unsigned int m_data[6]; };
struct SListNode { SListNode* m_pNext; SListNode* m_pPrev; SRecord24 m_data; };
struct CList {
    SListNode* m_pNode;
    CList() { m_pNode = new SListNode; m_pNode->m_pNext = m_pNode->m_pPrev = m_pNode; }
    ~CList() { erase(m_pNode->m_pNext, m_pNode); delete m_pNode; }
    unsigned int size() const { unsigned int n = 0; for (SListNode* p = m_pNode->m_pNext; p != m_pNode; p = p->m_pNext) ++n; return n; }
    void push_back(unsigned int v) { SListNode* n = new SListNode; for (int i = 0; i < 6; ++i) n->m_data.m_data[i] = v + i; n->m_pNext = m_pNode; n->m_pPrev = m_pNode->m_pPrev; m_pNode->m_pPrev->m_pNext = n; m_pNode->m_pPrev = n; }
    SListNode* erase(SListNode* first, SListNode* last)
    {
        while (first != last) { SListNode* n = first; first = first->m_pNext; n->m_pPrev->m_pNext = n->m_pNext; n->m_pNext->m_pPrev = n->m_pPrev; delete n; }
        return last;
    }
    void insert(SListNode* pos, const SListNode* first, const SListNode* last)
    {
        for (; first != last; first = first->m_pNext) { SListNode* n = new SListNode; n->m_data = first->m_data; n->m_pNext = pos; n->m_pPrev = pos->m_pPrev; pos->m_pPrev->m_pNext = n; pos->m_pPrev = n; }
    }
    CList& operator=(const CList& other)
    {
        if (this != &other) {
            SListNode* first1 = m_pNode->m_pNext; SListNode* last1 = m_pNode;
            const SListNode* first2 = other.m_pNode->m_pNext; const SListNode* last2 = other.m_pNode;
            while (first1 != last1 && first2 != last2) { first1->m_data = first2->m_data; first1 = first1->m_pNext; first2 = first2->m_pNext; }
            if (first2 == last2) erase(first1, last1); else insert(last1, first2, last2);
        }
        return *this;
    }
};
int main()
{
    CList a, b, c;
    a.push_back(10); a.push_back(20); a.push_back(30);
    b.push_back(1);
    b = a;
    CHECK(b.size() == 3 && b.m_pNode->m_pNext->m_data.m_data[0] == 10 && b.m_pNode->m_pPrev->m_data.m_data[5] == 35);
    c.push_back(5); c.push_back(6); c.push_back(7); c.push_back(8);
    c = b;
    CHECK(c.size() == 3 && c.m_pNode->m_pNext->m_pNext->m_data.m_data[0] == 20);
    a = a;
    CHECK(a.size() == 3);
    CList empty; a = empty;
    CHECK(a.size() == 0);
    std::printf("00471b73_TEST PASS\n");
    return 0;
}