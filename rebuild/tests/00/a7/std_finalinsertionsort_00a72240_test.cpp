// behaviour test for 0x00A72240 std::__final_insertion_sort over pair<CCharString, C3DHelperPoint>: a range of more than 16 elements is insertion-sorted on its first 16 and finished by the unguarded insertion (which relies on the sorted prefix as its sentinel); a short range is insertion-sorted whole
#include <cstdio>
static int gInsertionCalls = 0, gUnguardedCalls = 0;
static void InsertionSort(int* first, int* last)              // stand-in for __insertion_sort
{
    ++gInsertionCalls;
    if (first == last) return;
    for (int* i = first + 1; i != last; ++i) {
        int v = *i; int* j = i;
        while (j != first && v < j[-1]) { *j = j[-1]; --j; }
        *j = v;
    }
}
static void UnguardedInsertionSort(int* first, int* last)     // stand-in for __unguarded_insertion_sort_aux: no lower bound test
{
    ++gUnguardedCalls;
    for (int* i = first; i != last; ++i) {
        int v = *i; int* j = i;
        while (v < j[-1]) { *j = j[-1]; --j; }
        *j = v;
    }
}
static void FinalInsertionSort(int* first, int* last)          // the function under test (STLport 4.6 shape)
{
    if (last - first > 16) {
        InsertionSort(first, first + 16);
        UnguardedInsertionSort(first + 16, last);
    }
    else
        InsertionSort(first, last);
}
int main()
{
    int fail = 0;
    int a[40];
    for (int i = 0; i < 40; ++i) a[i] = (i * 37 + 11) % 40;
    for (int i = 0; i < 16; ++i) { int m = i; for (int j = i + 1; j < 40; ++j) if (a[j] < a[m]) m = j; int t = a[i]; a[i] = a[m]; a[m] = t; }  // introsort leaves the minimum in the first 16
    FinalInsertionSort(a, a + 40);
    for (int i = 1; i < 40; ++i) if (a[i - 1] > a[i]) fail |= 1;
    if (gInsertionCalls != 1 || gUnguardedCalls != 1) fail |= 2;
    int b[16];
    for (int i = 0; i < 16; ++i) b[i] = 16 - i;
    FinalInsertionSort(b, b + 16);
    for (int i = 0; i < 16; ++i) if (b[i] != i + 1) fail |= 4;
    if (gInsertionCalls != 2 || gUnguardedCalls != 1) fail |= 8;
    if (fail) { std::printf("00a72240_TEST FAIL %d\n", fail); return 1; }
    std::printf("00a72240_TEST PASS\n");
    return 0;
}