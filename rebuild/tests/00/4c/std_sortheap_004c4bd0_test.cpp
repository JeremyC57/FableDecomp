// behaviour test for 0x004C4BD0: sort_heap turns a max-heap of records into ascending order
#include <cstdio>
struct E { long k; long a; long b; };
static void Adjust(E* f, int hole, int len, E val)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) { if (f[child].k < f[child - 1].k) --child; f[hole] = f[child]; hole = child; child = 2 * child + 2; }
    if (child == len) { f[hole] = f[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && f[parent].k < val.k) { f[hole] = f[parent]; hole = parent; parent = (hole - 1) / 2; }
    f[hole] = val;
}
static void SortHeap(E* first, E* last) { while (last - first > 1) { --last; E val = *last; *last = *first; Adjust(first, 0, (int)(last - first), val); } }
int main()
{
    E v[5] = { { 9, 0, 0 }, { 7, 1, 0 }, { 8, 2, 0 }, { 1, 3, 0 }, { 5, 4, 0 } };
    SortHeap(v, v + 5);
    int ok = v[0].k == 1 && v[1].k == 5 && v[2].k == 7 && v[3].k == 8 && v[4].k == 9;
    if (!ok) { std::printf("004c4bd0_TEST FAIL\n"); return 1; }
    std::printf("004c4bd0_TEST PASS\n");
    return 0;
}