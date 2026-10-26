// behaviour test for 0x005fa41f std::sort<std::pair<long, long >*, CKeyPairCompareLess<long, long> >: an empty range does nothing; otherwise the
// introsort loop runs with depth limit 2*lg(n) over the 0x8-byte pairs, then the final insertion sort leaves the range ordered
#include <cstdio>
struct Pair8 {                                       // test-only pair of the retail size 0x8: a key and its value words
    int Key;
    int Value[1];
};
static int gLoopCalls = 0;
static int gLastDepth = -1;
static int gFinalCalls = 0;
static int Lg(int n)
{
    int k;
    for (k = 0; n != 1; n >>= 1) ++k;
    return k;
}
static void IntrosortLoop(Pair8*, Pair8*, int depth) { ++gLoopCalls; gLastDepth = depth; }
static void FinalInsertionSort(Pair8* first, Pair8* last)
{
    ++gFinalCalls;
    for (Pair8* i = first + 1; i < last; ++i) {
        Pair8 v = *i;
        Pair8* j = i;
        while (j != first && v.Key < (j - 1)->Key) { *j = *(j - 1); --j; }
        *j = v;
    }
}
static void Sort(Pair8* first, Pair8* last)
{
    if (first != last) {
        IntrosortLoop(first, last, Lg((int)(last - first)) * 2);
        FinalInsertionSort(first, last);
    }
}
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(Pair8) != 0x8u) fails |= 1;
    Pair8 v[20];
    for (int i = 0; i < 20; ++i) { v[i].Key = (i * 7) % 20; for (int k = 0; k < 1; ++k) v[i].Value[k] = v[i].Key * 10 + k; }
    Sort(v, v);
    if (gLoopCalls != 0 || gFinalCalls != 0) fails |= 2;
    Sort(v, v + 5);
    if (gLoopCalls != 1 || gLastDepth != 4 || gFinalCalls != 1) fails |= 4;
    Sort(v, v + 20);
    if (gLoopCalls != 2 || gLastDepth != 8 || gFinalCalls != 2) fails |= 8;
    for (int i = 0; i < 20; ++i) if (v[i].Key != i || v[i].Value[1 - 1] != i * 10 + 1 - 1) fails |= 16;
    Sort(v, v + 1);
    if (gLastDepth != 0) fails |= 32;
    if (fails) { std::printf("005fa41f_TEST FAIL %d\n", fails); return 1; }
    std::printf("005fa41f_TEST PASS\n");
    return 0;
}