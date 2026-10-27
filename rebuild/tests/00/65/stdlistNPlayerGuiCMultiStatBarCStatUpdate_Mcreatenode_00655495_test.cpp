// behaviour test for 0x00655495: the node holds a copy of the record
#include <cstdio>
#include <cstdlib>
#include <new>
struct R { unsigned long m[3]; };
struct Node { Node* next; Node* prev; R data; };
static Node* Create(const R& x) { Node* p = (Node*)std::malloc(sizeof(Node)); new (&p->data) R(x); return p; }
int main()
{
    R r = { { 1, 2, 3 } }; Node* n = Create(r);
    bool ok = sizeof(Node) >= 3 * sizeof(unsigned long) && n->data.m[0] == 1 && n->data.m[2] == 3; std::free(n);
    if (!ok) { std::printf("00655495_TEST FAIL\n"); return 1; }
    std::printf("00655495_TEST PASS\n");
    return 0;
}