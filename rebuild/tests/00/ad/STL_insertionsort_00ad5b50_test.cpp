// Behaviour test for 0x00AD5B50 std::__insertion_sort over (long, long) pairs with a function-pointer comparator.
#include <cstdio>

struct SKeyPair8 { long first; long second; };
typedef bool (*PFNKeyPairLess)(const SKeyPair8& a, const SKeyPair8& b);

static bool KeyLess(const SKeyPair8& a, const SKeyPair8& b) { return a.first < b.first; }

static int g_linearCalls = 0;

// STLport __linear_insert (the out-of-line callee 0x00AD4880)
static void __linear_insert(SKeyPair8* first, SKeyPair8* last, SKeyPair8 value, PFNKeyPairLess comp)
{
    ++g_linearCalls;
    if (comp(value, *first)) {
        for (SKeyPair8* p = last; p != first; --p) *p = *(p - 1);
        *first = value;
    } else {
        SKeyPair8* next = last - 1;
        while (comp(value, *next)) { *last = *next; last = next; --next; }
        *last = value;
    }
}

static void __insertion_sort(SKeyPair8* first, SKeyPair8* last, PFNKeyPairLess comp)
{
    if (first == last)
        return;
    for (SKeyPair8* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}

int main()
{
    SKeyPair8 a[7] = { {5,50}, {3,30}, {9,90}, {1,10}, {3,31}, {7,70}, {0,0} };
    __insertion_sort(a, a + 7, KeyLess);
    long keys[7] = { 0, 1, 3, 3, 5, 7, 9 };
    for (int k = 0; k < 7; ++k)
        if (a[k].first != keys[k]) { std::printf("00ad5b50_TEST FAIL\n"); return 1; }
    if (a[2].second != 30 || a[3].second != 31 || g_linearCalls != 6) { std::printf("00ad5b50_TEST FAIL\n"); return 1; }
    g_linearCalls = 0;
    __insertion_sort(a, a, KeyLess);          // empty range
    __insertion_sort(a, a + 1, KeyLess);      // one element: no insert calls
    if (g_linearCalls != 0) { std::printf("00ad5b50_TEST FAIL\n"); return 1; }
    std::printf("00ad5b50_TEST PASS\n");
    return 0;
}