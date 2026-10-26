// behaviour test for 0x0065EBE4 __find_if over list<CCountedPointer<CVillageTaskPickCrops > > with CIsTask: the first node whose task is the predicate's task is returned, last when none is
#include <cstdio>
struct Task { int id; };
struct Node { Node* next; Task* data; };
struct IsTask { Task* task; bool operator()(Task* p) const { return p == task; } };
static Node* FindIf(Node* first, Node* last, IsTask pred)      // the function under test
{
    while (first != last && !pred(first->data)) first = first->next;
    return first;
}
int main()
{
    int fail = 0;
    Task a, b, c; a.id = 1; b.id = 2; c.id = 3;
    Node head; Node n[3];
    n[0].next = &n[1]; n[1].next = &n[2]; n[2].next = &head; n[0].data = &a; n[1].data = &b; n[2].data = &b;
    IsTask p; p.task = &b;
    if (FindIf(&n[0], &head, p) != &n[1]) fail |= 1;
    p.task = &c;
    if (FindIf(&n[0], &head, p) != &head) fail |= 2;
    if (FindIf(&head, &head, p) != &head) fail |= 4;
    if (fail) { std::printf("0065ebe4_TEST FAIL %d\n", fail); return 1; }
    std::printf("0065ebe4_TEST PASS\n");
    return 0;
}