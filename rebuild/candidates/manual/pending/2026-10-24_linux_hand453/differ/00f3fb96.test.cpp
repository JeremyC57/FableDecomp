#include <cstdio>
// Behaviour test for 00f3fb96: __ehvec_ctor runs the constructor once per element, in order, stepping by size; with
// no exception the __finally does not unwind (the success flag is set).
static int g_order[8];
static int g_calls = 0;
static int g_unwinds = 0;
struct Elem { int id; void ctor() { id = 100 + g_calls; g_order[g_calls] = (int)(this - (Elem*)0); ++g_calls; } };
static void array_unwind(void*, unsigned int, int) { ++g_unwinds; }
static void ehvec_ctor(void* ptr, unsigned int size, int count)
{
    int i;
    int success = 0;
    for (i = 0; i < count; i++) {
        ((Elem*)ptr)->ctor();
        ptr = (char*)ptr + size;
    }
    success = 1;
    if (!success)
        array_unwind(ptr, size, i);
}
int main()
{
    Elem a[4];
    ehvec_ctor(a, sizeof(Elem), 4);
    int ok = g_calls == 4 && a[0].id == 100 && a[3].id == 103 && g_unwinds == 0;
    ehvec_ctor(a, sizeof(Elem), 0);
    ok = ok && g_calls == 4;
    if (!ok) { std::printf("00f3fb96_TEST FAIL\n"); return 1; }
    std::printf("00f3fb96_TEST PASS\n");
    return 0;
}
