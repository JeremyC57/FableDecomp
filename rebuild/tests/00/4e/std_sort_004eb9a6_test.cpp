// behaviour test for 0x004eb9a6 std::sort<std::pair<CCharString, CTCBase::CRegisteredTCInfo >*, CKeyPairCompareLess<CCharString, CTCBase::CRegisteredTCInfo> >: an empty range does nothing; otherwise the
// introsort loop runs with depth limit 2*lg(n) over the 0x10-byte pairs, then the final insertion sort leaves the range ordered
#include <cstdio>
struct Pair10 {                                       // test-only pair of the retail size 0x10: a key and its value words
    int Key;
    int Value[3];
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
static void IntrosortLoop(Pair10*, Pair10*, int depth) { ++gLoopCalls; gLastDepth = depth; }
static void FinalInsertionSort(Pair10* first, Pair10* last)
{
    ++gFinalCalls;
    for (Pair10* i = first + 1; i < last; ++i) {
        Pair10 v = *i;
        Pair10* j = i;
        while (j != first && v.Key < (j - 1)->Key) { *j = *(j - 1); --j; }
        *j = v;
    }
}
static void Sort(Pair10* first, Pair10* last)
{
    if (first != last) {
        IntrosortLoop(first, last, Lg((int)(last - first)) * 2);
        FinalInsertionSort(first, last);
    }
}
int main()
{
    int fails = 0;
    if ((unsigned int)sizeof(Pair10) != 0x10u) fails |= 1;
    Pair10 v[20];
    for (int i = 0; i < 20; ++i) { v[i].Key = (i * 7) % 20; for (int k = 0; k < 3; ++k) v[i].Value[k] = v[i].Key * 10 + k; }
    Sort(v, v);
    if (gLoopCalls != 0 || gFinalCalls != 0) fails |= 2;
    Sort(v, v + 5);
    if (gLoopCalls != 1 || gLastDepth != 4 || gFinalCalls != 1) fails |= 4;
    Sort(v, v + 20);
    if (gLoopCalls != 2 || gLastDepth != 8 || gFinalCalls != 2) fails |= 8;
    for (int i = 0; i < 20; ++i) if (v[i].Key != i || v[i].Value[3 - 1] != i * 10 + 3 - 1) fails |= 16;
    Sort(v, v + 1);
    if (gLastDepth != 0) fails |= 32;
    if (fails) { std::printf("004eb9a6_TEST FAIL %d\n", fails); return 1; }
    std::printf("004eb9a6_TEST PASS\n");
    return 0;
}