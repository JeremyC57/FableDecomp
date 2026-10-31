// Behaviour test for std::__adjust_heap (0x0097AF70): STLport sift-down + push_heap, max-heap under a "less" comparator.
#include <cstdio>
struct Less { bool operator()(int a, int b) const { return a < b; } };
static void push_heap_model(int* first, int hole, int top, int value, Less comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
static void adjust_heap_model(int* first, int hole, int len, int value, Less comp)
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
static bool is_heap(const int* a, int n) { for (int i = 1; i < n; ++i) if (a[(i - 1) / 2] < a[i]) return false; return true; }
int main()
{
    int a[10] = { 3, 9, 1, 7, 5, 8, 2, 6, 4, 0 };
    for (int p = (10 - 2) / 2; ; --p) { adjust_heap_model(a, p, 10, a[p], Less()); if (p == 0) break; }
    if (!is_heap(a, 10) || a[0] != 9) { std::printf("0097af70_TEST FAIL\n"); return 1; }
    int b[6] = { 9, 8, 7, 6, 5, 4 };       // replace the root, re-heapify (even length: the second == len tail)
    adjust_heap_model(b, 0, 6, 1, Less());
    if (!is_heap(b, 6) || b[0] != 8) { std::printf("0097af70_TEST FAIL\n"); return 1; }
    std::printf("0097af70_TEST PASS\n");
    return 0;
}