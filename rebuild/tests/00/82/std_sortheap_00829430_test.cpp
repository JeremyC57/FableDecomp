// behaviour test for 0x00829430 std::sort_heap (model: a max-heap of keys sorted ascending)
#include <cstdio>
static void Sift(int* a, int hole, int len, int v)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) { if (a[child] < a[child - 1]) --child; a[hole] = a[child]; hole = child; child = 2 * child + 2; }
    if (child == len) { a[hole] = a[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && a[parent] < v) { a[hole] = a[parent]; hole = parent; parent = (hole - 1) / 2; }
    a[hole] = v;
}
static void PopHeap(int* first, int* last) { int v = *(last - 1); *(last - 1) = *first; Sift(first, 0, (int)(last - 1 - first), v); }
static void SortHeap(int* first, int* last) { while (last - first > 1) PopHeap(first, last--); }
int main()
{
    int a[6] = { 9, 7, 8, 3, 5, 1 };
    SortHeap(a, a + 6);
    int ok = 1; for (int i = 1; i < 6; ++i) if (a[i - 1] > a[i]) ok = 0;
    if (!ok) { std::printf("00829430_TEST FAIL\n"); return 1; }
    std::printf("00829430_TEST PASS\n");
    return 0;
}