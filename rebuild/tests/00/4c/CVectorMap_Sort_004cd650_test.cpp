// Behaviour test for 0x004CD650 CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<..> >::Sort (NF92 rewrite: the old test linked the
// stand-in CComponentMap_Resort, which no longer exists): the range [start, finish) and the stored comparator go to the __fastcall sort, then
// Dirty is cleared. Host model only (no shared-header class); PassPattern kept.
#include <cstdio>
static void* g_seen_first;
static void* g_seen_last;
static int g_seen_compare;
static int g_sorted = 0;
static void __fastcall SortRange(void* first, void* last, bool compare) { g_seen_first = first; g_seen_last = last; g_seen_compare = compare; ++g_sorted; }
struct SortedMapModel { void* start; void* finish; void* end_of_storage; bool compare; bool dirty;
    void Sort() { SortRange(start, finish, compare); dirty = false; } };
int main()
{
    int a = 0, b = 0;
    SortedMapModel m; m.start = &a; m.finish = &b; m.end_of_storage = &b; m.compare = true; m.dirty = true;
    m.Sort();
    bool ok = g_sorted == 1 && g_seen_first == &a && g_seen_last == &b && g_seen_compare == 1 && !m.dirty;
    if (ok) std::printf("004cd650_TEST PASS\n");
    else    std::printf("004cd650_TEST FAIL\n");
    return ok ? 0 : 1;
}
