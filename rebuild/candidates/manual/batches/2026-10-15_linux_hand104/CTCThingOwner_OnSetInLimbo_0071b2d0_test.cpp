#include <cstdio>
struct SThingListNode { SThingListNode* next; SThingListNode* prev; int flag; };
struct CThingListIterator {
    SThingListNode* m_node;
    CThingListIterator(SThingListNode* node) : m_node(node) { }
    CThingListIterator(const CThingListIterator& other) : m_node(other.m_node) { }
};
struct CThingList {
    SThingListNode* m_head;
    CThingListIterator begin() { return CThingListIterator(m_head->next); }
    CThingListIterator end() { return CThingListIterator(m_head); }
};
struct CSetThingInLimbo { bool m_flag; CSetThingInLimbo(bool f) : m_flag(f) { } };
static int g_calls = 0;
static CSetThingInLimbo ForEach(CThingListIterator first, CThingListIterator last, CSetThingInLimbo func)
{
    for (; first.m_node != last.m_node; first.m_node = first.m_node->next) { first.m_node->flag = func.m_flag ? 1 : 0; ++g_calls; }
    return func;
}
struct CTCThingOwner {
    CThingList ObjectList;
    void OnSetInLimbo(bool b) { ForEach(ObjectList.begin(), ObjectList.end(), CSetThingInLimbo(b)); }
};
int main()
{
    SThingListNode head, a, b;
    head.next = &a; a.next = &b; b.next = &head; head.prev = &b; b.prev = &a; a.prev = &head;
    head.flag = 7; a.flag = 0; b.flag = 0;
    CTCThingOwner o; o.ObjectList.m_head = &head;
    o.OnSetInLimbo(true);
    if (a.flag != 1 || b.flag != 1 || head.flag != 7 || g_calls != 2) { std::printf("0071b2d0_TEST FAIL\n"); return 1; }
    o.OnSetInLimbo(false);
    if (a.flag != 0 || b.flag != 0 || head.flag != 7 || g_calls != 4) { std::printf("0071b2d0_TEST FAIL\n"); return 1; }
    std::printf("0071b2d0_TEST PASS\n");
    return 0;
}
