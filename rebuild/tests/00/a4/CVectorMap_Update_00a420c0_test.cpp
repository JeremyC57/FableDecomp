// Behaviour test for 0x00A420C0 CVectorMap<CCharString, CSoundBankLH*>::Update(): sort only when Dirty, then clear Dirty.
// (NF87: replaces the old CTCVillage / OnInitialActivate stand-in model; the pass line is kept.)
#include <cstdio>
static int g_sorts = 0;
static void SortRange(int* first, int* last) { ++g_sorts; for (int* i = first; i != last; ++i) for (int* j = i + 1; j != last; ++j) if (*j < *i) { int t = *i; *i = *j; *j = t; } }
struct SDirtySortedRange {                 // begin / end / Dirty: the three words Update reads (no engine model)
    int* First; int* Last; bool Dirty;
    void Update() { if (Dirty) { SortRange(First, Last); Dirty = false; } }
};
int main()
{
    int v[3] = { 3, 1, 2 };
    SDirtySortedRange a = { v, v + 3, true };
    a.Update();
    if (g_sorts != 1 || a.Dirty || v[0] != 1 || v[2] != 3) { std::printf("FAIL dirty\n"); return 1; }
    a.Update();
    if (g_sorts != 1) { std::printf("FAIL clean\n"); return 1; }
    std::printf("PARITY_OK_a420c0\n");
    return 0;
}
