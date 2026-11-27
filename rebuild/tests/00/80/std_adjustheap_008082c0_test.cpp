// Behaviour test for std::__adjust_heap (0x008082C0): STLport sift-down + push_heap.
// The retail comparator returns aValue > bValue, so the heap root is the smaller score.
#include <cstdio>
struct Greater { bool operator()(int a, int b) const { return a > b; } };
static void push_heap_model(int* first, int hole, int top, int value, Greater comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
static void adjust_heap_model(int* first, int hole, int len, int value, Greater comp)
{
    int top = hole;
    int second = 2 * hole + 2;
    while (second < len) {
        if (comp(*(first + second), *(first + (second - 1)))) second--;
        *(first + hole) = *(first + second);
        hole = second;
        second = 2 * (second + 1);
    }
    if (second == len) { *(first + hole) = *(first + (second - 1)); hole = second - 1; }
    push_heap_model(first, hole, top, value, comp);
}
static bool is_min_heap(const int* a, int n)
{
    for (int i = 1; i < n; ++i) if (a[(i - 1) / 2] > a[i]) return false;
    return true;
}
int main()
{
    int a[10] = { 3, 9, 1, 7, 5, 8, 2, 6, 4, 0 };
    for (int p = (10 - 2) / 2; ; --p) { adjust_heap_model(a, p, 10, a[p], Greater()); if (p == 0) break; }
    if (!is_min_heap(a, 10) || a[0] != 0) { std::printf("008082c0_TEST FAIL\n"); return 1; }
    int b[6] = { 1, 2, 3, 4, 5, 9 };       // replace the root; even length, the second == len tail is not taken
    adjust_heap_model(b, 0, 6, 8, Greater());
    if (!is_min_heap(b, 6) || b[0] != 2 || b[1] != 4 || b[3] != 8) { std::printf("008082c0_TEST FAIL\n"); return 1; }
    int c[2] = { 4, 1 };                   // second == len: take the only child, then push
    adjust_heap_model(c, 0, 2, 2, Greater());
    if (!is_min_heap(c, 2) || c[0] != 1 || c[1] != 2) { std::printf("008082c0_TEST FAIL\n"); return 1; }
    std::printf("008082c0_TEST PASS\n");
    return 0;
}