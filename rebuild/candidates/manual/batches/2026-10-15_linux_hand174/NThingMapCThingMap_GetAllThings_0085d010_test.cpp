#include <cstdio>
struct Node { int thing; Node* next; };
struct Cell { Node* first; };
struct Set { int items[16]; int n;
    void insert(int v) { for (int i = 0; i < n; ++i) if (items[i] == v) return; items[n++] = v; } };
static long g_levels = 2;
static void GetAll(Cell** cells, unsigned long* counts, Set* out)
{
    for (long level = 0; level < g_levels; ++level)
        for (unsigned long i = 0; i < counts[level]; ++i)
            for (Node* n = cells[level][i].first; n; n = n->next) out->insert(n->thing);
}
int main()
{
    Node n3 = { 3, 0 }, n2 = { 2, &n3 }, n1 = { 1, 0 }, n4 = { 2, 0 };
    Cell l0[2] = { { &n2 }, { 0 } }, l1[1] = { { &n1 } };
    Cell l2[1] = { { &n4 } };
    Cell* cells[3] = { l0, l1, l2 };
    unsigned long counts[3] = { 2, 1, 1 };
    Set s; s.n = 0;
    GetAll(cells, counts, &s);
    bool ok = s.n == 3;
    if (!ok) { std::printf("0085d010_TEST FAIL\n"); return 1; }
    std::printf("0085d010_TEST PASS\n");
    return 0;
}
