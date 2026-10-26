// behaviour test for 0x0087E3B0 std::sort_heap<const CAnimationEntry**, CTurningAnimCompare >: a heap built under the comparator comes out sorted by it
// (each pass moves the root to the back and sifts the old back element down from the root)
#include <cstdio>
struct Comp { bool operator()(int a, int b) const { return a > b; } };
static void Push(int* first, int hole, int top, int val, Comp comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void Adjust(int* first, int hole, int len, int val, Comp comp)
{
    int top = hole, second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1])) second--;
        first[hole] = first[second]; hole = second; second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    Push(first, hole, top, val, comp);
}
static void SortHeap(int* first, int* last, Comp comp)        // the function under test
{
    while (last - first > 1) {
        int val = *(last - 1);
        *(last - 1) = *first;
        --last;
        Adjust(first, 0, (int)(last - first), val, comp);
    }
}
int main()
{
    int fail = 0;
    int v[9] = { 5, 1, 8, 3, 9, 2, 7, 4, 6 };
    for (int i = 1; i < 9; ++i) Push(v, i, 0, v[i], Comp());     // make_heap by repeated push
    SortHeap(v, v + 9, Comp());
    for (int i = 0; i + 1 < 9; ++i) if (Comp()(v[i + 1], v[i])) fail |= 1;
    int sum = 0; for (int i = 0; i < 9; ++i) sum += v[i];
    if (sum != 45) fail |= 2;
    int one[1] = { 42 }; SortHeap(one, one + 1, Comp()); if (one[0] != 42) fail |= 4;
    int two[2] = { 3, 3 }; SortHeap(two, two + 2, Comp()); if (two[0] != 3 || two[1] != 3) fail |= 8;
    if (fail) { std::printf("0087e3b0_TEST FAIL %d\n", fail); return 1; }
    std::printf("0087e3b0_TEST PASS\n");
    return 0;
}