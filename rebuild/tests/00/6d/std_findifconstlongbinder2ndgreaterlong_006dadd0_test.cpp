// Behaviour test for 0x006DADD0 std::__find_if<const long*, binder2nd<greater<long> > >: the first element greater
// than the bound value, or last, for every remainder of the four-way unrolled loop.
#include <cstdio>

struct Greater { bool operator()(const long& x, const long& y) const { return x > y; } };
struct Binder { Greater op; long value; bool operator()(const long& x) const { return op(x, value); } };

static const long* FindIf(const long* first, const long* last, Binder pred)
{
    int trip_count = (int)(last - first) >> 2;
    for ( ; trip_count > 0 ; --trip_count) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}

int main()
{
    long v[9] = { 1, 2, 4, 8, 16, 32, 64, 128, 256 };
    for (int n = 0; n <= 9; ++n) {
        for (long bound = 0; bound <= 300; bound += 3) {
            Binder b; b.value = bound;
            const long* expect = v + n;
            for (int i = 0; i < n; ++i) if (v[i] > bound) { expect = v + i; break; }
            if (FindIf(v, v + n, b) != expect) { std::printf("006dadd0_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("006dadd0_TEST PASS\n");
    return 0;
}