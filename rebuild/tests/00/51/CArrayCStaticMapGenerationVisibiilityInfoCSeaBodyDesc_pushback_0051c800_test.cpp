#include <cstdio>
#include <new>
// Behaviour model of STLport push_back over a counted-copy element: copy-construct at finish when there is room, else
// hand (finish, x, 1, atend) to the growth path.
struct E { int a, b; static int copies; E(int x, int y) : a(x), b(y) { } E(const E& o) : a(o.a), b(o.b) { ++copies; } };
int E::copies = 0;
struct V {
    E* s; E* f; E* e; int overflows; const E* lastx; E* lastpos;
    void insert_overflow(E* pos, const E& x, unsigned int n, bool atend) { ++overflows; lastx = &x; lastpos = pos; (void)n; (void)atend; }
    void push_back(const E& x)
    {
        if (f != e) { new (f) E(x); ++f; }
        else insert_overflow(f, x, 1, true);
    }
};
int main()
{
    E* buf = (E*)operator new(3 * sizeof(E));
    V v; v.s = buf; v.f = buf; v.e = buf + 2; v.overflows = 0; v.lastx = 0; v.lastpos = 0;
    E x(1, 2), y(3, 4), z(5, 6);
    v.push_back(x); v.push_back(y);
    if (v.f != buf + 2 || buf[0].a != 1 || buf[1].b != 4 || E::copies != 2 || v.overflows != 0) { std::printf("0051c800_TEST FAIL\n"); return 1; }
    v.push_back(z);                                      // full: growth path with finish and x
    if (v.overflows != 1 || v.lastx != &z || v.lastpos != buf + 2 || v.f != buf + 2 || E::copies != 2) { std::printf("0051c800_TEST FAIL\n"); return 1; }
    std::printf("0051c800_TEST PASS\n");
    return 0;
}