#include <cstdio>
// Behaviour test for 0092b230: the list iterator constructor stores the node pointer.
struct Node { Node* next; Node* prev; };
struct It { Node* node; It(Node* n) : node(n) { } };
int main()
{
    Node n; It it(&n);
    if (it.node != &n) { std::printf("0092b230_TEST FAIL\n"); return 1; }
    std::printf("0092b230_TEST PASS\n");
    return 0;
}
