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
// Behaviour test for 0x007FEC30 CTCUserActionPointBase::IsActionPointIndexBlocked: whether the index is in
// BlockedActionPointIndexes.
struct CTCUserActionPointBase {
    const long* first; const long* last;
    bool IsActionPointIndexBlocked(long i) { if (find4(first, last, i) != last) return true; return false; }
};
int main()
{
    static const long v[3] = { 2, 0, 5 };
    CTCUserActionPointBase p; p.first = v; p.last = v + 3;
    CHECK(p.IsActionPointIndexBlocked(0)); CHECK(p.IsActionPointIndexBlocked(5));
    CHECK(!p.IsActionPointIndexBlocked(1)); CHECK(!p.IsActionPointIndexBlocked(-1));
    p.last = v; CHECK(!p.IsActionPointIndexBlocked(2));
    if (fails == 0) printf("007fec30_TEST PASS\n"); else printf("007fec30_TEST FAIL\n");
    return fails;
}