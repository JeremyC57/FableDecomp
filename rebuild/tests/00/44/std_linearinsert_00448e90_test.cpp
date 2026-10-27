// behaviour test for 0x00448E90 std::__linear_insert over keyed counted-pointer pairs (model over int keys)
#include <cstdio>
struct P { int key; int val; };
static P MakeP(int k, int v) { P p; p.key = k; p.val = v; return p; }
static void Unguarded(P* last, P v) { P* next = last - 1; while (v.key < next->key) { *last = *next; last = next; --next; } *last = v; }
static void LinearInsert(P* first, P* last, P v)
{
    if (v.key < first->key) { for (P* s = last; s != first; --s) *s = *(s - 1); *first = v; }
    else Unguarded(last, v);
}
int main()
{
    P a[4] = { { 2, 20 }, { 5, 50 }, { 7, 70 }, { 0, 0 } };
    LinearInsert(a, a + 3, MakeP(1, 10));
    int ok = a[0].key == 1 && a[1].key == 2 && a[3].key == 7;
    P b[4] = { { 2, 20 }, { 5, 50 }, { 7, 70 }, { 0, 0 } };
    LinearInsert(b, b + 3, MakeP(6, 60));
    ok = ok && b[2].key == 6 && b[3].key == 7 && b[1].key == 5;
    if (!ok) { std::printf("00448e90_TEST FAIL\n"); return 1; }
    std::printf("00448e90_TEST PASS\n");
    return 0;
}