#include <cstdio>
// Behaviour test for 0092b250: list begin() is an iterator on the sentinel's next node.
struct Node { Node* next; Node* prev; };
struct It { Node* node; It(Node* n) : node(n) { } };
struct List { Node* node; It begin() { return It(node->next); } };
int main()
{
    Node s, a; s.next = &a; a.next = &s;
    List l; l.node = &s;
    int ok = (l.begin().node == &a);
    s.next = &s; ok = ok && l.begin().node == &s;
    if (!ok) { std::printf("0092b250_TEST FAIL\n"); return 1; }
    std::printf("0092b250_TEST PASS\n");
    return 0;
}