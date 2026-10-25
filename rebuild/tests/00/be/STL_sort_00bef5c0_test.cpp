// Behaviour test for 0x00BEF5C0 std::sort over the CVectorMap<long, CCompositeSurfaceInfo> entries: a non-empty range
// runs the introsort loop with depth 2*lg(n), then __insertion_sort on the first 16 and the unguarded aux on the rest
// (or one __insertion_sort for short ranges); an empty range does nothing.
#include <cstdio>
static int g_i = 0, g_f = 0;
struct CSurfaceEntry { unsigned char b[16]; };
struct CSurfaceEntryLess { void* c; };
template <class S> inline S __lg(S n) { S k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
static void __fastcall IntrosortLoop(CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntry*, int d, CSurfaceEntryLess) { g_i += d; }
static void __fastcall InsertionSort(CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntryLess) { ++g_f; }
static void __fastcall UnguardedInsertionSortAux(CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntry*, CSurfaceEntryLess) { g_f += 10; }
static void FinalInsertionSort(CSurfaceEntry* f, CSurfaceEntry* l, CSurfaceEntryLess c)
{
    if (l - f > 16) { InsertionSort(f, f + 16, c); UnguardedInsertionSortAux(f + 16, l, (CSurfaceEntry*)0, c); }
    else InsertionSort(f, l, c);
}
static void __fastcall Sort(CSurfaceEntry* f, CSurfaceEntry* l, CSurfaceEntryLess c)
{
    if (f != l) { IntrosortLoop(f, l, (CSurfaceEntry*)0, __lg(l - f) * 2, c); FinalInsertionSort(f, l, c); }
}
int main()
{
    CSurfaceEntry a[20]; CSurfaceEntryLess c = { 0 };
    Sort(a, a + 20, c); if (g_i != 8 || g_f != 11) { std::printf("00bef5c0_TEST FAIL\n"); return 1; }
    Sort(a, a + 3, c); if (g_i != 10 || g_f != 12) { std::printf("00bef5c0_TEST FAIL\n"); return 1; }
    Sort(a, a, c); if (g_i != 10 || g_f != 12) { std::printf("00bef5c0_TEST FAIL\n"); return 1; }
    std::printf("00bef5c0_TEST PASS\n");
    return 0;
}
