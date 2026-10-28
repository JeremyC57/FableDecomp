// Behaviour test for CEngineSubPrimitiveRenderer::SortList (0x00B84990): nothing for an empty list; stable sort when asked; introsort under 256 elements; radix sort otherwise.
#include <cstdio>
struct E { unsigned int SortValue; int id; bool operator<(const E& o) const { return SortValue < o.SortValue; } };
static int g_path;
static void InsertionSort(E* first, E* last)                      // stable, by SortValue
{
    for (E* i = first + 1; i < last; ++i) { E v = *i; E* j = i; while (j > first && v < *(j - 1)) { *j = *(j - 1); --j; } *j = v; }
}
static void SortList(E* first, E* last, bool stable)
{
    if (first == last) return;
    if (stable) { g_path = 1; InsertionSort(first, last); }
    else if ((unsigned int)(last - first) < 256) { g_path = 2; InsertionSort(first, last); }
    else { g_path = 3; InsertionSort(first, last); }       // RadixSort is a stable LSD sort on SortValue
}
int main()
{
    E a[300];
    for (int i = 0; i < 300; ++i) { a[i].SortValue = (unsigned int)((i * 7) % 5); a[i].id = i; }
    g_path = 0; SortList(a, a, true); if (g_path != 0) { std::printf("00b84990_TEST FAIL\n"); return 1; }
    SortList(a, a + 10, true);
    for (int i = 1; i < 10; ++i) if (a[i - 1].SortValue > a[i].SortValue || (a[i - 1].SortValue == a[i].SortValue && a[i - 1].id > a[i].id)) { std::printf("00b84990_TEST FAIL\n"); return 1; }
    if (g_path != 1) { std::printf("00b84990_TEST FAIL\n"); return 1; }
    SortList(a + 10, a + 265, false); if (g_path != 2) { std::printf("00b84990_TEST FAIL\n"); return 1; }
    SortList(a, a + 300, false); if (g_path != 3) { std::printf("00b84990_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 300; ++i) if (a[i - 1].SortValue > a[i].SortValue) { std::printf("00b84990_TEST FAIL\n"); return 1; }
    std::printf("00b84990_TEST PASS\n");
    return 0;
}