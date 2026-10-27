// Behaviour test for 0x0076DAE0: list iterator pre-decrement walks to the previous node and returns itself
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SNode { SNode* Next; SNode* Prev; int V; };
struct SIter { SNode* N; SIter& operator--() { N = N->Prev; return *this; } };
int main()
{
    SNode a, b, c; a.V = 1; b.V = 2; c.V = 3;
    a.Next = &b; b.Next = &c; c.Next = &a; a.Prev = &c; b.Prev = &a; c.Prev = &b;
    SIter it; it.N = &c;
    SIter& r = --it; CHECK(&r == &it && it.N->V == 2);
    --(--it); CHECK(it.N->V == 3);
    if (!g_fail) printf("0076dae0_TEST PASS\n");
    return g_fail;
}