// Behaviour test for 00535320 (catalog CTCCameraPointScripted::GetLookDirection): the find front end returns the first in-order node holding the value, or last.
#include <cstdio>

struct STreeNode
{
    STreeNode* _M_left;
    STreeNode* _M_parent;
    STreeNode* _M_right;
    long       _M_value;
};

struct STreeIterator
{
    STreeNode* _M_node;
    STreeIterator() {}
    STreeIterator(STreeNode* node) : _M_node(node) {}
};

struct SInputIteratorTag { SInputIteratorTag() {} };

static STreeNode* Successor(STreeNode* n)           // in-order successor (null parent ends the walk)
{
    if (n->_M_right) { n = n->_M_right; while (n->_M_left) n = n->_M_left; return n; }
    STreeNode* p = n->_M_parent;
    while (p && n == p->_M_right) { n = p; p = p->_M_parent; }
    return p;
}

STreeIterator FindInTreeRangeTagged(STreeIterator first, STreeIterator last, const long& value, const SInputIteratorTag&)
{
    while (first._M_node != last._M_node && first._M_node->_M_value != value)
        first._M_node = Successor(first._M_node);
    return first;
}

STreeIterator FindInTreeRange(STreeIterator first, STreeIterator last, const long& value)
{
    return FindInTreeRangeTagged(first, last, value, SInputIteratorTag());
}

int main()
{
    // tree: 2 is the root, 1 left, 3 right; in-order 1, 2, 3
    STreeNode n1 = { 0, 0, 0, 1 }, n2 = { 0, 0, 0, 2 }, n3 = { 0, 0, 0, 3 };
    n2._M_left = &n1; n2._M_right = &n3; n1._M_parent = &n2; n3._M_parent = &n2;
    STreeIterator first(&n1), end(0);
    long three = 3, four = 4, one = 1;
    if (FindInTreeRange(first, end, three)._M_node != &n3) { std::printf("00535320_TEST FAIL\n"); return 1; }
    if (FindInTreeRange(first, end, one)._M_node != &n1) { std::printf("00535320_TEST FAIL\n"); return 1; }
    if (FindInTreeRange(first, end, four)._M_node != 0) { std::printf("00535320_TEST FAIL\n"); return 1; }
    std::printf("00535320_TEST PASS\n");
    return 0;
}
