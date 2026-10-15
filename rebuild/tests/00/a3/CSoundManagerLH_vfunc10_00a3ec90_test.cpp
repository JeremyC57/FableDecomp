#include <cstdio>
struct SNode { SNode* next; SNode* prev; };
struct CList {
    SNode* m_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        SNode* first = m_node->next; SNode* last = m_node;
        while (first != last) { first = first->next; ++n; }
        return n;
    }
};
struct CSoundManagerLH {
    unsigned long m_maxEntries; CList m_entries;
    unsigned long vfunc_10() { return m_maxEntries - m_entries.size(); }
};
int main()
{
    SNode head, a, b;
    head.next = &head; head.prev = &head;
    CSoundManagerLH m; m.m_entries.m_node = &head; m.m_maxEntries = 2;
    bool ok = m.vfunc_10() == 2;
    head.next = &a; a.next = &b; b.next = &head;
    ok = ok && m.vfunc_10() == 0;
    m.m_maxEntries = 3;
    ok = ok && m.vfunc_10() == 1;
    if (!ok) { std::printf("00a3ec90_TEST FAIL\n"); return 1; }
    std::printf("00a3ec90_TEST PASS\n");
    return 0;
}