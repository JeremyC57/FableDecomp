// behaviour test for 0x005F7ACC std::__find over 4-byte elements (EHeroAbility*, EHeroAbility): the first match from the front, the unrolled
// trips and every remainder length 0..3, and last when there is no match
#include <cstdio>
static const int* find4(const int* first, const int* last, const int& val)   // the function under test
{
    int trips = (int)(last - first) >> 2;
    for (; trips > 0; --trips) {
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
    }
    switch (last - first) {
    case 3: if (*first == val) return first; ++first;
    case 2: if (*first == val) return first; ++first;
    case 1: if (*first == val) return first; ++first;
    default: return last;
    }
}
int main()
{
    int fail = 0;
    int a[11];
    for (int i = 0; i < 11; ++i) a[i] = 100 + i;
    for (int n = 0; n <= 11; ++n) {
        for (int k = 0; k < n; ++k) { int v = 100 + k; if (find4(a, a + n, v) != a + k) fail |= 1; }
        int miss = 7; if (find4(a, a + n, miss) != a + n) fail |= 2;
    }
    a[6] = 105; int dup = 105; if (find4(a, a + 11, dup) != a + 5) fail |= 4;   // first match wins
    if (fail) { std::printf("005f7acc_TEST FAIL %d\n", fail); return 1; }
    std::printf("005f7acc_TEST PASS\n");
    return 0;
}