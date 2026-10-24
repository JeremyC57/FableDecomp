// Behaviour test for 0x006568F2 vector<CIntelligentPointer<CParticleEmitter>>::_M_fill_insert (model copy)
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_live = 0;
struct Ptr {
    int v;
    Ptr(int x) : v(x) { ++g_live; }
    Ptr(const Ptr& o) : v(o.v) { ++g_live; }
    Ptr& operator=(const Ptr& o) { v = o.v; return *this; }
    ~Ptr() { --g_live; }
};
static int g_overflow = 0;
Ptr* ucopy(Ptr* f, Ptr* l, Ptr* r) { for (; f != l; ++f, ++r) new (r) Ptr(*f); return r; }
Ptr* ufill(Ptr* f, unsigned int n, const Ptr& x) { for (; n > 0; --n, ++f) new (f) Ptr(x); return f; }
Ptr* cbackward(Ptr* f, Ptr* l, Ptr* r) { for (int n = int(l - f); n > 0; --n) *--r = *--l; return r; }
void fillr(Ptr* f, Ptr* l, const Ptr& x) { for (; f != l; ++f) *f = x; }
struct Vec {
    Ptr* start; Ptr* finish; Ptr* eos;
    void fill_insert(Ptr* pos, unsigned int n, const Ptr& x)
    {
        if (n != 0) {
            if ((unsigned int)(eos - finish) >= n) {
                Ptr x_copy = x;
                const unsigned int after = finish - pos;
                Ptr* old_finish = finish;
                if (after > n) {
                    ucopy(finish - n, finish, finish); finish += n;
                    cbackward(pos, old_finish - n, old_finish);
                    fillr(pos, pos + n, x_copy);
                } else {
                    ufill(finish, n - after, x_copy); finish += n - after;
                    ucopy(pos, old_finish, finish); finish += after;
                    fillr(pos, old_finish, x_copy);
                }
            } else ++g_overflow;
        }
    }
};
int main()
{
    Ptr* buf = (Ptr*)malloc(8 * sizeof(Ptr));
    for (int i = 0; i < 4; ++i) new (buf + i) Ptr(i + 1);     // 1 2 3 4, capacity 8
    Vec v; v.start = buf; v.finish = buf + 4; v.eos = buf + 8;
    v.fill_insert(v.start + 1, 2, Ptr(9));                    // after (3) > n (2): 1 9 9 2 3 4
    int e1[6] = { 1, 9, 9, 2, 3, 4 };
    if (v.finish - v.start != 6) { std::printf("006568f2_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i) if (v.start[i].v != e1[i]) { std::printf("006568f2_TEST FAIL\n"); return 1; }
    v.fill_insert(v.start + 5, 2, Ptr(7));                    // after (1) <= n (2): 1 9 9 2 3 7 7 4
    int e2[8] = { 1, 9, 9, 2, 3, 7, 7, 4 };
    if (v.finish - v.start != 8 || g_live != 8) { std::printf("006568f2_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 8; ++i) if (v.start[i].v != e2[i]) { std::printf("006568f2_TEST FAIL\n"); return 1; }
    v.fill_insert(v.start, 1, Ptr(5));                        // no room: growth path
    v.fill_insert(v.start, 0, Ptr(5));                        // n == 0: nothing
    if (g_overflow != 1 || v.finish - v.start != 8) { std::printf("006568f2_TEST FAIL\n"); return 1; }
    std::printf("006568f2_TEST PASS\n");
    return 0;
}