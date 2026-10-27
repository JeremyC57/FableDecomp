// behaviour test for 0x009B5A80: sort_heap pops the maximum to the back until the range is sorted
#include <cstdio>
#include <string>
static void Adjust(std::string* f, int hole, int len, std::string val)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) { if (f[child] < f[child - 1]) --child; f[hole] = f[child]; hole = child; child = 2 * child + 2; }
    if (child == len) { f[hole] = f[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && f[parent] < val) { f[hole] = f[parent]; hole = parent; parent = (hole - 1) / 2; }
    f[hole] = val;
}
static void SortHeap(std::string* first, std::string* last)
{
    while (last - first > 1) { --last; std::string val(*last); *last = *first; Adjust(first, 0, (int)(last - first), val); }
}
int main()
{
    std::string v[5] = { "e", "d", "c", "a", "b" };   // a max-heap
    SortHeap(v, v + 5);
    if (v[0] != "a" || v[1] != "b" || v[2] != "c" || v[3] != "d" || v[4] != "e") { std::printf("009b5a80_TEST FAIL\n"); return 1; }
    std::printf("009b5a80_TEST PASS\n");
    return 0;
}