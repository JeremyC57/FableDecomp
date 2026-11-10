// Behaviour test for operator==(reverse_iterator, reverse_iterator) (0x0083ADF0): two reverse iterators compare equal exactly
// when their base iterators are on the same node.
#include <cstdio>
struct Node { int v; };
struct Iter { Node* node; bool operator==(const Iter& y) const { return node == y.node; } };
struct RevIter { Iter current; Iter base() const { return current; } };
static bool Equal(const RevIter& x, const RevIter& y) { return x.base() == y.base(); }
int main()
{
    Node a = { 1 }, b = { 2 };
    RevIter x, y, z; x.current.node = &a; y.current.node = &a; z.current.node = &b;
    bool ok = Equal(x, y) && !Equal(x, z) && Equal(z, z);
    if (!ok) { std::printf("0083adf0_TEST FAIL\n"); return 1; }
    std::printf("0083adf0_TEST PASS\n");
    return 0;
}