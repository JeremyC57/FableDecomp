// Behaviour test for std::_List_iterator::operator++ (0x00685200): prefix increment follows the node's next pointer and returns *this.
#include <cstdio>
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { int data; };
struct Iter {
    NodeBase* node;
    Iter& operator++() { node = node->next; return *this; }
};
int main()
{
    Node a, b, c;
    a.next = &b; b.next = &c; c.next = &a;
    a.data = 1; b.data = 2; c.data = 3;
    Iter it; it.node = &a;
    Iter& r = ++it;
    if (&r != &it || it.node != &b) { std::printf("00685200_TEST FAIL\n"); return 1; }
    ++(++it);
    if (it.node != &a || ((Node*)it.node)->data != 1) { std::printf("00685200_TEST FAIL\n"); return 1; }
    std::printf("00685200_TEST PASS\n");
    return 0;
}