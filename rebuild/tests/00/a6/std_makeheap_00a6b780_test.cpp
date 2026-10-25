// behaviour test for 0x00A6B780 __make_heap over std::pair<unsigned long, unsigned long> (std::less<std::pair<unsigned long, unsigned long> >): every parent from (len - 2) / 2 down to 0 is sifted once with a copy of its own element; ranges shorter than 2 are left alone; the result is a valid heap under the comparator (top = largest)
#include <cstdio>
struct P { int first; int second; };
static int gCalls = 0;
static bool Comp(const P& a, const P& b) { return a.first < b.first || (!(b.first < a.first) && a.second < b.second); }
static void AdjustHeap(P* first, int hole, int len, P value)   // STLport __adjust_heap
{
    int top = hole;
    int child = 2 * hole + 2;
    ++gCalls;
    while (child < len) {
        if (Comp(first[child], first[child - 1])) child--;
        first[hole] = first[child];
        hole = child;
        child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && Comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
static void MakeHeap(P* first, P* last)
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        P v = first[parent];
        AdjustHeap(first, parent, len, v);
        if (parent == 0) return;
        parent--;
    }
}
int main()
{
    int fails = 0;
    P a[9] = { {5,0}, {1,1}, {9,2}, {3,3}, {7,4}, {2,5}, {8,6}, {4,7}, {6,8} };
    MakeHeap(a, a + 9);
    if (gCalls != 4) fails |= 1;                                 // parents 3, 2, 1, 0
    for (int i = 1; i < 9; ++i) if (Comp(a[(i - 1) / 2], a[i])) fails |= 2;   // heap property
    int sum = 0; for (int i = 0; i < 9; ++i) sum += a[i].first;
    if (sum != 45) fails |= 4;                                   // a permutation
    gCalls = 0;
    P b[1] = { {3,0} };
    MakeHeap(b, b + 1);
    MakeHeap(b, b);
    if (gCalls != 0 || b[0].first != 3) fails |= 8;
    P c[2] = { {1,0}, {2,1} };
    MakeHeap(c, c + 2);
    if (gCalls != 1 || c[0].first != 2) fails |= 16;
    if (fails) { std::printf("00a6b780_TEST FAIL %d\n", fails); return 1; }
    std::printf("00a6b780_TEST PASS\n");
    return 0;
}