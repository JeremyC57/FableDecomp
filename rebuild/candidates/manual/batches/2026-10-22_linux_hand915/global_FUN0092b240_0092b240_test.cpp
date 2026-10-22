#include <cstdio>
// Behaviour test for 0092b240: the list iterator pre-increment steps to the next node and returns *this.
struct Node { Node* next; Node* prev; };
struct It { Node* node; It& operator++() { node = node->next; return *this; } };
int main()
{
    Node a, b; a.next = &b; b.next = &a;
    It it; it.node = &a;
    It& r = ++it;
    int ok = (&r == &it && it.node == &b);
    ++it; ok = ok && it.node == &a;
    if (!ok) { std::printf("0092b240_TEST FAIL\n"); return 1; }
    std::printf("0092b240_TEST PASS\n");
    return 0;
}
