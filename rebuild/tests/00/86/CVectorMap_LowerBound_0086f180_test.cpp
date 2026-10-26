// behaviour test for 0x0086F180 CVectorMap::LowerBound (CVectorMap<EExpressionType, CExpressionSet::CEntry >): the first 12-byte entry whose key is not below the probe, end() when every key is below it; duplicate keys resolve to the first of the run; signed keys
#include <cstdio>
struct Entry { int first; int second_a; int second_b; };             // pair<K, V>: key +0, 8-byte value +4
static Entry* LowerBound(Entry* begin, Entry* end, const int& key)   // the function under test
{
    Entry* first = begin;
    int count = (int)(end - begin);
    while (count > 0) {
        int half = count / 2;
        Entry* middle = first + half;
        if (middle->first < key) { first = middle + 1; count -= half + 1; }
        else count = half;
    }
    return first;
}
int main()
{
    int fail = 0;
    if (sizeof(Entry) != 12u) fail |= 1;
    Entry e[7] = { { -5, 0, 0 }, { -1, 1, 0 }, { 2, 2, 0 }, { 2, 3, 0 }, { 2, 4, 0 }, { 9, 5, 0 }, { 40, 6, 0 } };
    int k;
    k = 2;   if (LowerBound(e, e + 7, k) != e + 2) fail |= 2;            // first of the duplicate run
    k = 3;   if (LowerBound(e, e + 7, k) != e + 5) fail |= 4;
    k = -9;  if (LowerBound(e, e + 7, k) != e) fail |= 8;                // signed compare
    k = 41;  if (LowerBound(e, e + 7, k) != e + 7) fail |= 16;           // end()
    k = 0;   if (LowerBound(e, e, k) != e) fail |= 32;                   // empty map
    k = -1;  if (LowerBound(e, e + 7, k)->second_a != 1) fail |= 64;
    if (fail) { std::printf("0086f180_TEST FAIL %d\n", fail); return 1; }
    std::printf("0086f180_TEST PASS\n");
    return 0;
}