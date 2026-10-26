// behaviour test for 0x0051D030 std::vector<set<unsigned long> >::resize(n, x): shrinking destroys the tail and keeps the head; growing
// appends n - size() copies of x; resize to the current size changes nothing
#include <cstdio>
#include <vector>
static int g_live = 0;
struct Elem {                                              // 12-byte element
    int a, b, c;
    Elem() : a(0), b(0), c(0) { ++g_live; }
    Elem(int x) : a(x), b(x + 1), c(x + 2) { ++g_live; }
    Elem(const Elem& o) : a(o.a), b(o.b), c(o.c) { ++g_live; }
    Elem& operator=(const Elem& o) { a = o.a; b = o.b; c = o.c; return *this; }
    ~Elem() { --g_live; }
};
static void resize(std::vector<Elem>& v, unsigned int n, const Elem& x)
{
    if (n < v.size())
        v.erase(v.begin() + n, v.end());
    else
        v.insert(v.end(), n - v.size(), x);
}
int main()
{
    int fail = 0;
    if (sizeof(Elem) != 12u) fail |= 1;
    {
        std::vector<Elem> v;
        Elem x(7);
        resize(v, 3, x);
        if (v.size() != 3u || v[0].a != 7 || v[2].c != 9) fail |= 2;
        v[1] = Elem(40);
        resize(v, 5, Elem(100));
        if (v.size() != 5u || v[1].a != 40 || v[3].a != 100 || v[4].b != 101) fail |= 4;
        resize(v, 2, x);
        if (v.size() != 2u || v[0].a != 7 || v[1].a != 40) fail |= 8;
        resize(v, 2, Elem(55));
        if (v.size() != 2u || v[1].a != 40) fail |= 16;
        if (g_live != 3) fail |= 32;                       // two elements plus x
        resize(v, 0, x);
        if (!v.empty()) fail |= 64;
    }
    if (g_live != 0) fail |= 128;
    if (fail) { std::printf("0051d030_TEST FAIL %d\n", fail); return 1; }
    std::printf("0051d030_TEST PASS\n");
    return 0;
}