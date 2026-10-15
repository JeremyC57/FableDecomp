// Behaviour test for 00567a40 (catalog CTCCameraPointScripted::GetLookDirection): the list find front end returns the first node whose data equals the value, or last.
#include <cstdio>

struct CWideString { int id; };

struct SListNode
{
    SListNode*   _M_next;
    SListNode*   _M_prev;
    CWideString* _M_data;
};

struct SListIterator
{
    SListNode* _M_node;
    SListIterator() {}
    SListIterator(SListNode* node) : _M_node(node) {}
    SListIterator(const SListIterator& other) : _M_node(other._M_node) {}
};

struct SInputIteratorTag { SInputIteratorTag() {} };

SListIterator FindInListRangeTagged(SListIterator first, SListIterator last, CWideString* const& value, const SInputIteratorTag&)
{
    while (first._M_node != last._M_node && first._M_node->_M_data != value)
        first._M_node = first._M_node->_M_next;
    return first;
}

SListIterator FindInListRange(SListIterator first, SListIterator last, CWideString* const& value)
{
    return FindInListRangeTagged(first, last, value, SInputIteratorTag());
}

int main()
{
    CWideString a = { 1 }, b = { 2 }, c = { 3 };
    SListNode head, n1, n2;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &head;
    n1._M_data = &a; n2._M_data = &b; head._M_data = 0;
    CWideString* pb = &b; CWideString* pc = &c;
    if (FindInListRange(SListIterator(&n1), SListIterator(&head), pb)._M_node != &n2) { std::printf("00567a40_TEST FAIL\n"); return 1; }
    if (FindInListRange(SListIterator(&n1), SListIterator(&head), pc)._M_node != &head) { std::printf("00567a40_TEST FAIL\n"); return 1; }
    std::printf("00567a40_TEST PASS\n");
    return 0;
}
