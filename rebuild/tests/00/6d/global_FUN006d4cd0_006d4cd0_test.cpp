#include <cstdio>
// Behaviour test for 006d4cd0: STLport list empty() -- the sentinel's next pointer points back at it.
struct Node { Node* next; Node* prev; };
struct List { Node* node; bool Empty() const { return node->next == node; } };
int main()
{
    Node sentinel; sentinel.next = &sentinel; sentinel.prev = &sentinel;
    List l; l.node = &sentinel;
    int ok = l.Empty();
    Node n; n.next = &sentinel; n.prev = &sentinel; sentinel.next = &n; sentinel.prev = &n;
    ok = ok && !l.Empty();
    if (!ok) { std::printf("006d4cd0_TEST FAIL\n"); return 1; }
    std::printf("006d4cd0_TEST PASS\n");
    return 0;
}