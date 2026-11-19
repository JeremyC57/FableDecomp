// behaviour test for 0x005E21F0 __pop_heap: *result = *first, then adjust the heap with the saved value (max-heap on the key)
#include <cstdio>
struct SPair { unsigned int first; int BankIndex, AnimName, GroupName; void* Components; };
struct Less { bool operator()(const SPair& a, const SPair& b) const { return a.first < b.first; } };
static void adjust_heap(SPair* first, int hole, int len, SPair value, Less comp)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) { if (comp(first[child], first[child - 1])) --child; first[hole] = first[child]; hole = child; child = 2 * child + 2; }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = value;
}
static void pop_heap_core(SPair* first, SPair* last, SPair* result, SPair value, Less comp)
{
    *result = *first;
    adjust_heap(first, 0, int(last - first), value, comp);
}
int main()
{
    SPair h[4] = { { 9, 1, 0, 0, 0 }, { 7, 2, 0, 0, 0 }, { 8, 3, 0, 0, 0 }, { 3, 4, 0, 0, 0 } };
    SPair v = h[3];
    pop_heap_core(h, h + 3, h + 3, v, Less());
    if (h[3].first != 9 || h[3].BankIndex != 1 || h[0].first != 8) { std::printf("005e21f0_TEST FAIL\n"); return 1; }
    std::printf("005e21f0_TEST PASS\n");
    return 0;
}