#include <cstdio>
// Behaviour test for 0092b260: list end() is an iterator on the sentinel itself.
struct Node { Node* next; Node* prev; };
struct It { Node* node; It(Node* n) : node(n) { } };
struct List { Node* node; It end() { return It(node); } };
int main()
{
    Node s, a; s.next = &a; a.next = &s;
    List l; l.node = &s;
    if (l.end().node != &s) { std::printf("0092b260_TEST FAIL\n"); return 1; }
    std::printf("0092b260_TEST PASS\n");
    return 0;
}