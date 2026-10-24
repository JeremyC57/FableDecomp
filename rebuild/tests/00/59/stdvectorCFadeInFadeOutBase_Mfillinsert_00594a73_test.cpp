// Behaviour test for 0x00594A73 std::vector<CFadeInFadeOutBase>::_M_fill_insert (spare-capacity arms, model copy)
#include <cstdio>
#include <new>
struct Fade {
    float FadeInTime; int tag;
    Fade() : FadeInTime(0), tag(0) {}
    Fade(float t, int g) : FadeInTime(t), tag(g) {}
    Fade(const Fade& o) : FadeInTime(o.FadeInTime), tag(o.tag) {}
};
static Fade* ucopy(Fade* f, Fade* l, Fade* r) { Fade* c = r; for (; f != l; ++f, ++c) new (c) Fade(*f); return c; }
static Fade* ufill_n(Fade* f, unsigned int n, const Fade& x) { for (; n > 0; --n, ++f) new (f) Fade(x); return f; }
static Fade* copy_backward(Fade* f, Fade* l, Fade* r) { for (int n = int(l - f); n > 0; --n) *--r = *--l; return r; }
static void fill(Fade* f, Fade* l, const Fade& x) { for (; f != l; ++f) *f = x; }
struct Vec {
    Fade* start; Fade* finish; Fade* eos; int overflows;
    void fill_insert(Fade* pos, unsigned int n, const Fade& x)
    {
        if (n != 0) {
            if ((unsigned int)(eos - finish) >= n) {
                Fade x_copy = x;
                const unsigned int elems_after = (unsigned int)(finish - pos);
                Fade* old_finish = finish;
                if (elems_after > n) {
                    ucopy(finish - n, finish, finish); finish += n;
                    copy_backward(pos, old_finish - n, old_finish);
                    fill(pos, pos + n, x_copy);
                } else {
                    ufill_n(finish, n - elems_after, x_copy); finish += n - elems_after;
                    ucopy(pos, old_finish, finish); finish += elems_after;
                    fill(pos, old_finish, x_copy);
                }
            } else ++overflows;
        }
    }
};
int main()
{
    Fade buf[16];
    Vec v; v.start = buf; v.finish = buf; v.eos = buf + 16; v.overflows = 0;
    for (int i = 0; i < 5; ++i) { buf[i] = Fade((float)i, i); } v.finish = buf + 5;
    v.fill_insert(v.start + 3, 1, Fade(9.0f, 9));            // elems_after 2 > 1: 0 1 2 9 3 4
    int want1[6] = { 0, 1, 2, 9, 3, 4 };
    if (v.finish - v.start != 6) { std::printf("00594a73_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 6; ++i) if (v.start[i].tag != want1[i]) { std::printf("00594a73_TEST FAIL\n"); return 1; }
    v.fill_insert(v.start + 4, 3, Fade(7.0f, 7));            // elems_after 2 <= 3: 0 1 2 9 7 7 7 3 4
    int want2[9] = { 0, 1, 2, 9, 7, 7, 7, 3, 4 };
    if (v.finish - v.start != 9) { std::printf("00594a73_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i) if (v.start[i].tag != want2[i]) { std::printf("00594a73_TEST FAIL\n"); return 1; }
    v.fill_insert(v.start, 0, Fade(1.0f, 1));
    v.fill_insert(v.start, 8, Fade(1.0f, 1));                // no room: overflow arm
    if (v.overflows != 1 || v.finish - v.start != 9) { std::printf("00594a73_TEST FAIL\n"); return 1; }
    std::printf("00594a73_TEST PASS\n");
    return 0;
}