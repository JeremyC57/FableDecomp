// Behaviour test for 0x007B5400 __copy(const T*, const T*, back_insert_iterator<deque<T> >): n push_backs, returns the inserter.
#include <cstdio>
struct Rec { int a, b; };
struct Deque { Rec items[32]; int n; void push_back(const Rec& r) { items[n++] = r; } };
struct Inserter {
    Deque* c;
    explicit Inserter(Deque& d) : c(&d) {}
    Inserter& operator=(const Rec& v) { c->push_back(v); return *this; }
    Inserter& operator*() { return *this; }
    Inserter& operator++() { return *this; }
};
static Inserter Copy(const Rec* first, const Rec* last, Inserter result)
{
    for (int n = (int)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
int main()
{
    Rec src[3] = { { 1, 2 }, { 3, 4 }, { 5, 6 } };
    Deque d; d.n = 0;
    Inserter out = Copy(src, src + 3, Inserter(d));
    if (out.c != &d || d.n != 3 || d.items[2].a != 5 || d.items[0].b != 2) { std::printf("007b5400_TEST FAIL\n"); return 1; }
    out = Copy(src, src, Inserter(d));
    if (d.n != 3 || out.c != &d) { std::printf("007b5400_TEST FAIL\n"); return 1; }
    std::printf("007b5400_TEST PASS\n");
    return 0;
}