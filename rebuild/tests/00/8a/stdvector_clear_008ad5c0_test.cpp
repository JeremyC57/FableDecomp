#include <cstdio>
// Behaviour test for std::vector<CScriptThing>::clear() @ 0x008AD5C0 (NF48: restated; was the CPair_Retail::CallWithPair stand-in test):
// clear() hands erase the vector's own [begin, end) range on the same object.
static void* g_self = 0; static void* g_first = 0; static void* g_last = 0;
struct V {
    int* start; int* finish; int* storage;
    int* erase(int* first, int* last) { g_self = this; g_first = first; g_last = last; finish = first; return first; }
    void clear() { erase(start, finish); }
};
int main()
{
    int items[3] = { 1, 2, 3 };
    V v; v.start = items; v.finish = items + 3; v.storage = items + 3;
    v.clear();
    if (g_self != &v || g_first != items || g_last != items + 3 || v.finish != items) { std::printf("008ad5c0_TEST FAIL\n"); return 1; }
    std::printf("008ad5c0_TEST PASS\n");
    return 0;
}
