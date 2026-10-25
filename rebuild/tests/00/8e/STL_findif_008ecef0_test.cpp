// Behaviour test for 0x008ECEF0 std::__find_if: unrolled search returns the first match or last.
#include <cstdio>

struct Item { long first; int second; };

struct DefIndexEquals {
    long DefIndex;
    DefIndexEquals(long d) : DefIndex(d) {}
    bool operator()(const Item& item) const { return item.first == DefIndex; }
};

static Item* __find_if(Item* first, Item* last, DefIndexEquals pred)
{
    int trip = (int)(last - first) >> 2;
    for ( ; trip > 0; --trip) {
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
    Item v[11];
    for (int i = 0; i < 11; ++i) { v[i].first = 100 + i; v[i].second = i; }
    v[9].first = 104;                                              // duplicate key later in the range
    int fail = 0;
    for (int n = 0; n <= 11; ++n)                                  // every tail length
        for (int k = 0; k < n; ++k)
            if (v[k].first != 104 || k == 4)
                if (__find_if(v, v + n, DefIndexEquals(v[k].first)) != v + k) fail = 1;
    if (__find_if(v, v + 11, DefIndexEquals(104)) != v + 4) fail = 1;
    if (__find_if(v, v + 11, DefIndexEquals(7)) != v + 11) fail = 1;
    if (__find_if(v, v, DefIndexEquals(100)) != v) fail = 1;
    if (fail) { std::printf("008ecef0_TEST FAIL\n"); return 1; }
    std::printf("008ecef0_TEST PASS\n");
    return 0;
}