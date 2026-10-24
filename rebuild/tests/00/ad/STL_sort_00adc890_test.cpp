// Behaviour test for 0x00ADC890 sort<CBlockInfo*> (introsort loop + final insertion sort, ordered by Cost).
#include <cstdio>
#include <cstdlib>
#include <algorithm>

struct CBlockInfo { unsigned long BlockIndex; long Cost; long Benefit; bool operator<(const CBlockInfo& o) const { return Cost < o.Cost; } };
static int g_introDepth = -1, g_introCalls = 0, g_finalCalls = 0;
// stand-ins for the out-of-line halves 0x00ADC650 / 0x00ADC080
static void IntrosortLoop(CBlockInfo* first, CBlockInfo* last, CBlockInfo*, int depth) { ++g_introCalls; g_introDepth = depth; std::sort(first, last); }
static void FinalInsertionSort(CBlockInfo* first, CBlockInfo* last) { ++g_finalCalls; for (CBlockInfo* i = first + 1; i < last; ++i) if (*i < *(i - 1)) std::abort(); }
static int Lg(int n) { int k; for (k = 0; n != 1; n >>= 1) ++k; return k; }
// reconstructed body
static void Sort(CBlockInfo* first, CBlockInfo* last)
{
    if (first != last) {
        IntrosortLoop(first, last, (CBlockInfo*)0, Lg((int)(last - first)) * 2);
        FinalInsertionSort(first, last);
    }
}

int main()
{
    CBlockInfo a[37];
    std::srand(890);
    for (int i = 0; i < 37; ++i) { a[i].BlockIndex = i; a[i].Cost = std::rand() % 50 - 25; a[i].Benefit = 0; }
    Sort(a, a + 37);
    if (g_introCalls != 1 || g_finalCalls != 1 || g_introDepth != 2 * 5) { std::printf("00adc890_TEST FAIL\n"); return 1; }   // lg(37) = 5
    for (int i = 1; i < 37; ++i) if (a[i].Cost < a[i - 1].Cost) { std::printf("00adc890_TEST FAIL\n"); return 1; }
    Sort(a, a);                                                    // empty range: nothing called
    if (g_introCalls != 1 || g_finalCalls != 1) { std::printf("00adc890_TEST FAIL\n"); return 1; }
    Sort(a, a + 1);                                                // one element: depth 0
    if (g_introCalls != 2 || g_introDepth != 0) { std::printf("00adc890_TEST FAIL\n"); return 1; }
    std::printf("00adc890_TEST PASS\n");
    return 0;
}