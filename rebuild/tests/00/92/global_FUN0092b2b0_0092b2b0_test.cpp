#include <cstdio>
// Behaviour test for 0092b2b0: list size() counts the nodes from begin() (sentinel->next) to end() (the sentinel).
struct Node { Node* next; Node* prev; };
struct List { Node* node; long size() const { long n = 0; for (Node* p = node->next; p != node; p = p->next) ++n; return n; } };
int main()
{
    Node s; s.next = &s;
    List l; l.node = &s;
    int ok = (l.size() == 0);
    Node a, b; s.next = &a; a.next = &b; b.next = &s;
    ok = ok && l.size() == 2;
    if (!ok) { std::printf("0092b2b0_TEST FAIL\n"); return 1; }
    std::printf("0092b2b0_TEST PASS\n");
    return 0;
}