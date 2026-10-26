// behaviour test for 0x00749050 std::__make_heap<const CThing**, CCompare_FurthestFromInitPos2D >: after the parents are sifted down from (len - 2) / 2 to 0,
// no child outranks its parent under the comparator; ranges shorter than two are left alone
#include <cstdio>
struct Comp { bool operator()(int a, int b) const { return a < b; } };
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
static void MakeHeap(int* first, int* last, Comp comp)        // the function under test
{
    if (last - first < 2) return;
    int len = (int)(last - first);
    int parent = (len - 2) / 2;
    for (;;) {
        Adjust(first, parent, len, first[parent], comp);
        if (parent == 0) return;
        parent--;
    }
}
static int IsHeap(const int* v, int n, Comp comp)
{
    for (int i = 1; i < n; ++i) if (comp(v[(i - 1) / 2], v[i])) return 0;
    return 1;
}
int main()
{
    int fail = 0;
    int v[10] = { 5, 1, 8, 3, 9, 2, 7, 4, 6, 0 };
    MakeHeap(v, v + 10, Comp());
    if (!IsHeap(v, 10, Comp())) fail |= 1;
    int sum = 0; for (int i = 0; i < 10; ++i) sum += v[i];
    if (sum != 45) fail |= 2;
    int one[1] = { 42 }; MakeHeap(one, one + 1, Comp()); if (one[0] != 42) fail |= 4;
    int two[2] = { 1, 2 }; MakeHeap(two, two + 2, Comp()); if (!IsHeap(two, 2, Comp())) fail |= 8;
    if (fail) { std::printf("00749050_TEST FAIL %d\n", fail); return 1; }
    std::printf("00749050_TEST PASS\n");
    return 0;
}