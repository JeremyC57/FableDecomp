// Behaviour test for 0x0077F4C0 CVectorMap<CCharString, C3DVector>::Sort(): sort the range, then clear Dirty.
// (NF87: replaces the old CGuiVarTransferBool stand-in model; the pass line is kept.)
#include <cstdio>
static int g_sorts = 0;
static void SortRange(int* first, int* last) { ++g_sorts; for (int* i = first; i != last; ++i) for (int* j = i + 1; j != last; ++j) if (*j < *i) { int t = *i; *i = *j; *j = t; } }
struct SDirtySortedRange {                 // begin / end / Dirty: the words Sort reads and writes (no engine model)
    int* First; int* Last; bool Dirty;
    void Sort() { SortRange(First, Last); Dirty = false; }
};
int main()
{
    int v[3] = { 2, 3, 1 };
    SDirtySortedRange a = { v, v + 3, true };
    a.Sort();
    bool ok = g_sorts == 1 && !a.Dirty && v[0] == 1 && v[1] == 2 && v[2] == 3;
    std::printf(ok ? "0077f4c0_TEST PASS\n" : "0077f4c0_TEST FAIL\n");
    return ok ? 0 : 1;
}
