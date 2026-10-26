// behaviour test for 0x0065FB72 for_each over list<CCountedPointer<CVillageTaskSecuritySweep > >: the functor sees every node's value in order and comes back with its final state
#include <cstdio>
struct Node { Node* next; int value; };
struct Functor { int calls; int last; void operator()(const int& v) { ++calls; last = v; } };
static Functor ForEach(Node* first, Node* last, Functor f)     // the function under test
{
    for (; first != last; first = first->next) f(first->value);
    return f;
}
int main()
{
    int fail = 0;
    Node head; Node n[3];
    n[0].next = &n[1]; n[1].next = &n[2]; n[2].next = &head; n[0].value = 4; n[1].value = 5; n[2].value = 6;
    Functor f; f.calls = 0; f.last = 0;
    Functor r = ForEach(&n[0], &head, f);
    if (r.calls != 3 || r.last != 6) fail |= 1;
    if (f.calls != 0) fail |= 2;
    Functor e = ForEach(&head, &head, f);
    if (e.calls != 0) fail |= 4;
    if (fail) { std::printf("0065fb72_TEST FAIL %d\n", fail); return 1; }
    std::printf("0065fb72_TEST PASS\n");
    return 0;
}