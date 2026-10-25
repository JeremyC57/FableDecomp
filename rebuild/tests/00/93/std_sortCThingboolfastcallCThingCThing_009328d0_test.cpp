#include <cstdio>
// Behaviour test for 0x009328D0 std::sort<CThing**, bool (__fastcall*)(CThing*, CThing*)>: links against the source
// object's instantiation; the three out-of-line STLport callees are explicit specialisations that record their calls.
class CThing { public: int Z; };
typedef bool (__fastcall* ThingCompare)(CThing* a, CThing* b);
static int g_introDepth = -1, g_introCalls = 0, g_insCalls = 0, g_ungCalls = 0;
static CThing** g_insFirst; static CThing** g_insLast; static CThing** g_ungFirst; static CThing** g_ungLast;
static ThingCompare g_seenComp;
namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);
template <class _RandomAccessIter, class _Tp, class _Size, class _Compare>
void __fastcall __introsort_loop(_RandomAccessIter, _RandomAccessIter, _Tp*, _Size, _Compare);
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter, _RandomAccessIter, _Compare);
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter, _RandomAccessIter, _Tp*, _Compare);
template <> void __fastcall __introsort_loop<CThing**, CThing*, int, ThingCompare>(CThing**, CThing**, CThing**, int d, ThingCompare c)
{ ++g_introCalls; g_introDepth = d; g_seenComp = c; }
template <> void __fastcall __insertion_sort<CThing**, ThingCompare>(CThing** f, CThing** l, ThingCompare c)
{ ++g_insCalls; g_insFirst = f; g_insLast = l; g_seenComp = c; }
template <> void __fastcall __unguarded_insertion_sort_aux<CThing**, CThing*, ThingCompare>(CThing** f, CThing** l, CThing**, ThingCompare c)
{ ++g_ungCalls; g_ungFirst = f; g_ungLast = l; g_seenComp = c; }
}
static bool __fastcall Lower(CThing* a, CThing* b) { return a->Z < b->Z; }
static void Reset() { g_introDepth = -1; g_introCalls = g_insCalls = g_ungCalls = 0; g_insFirst = g_insLast = g_ungFirst = g_ungLast = 0; g_seenComp = 0; }
int main()
{
    CThing* a[40];
    Reset(); std::sort<CThing**, ThingCompare>(a, a, &Lower);                   // empty: nothing
    if (g_introCalls || g_insCalls || g_ungCalls) { std::printf("009328d0_TEST FAIL empty\n"); return 1; }
    Reset(); std::sort<CThing**, ThingCompare>(a, a + 40, &Lower);              // 40: depth 2*lg(40) = 10, split at 16
    if (g_introCalls != 1 || g_introDepth != 10 || g_insCalls != 1 || g_insFirst != a || g_insLast != a + 16 ||
        g_ungCalls != 1 || g_ungFirst != a + 16 || g_ungLast != a + 40 || g_seenComp != &Lower) { std::printf("009328d0_TEST FAIL long\n"); return 1; }
    Reset(); std::sort<CThing**, ThingCompare>(a, a + 16, &Lower);              // 16: one insertion sort over all
    if (g_introCalls != 1 || g_introDepth != 8 || g_insCalls != 1 || g_insLast != a + 16 || g_ungCalls != 0) { std::printf("009328d0_TEST FAIL short\n"); return 1; }
    Reset(); std::sort<CThing**, ThingCompare>(a, a + 1, &Lower);               // 1: depth 0
    if (g_introDepth != 0 || g_insCalls != 1) { std::printf("009328d0_TEST FAIL one\n"); return 1; }
    std::printf("009328d0_TEST PASS\n");
    return 0;
}
