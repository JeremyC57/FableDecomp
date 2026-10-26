// behaviour test for 0x005BBBF3 map<EClothingSuitPart, long>::operator[]: an absent key is inserted with a zero value at the lower-bound hint and its value returned, a present key returns its existing value without inserting
#include <cstdio>
struct Entry { long key; long value; };
static Entry gTab[8]; static int gCount = 0; static int gInserts = 0;
static int LowerBound(long k) { int i = 0; while (i < gCount && gTab[i].key < k) ++i; return i; }
static int Insert(int pos, long k, long v)
{
    ++gInserts;
    for (int j = gCount; j > pos; --j) gTab[j] = gTab[j - 1];
    gTab[pos].key = k; gTab[pos].value = v; ++gCount;
    return pos;
}
static long& Subscript(long k)                               // the function under test
{
    int i = LowerBound(k);
    if (i == gCount || k < gTab[i].key)
        i = Insert(i, k, 0);
    return gTab[i].value;
}
int main()
{
    int fail = 0;
    Subscript(5) = 50; Subscript(-3) = 30; Subscript(9) = 90;
    if (gCount != 3 || gInserts != 3) fail |= 1;
    if (gTab[0].key != -3 || gTab[1].key != 5 || gTab[2].key != 9) fail |= 2;
    if (Subscript(5) != 50 || gInserts != 3) fail |= 4;
    if (Subscript(7) != 0 || gInserts != 4 || gTab[2].key != 7) fail |= 8;
    if (fail) { std::printf("005bbbf3_TEST FAIL %d\n", fail); return 1; }
    std::printf("005bbbf3_TEST PASS\n");
    return 0;
}