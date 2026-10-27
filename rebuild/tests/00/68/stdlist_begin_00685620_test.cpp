// Behaviour test for std::list::begin (0x00685620): the first element is the sentinel node's next.
#include <cstdio>
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { int data; };
struct Iter { NodeBase* node; };
struct List {
    NodeBase* sentinel;
    Iter begin() { Iter i; i.node = sentinel->next; return i; }
};
int main()
{
    Node head, a;
    List l; l.sentinel = &head;
    head.next = &head; head.prev = &head;
    if (l.begin().node != &head) { std::printf("00685620_TEST FAIL\n"); return 1; }
    head.next = &a; a.next = &head; a.prev = &head; head.prev = &a; a.data = 5;
    if (l.begin().node != &a || ((Node*)l.begin().node)->data != 5) { std::printf("00685620_TEST FAIL\n"); return 1; }
    std::printf("00685620_TEST PASS\n");
    return 0;
}