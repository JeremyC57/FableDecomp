#include <cstdio>
// Behaviour test for 0x0065FA88: STLport find_if over a list of counted task pointers with an is-this-task predicate
// (input-iterator __find_if: the first element the predicate accepts, else last).
struct Node { Node* next; Node* prev; const void* task; };
struct Iter { Node* n; bool operator!=(const Iter& o) const { return n != o.n; } Iter& operator++() { n = n->next; return *this; } const void* operator*() const { return n->task; } };
struct IsTask { const void* t; bool operator()(const void* p) const { return p == t; } };
static Iter find_if_(Iter first, Iter last, IsTask pred) { while (first != last && !pred(*first)) ++first; return first; }
int main()
{
    int a, b, c, d;
    Node head, n1, n2, n3;
    head.next = &n1; n1.next = &n2; n2.next = &n3; n3.next = &head;
    n1.task = &a; n2.task = &b; n3.task = &c;
    Iter first = { &n1 }, last = { &head };
    IsTask pb = { &b }, pc = { &c }, pd = { &d };
    if (find_if_(first, last, pb).n != &n2 || find_if_(first, last, pc).n != &n3 || find_if_(first, last, pd).n != &head) {
        std::printf("0065fa88_TEST FAIL\n"); return 1;
    }
    Iter empty = { &head };
    if (find_if_(empty, last, pb).n != &head) { std::printf("0065fa88_TEST FAIL\n"); return 1; }
    std::printf("0065fa88_TEST PASS\n");
    return 0;
}
