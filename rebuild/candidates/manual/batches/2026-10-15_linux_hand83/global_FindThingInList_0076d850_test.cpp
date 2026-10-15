// Behaviour test for 0076d850 (_global::FindThingInList): the find front end forwards to the tagged loop and returns the first node holding the thing, or last.
#include <cstdio>

struct CThing { long m_id; };

struct SThingListNode {
    SThingListNode* m_pNext;
    SThingListNode* m_pPrev;
    CThing*         m_pItem;
};

struct CThingListIterator {
    SThingListNode* m_pNode;
    CThingListIterator() { }

};

struct CIteratorTag { CIteratorTag() { } };

CThingListIterator FindThingInListImpl(CThingListIterator first, CThingListIterator last, CThing* const& value, const CIteratorTag&)
{
    while (first.m_pNode != last.m_pNode && first.m_pNode->m_pItem != value)
        first.m_pNode = first.m_pNode->m_pNext;
    return first;
}

CThingListIterator FindThingInList(CThingListIterator first, CThingListIterator last, CThing* const& value)
{
    return FindThingInListImpl(first, last, value, CIteratorTag());
}

int main()
{
    CThing a = { 1 }, b = { 2 }, c = { 3 };
    SThingListNode head, n1, n2;
    head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &head;
    head.m_pPrev = &n2; n1.m_pPrev = &head; n2.m_pPrev = &n1;
    head.m_pItem = 0; n1.m_pItem = &a; n2.m_pItem = &b;
    CThingListIterator first, last;
    first.m_pNode = &n1; last.m_pNode = &head;
    CThing* pa = &a; CThing* pb = &b; CThing* pc = &c;
    if (FindThingInList(first, last, pb).m_pNode != &n2 || FindThingInList(first, last, pa).m_pNode != &n1
        || FindThingInList(first, last, pc).m_pNode != &head) { std::printf("0076d850_TEST FAIL\n"); return 1; }
    std::printf("0076d850_TEST PASS\n");
    return 0;
}
