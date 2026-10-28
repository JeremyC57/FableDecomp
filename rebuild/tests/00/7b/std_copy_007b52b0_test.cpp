extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x007B52B0 std::copy(deque iterator, deque iterator, back_insert_iterator<vector>): every element of
// the segmented range (16 eight-byte records per 0x80-byte node) is appended in order, and the inserter is returned.
class CCrimeWaitingForBribe { public: long DeedHandle; long Type; };
struct DequeIt {
    CCrimeWaitingForBribe* cur; CCrimeWaitingForBribe* first; CCrimeWaitingForBribe* last; CCrimeWaitingForBribe** node;
    void operator++() { ++cur; if (cur == last) { ++node; first = *node; last = first + 16; cur = first; } }
    int operator-(const DequeIt& x) const { return 16 * (int)(node - x.node - 1) + (int)(cur - first) + (int)(x.last - x.cur); }
};
struct Vec { CCrimeWaitingForBribe data[64]; int n; void push_back(const CCrimeWaitingForBribe& c) { data[n++] = c; } };
struct BackInserter { Vec* c; };
static BackInserter copy(DequeIt first, DequeIt last, BackInserter out)
{
    for (int n = last - first; n > 0; --n) { out.c->push_back(*first.cur); ++first; }
    return out;
}
int main()
{
    static CCrimeWaitingForBribe b0[16], b1[16], b2[16];
    CCrimeWaitingForBribe* map[3] = { b0, b1, b2 };
    for (int i = 0; i < 16; ++i) { b0[i].DeedHandle = i; b1[i].DeedHandle = 16 + i; b2[i].DeedHandle = 32 + i; b0[i].Type = b1[i].Type = b2[i].Type = 3; }
    DequeIt f; f.node = &map[0]; f.first = b0; f.last = b0 + 16; f.cur = b0 + 10;
    DequeIt l; l.node = &map[2]; l.first = b2; l.last = b2 + 16; l.cur = b2 + 4;
    Vec v; v.n = 0; BackInserter in; in.c = &v;
    BackInserter r = copy(f, l, in);
    CHECK(r.c == &v);
    CHECK(v.n == 6 + 16 + 4);
    CHECK(v.data[0].DeedHandle == 10 && v.data[5].DeedHandle == 15 && v.data[6].DeedHandle == 16 && v.data[25].DeedHandle == 35);
    Vec w; w.n = 0; in.c = &w; copy(f, f, in); CHECK(w.n == 0);
    if (fails == 0) printf("007b52b0_TEST PASS\n"); else printf("007b52b0_TEST FAIL\n");
    return fails;
}