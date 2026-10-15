#include <cstdio>
struct CVtable_012B5868 { virtual ~CVtable_012B5868() { } };
struct CVtable_012B8778 : CVtable_012B5868 {
    CVtable_012B8778* m_pPrev; CVtable_012B8778* m_pNext;
    CVtable_012B8778() : m_pPrev(0), m_pNext(0) { }
    void Link(CVtable_012B8778* prev, CVtable_012B8778* next) { if (prev) prev->m_pNext = this; if (next) next->m_pPrev = this; m_pPrev = prev; m_pNext = next; }
    void Unlink() { if (m_pPrev) m_pPrev->m_pNext = m_pNext; if (m_pNext) m_pNext->m_pPrev = m_pPrev; m_pPrev = 0; m_pNext = 0; }
    virtual ~CVtable_012B8778() { Unlink(); }
};
int main()
{
    CVtable_012B8778 a, c;
    CVtable_012B8778* b = new CVtable_012B8778;
    b->Link(&a, &c);
    if (a.m_pNext != b || c.m_pPrev != b) { std::printf("00c4cf40_TEST FAIL\n"); return 1; }
    delete b;
    if (a.m_pNext != &c || c.m_pPrev != &a) { std::printf("00c4cf40_TEST FAIL\n"); return 1; }
    CVtable_012B8778* d = new CVtable_012B8778; d->Link(0, &a); delete d;
    if (a.m_pPrev != 0) { std::printf("00c4cf40_TEST FAIL\n"); return 1; }
    std::printf("00c4cf40_TEST PASS\n");
    return 0;
}