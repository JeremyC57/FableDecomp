// Behaviour test for 0070d100: body copied from src/0070d100.cpp (CGraphicBankManager::ProcessLoading), callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct SScalarTag { SScalarTag() { } };
struct SBankEntry;
static int g_grow = 0;
struct CRetailVector_ptr {
    SBankEntry** first; SBankEntry** last; SBankEntry** end;
    void InsertGrow(SBankEntry** where, SBankEntry* const& value, const SScalarTag&, unsigned long count, bool noTail)
    { ++g_grow; CHECK(where == last && count == 1 && noTail); *last = value; ++last; ++end; }
    void push_back(SBankEntry* const& value)
    {
        if (last != end) {
            new (last) SBankEntry*(value);
            ++last;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
};
struct _List_node { _List_node* _M_next; _List_node* _M_prev; unsigned char _M_data[4]; };
struct list { _List_node* _M_node; };
struct CGraphicBankManager {
    unsigned char _pad00[0x58]; list m_loadingA; unsigned char _pad5c[8]; list m_loadingB;
    void ProcessLoading(CRetailVector_ptr& out);
};
void CGraphicBankManager::ProcessLoading(CRetailVector_ptr& out)
{
    for (_List_node* node = m_loadingA._M_node->_M_next; node != m_loadingA._M_node; node = node->_M_next)
        out.push_back((SBankEntry*)node->_M_data);
    for (_List_node* node2 = m_loadingB._M_node->_M_next; node2 != m_loadingB._M_node; node2 = node2->_M_next)
        out.push_back((SBankEntry*)node2->_M_data);
}
static void link(_List_node* head, _List_node* nodes, int n)
{
    head->_M_next = n ? nodes : head; head->_M_prev = n ? nodes + n - 1 : head;
    for (int i = 0; i < n; ++i) { nodes[i]._M_next = (i + 1 < n) ? nodes + i + 1 : head; nodes[i]._M_prev = i ? nodes + i - 1 : head; }
}
int main()
{
    _List_node headA, nodesA[2], headB, nodesB[3];
    link(&headA, nodesA, 2); link(&headB, nodesB, 3);
    CGraphicBankManager mgr; memset(&mgr, 0, sizeof mgr); mgr.m_loadingA._M_node = &headA; mgr.m_loadingB._M_node = &headB;
    SBankEntry* storage[8]; memset(storage, 0, sizeof storage);
    CRetailVector_ptr out; out.first = storage; out.last = storage; out.end = storage + 4;
    mgr.ProcessLoading(out);
    CHECK(out.last - out.first == 5 && g_grow == 1);
    CHECK(out.first[0] == (SBankEntry*)nodesA[0]._M_data && out.first[1] == (SBankEntry*)nodesA[1]._M_data);
    CHECK(out.first[2] == (SBankEntry*)nodesB[0]._M_data && out.first[4] == (SBankEntry*)nodesB[2]._M_data);
    link(&headA, nodesA, 0); link(&headB, nodesB, 0);              // both lists empty: nothing appended
    out.last = storage; mgr.ProcessLoading(out);
    CHECK(out.last == storage && g_grow == 1);
    link(&headB, nodesB, 1); mgr.ProcessLoading(out);             // only the second list
    CHECK(out.last - out.first == 1 && out.first[0] == (SBankEntry*)nodesB[0]._M_data);
    if (fails == 0) printf("0070d100_TEST PASS\n"); else printf("0070d100_TEST FAIL (%d)\n", fails);
    return fails;
}