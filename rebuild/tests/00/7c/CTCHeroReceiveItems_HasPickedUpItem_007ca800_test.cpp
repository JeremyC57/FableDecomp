extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
template <class It, class T> It find4(It first, It last, const T& val)   // STLport random-access __find
{
    int trip = (int)(last - first) >> 2;
    for (; trip > 0; --trip) {
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
        if (*first == val) return first; ++first;
    }
    switch (last - first) {
    case 3: if (*first == val) return first; ++first;
    case 2: if (*first == val) return first; ++first;
    case 1: if (*first == val) return first;
    default: return last;
    }
}
// Behaviour test for 0x007CA800 CTCHeroReceiveItems::HasPickedUpItem: whether the item id is in PickedUpItems.
class CTCHeroReceiveItems { public:
    const long* first; const long* last;
    bool HasPickedUpItem(long item) const { if (find4(first, last, item) != last) return true; return false; }
};
int main()
{
    static const long v[9] = { 4, 8, 15, 16, 23, 42, 7, 9, 11 };
    CTCHeroReceiveItems r; r.first = v; r.last = v + 9;
    CHECK(r.HasPickedUpItem(4)); CHECK(r.HasPickedUpItem(42)); CHECK(r.HasPickedUpItem(11));
    CHECK(!r.HasPickedUpItem(5));
    r.last = v + 8; CHECK(!r.HasPickedUpItem(11));
    r.last = v; CHECK(!r.HasPickedUpItem(4));
    if (fails == 0) printf("007ca800_TEST PASS\n"); else printf("007ca800_TEST FAIL\n");
    return fails;
}