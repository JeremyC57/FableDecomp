#include <cstdio>
// Behaviour test for 0092b290: __distance counts the steps between two list iterators.
struct Node { Node* next; Node* prev; };
struct It { Node* node; It(Node* n) : node(n) { }
    bool operator!=(const It& o) const { return node != o.node; } It& operator++() { node = node->next; return *this; } };
long Distance(const It& first, const It& last) { long n = 0; It it(first); while (it != last) { ++it; ++n; } return n; }
int main()
{
    Node s, a, b, c; s.next = &a; a.next = &b; b.next = &c; c.next = &s;
    int ok = (Distance(It(&a), It(&s)) == 3 && Distance(It(&s), It(&s)) == 0 && Distance(It(&b), It(&c)) == 1);
    if (!ok) { std::printf("0092b290_TEST FAIL\n"); return 1; }
    std::printf("0092b290_TEST PASS\n");
    return 0;
}
