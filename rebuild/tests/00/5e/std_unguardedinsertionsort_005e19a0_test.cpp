// behaviour test for 0x005E19A0: the unguarded insertion sort orders a run whose first element is already a lower bound
#include <cstdio>
#include <string>
static void LinearInsert(std::string* last, std::string val) { std::string* next = last; --next; while (val < *next) { *last = *next; last = next; --next; } *last = val; }
static void UnguardedSort(std::string* first, std::string* last) { for (std::string* i = first; i != last; ++i) LinearInsert(i, std::string(*i)); }
int main()
{
    std::string v[5] = { "a", "d", "b", "e", "c" };
    UnguardedSort(v + 1, v + 5);
    if (v[0] != "a" || v[1] != "b" || v[2] != "c" || v[3] != "d" || v[4] != "e") { std::printf("005e19a0_TEST FAIL\n"); return 1; }
    std::printf("005e19a0_TEST PASS\n");
    return 0;
}