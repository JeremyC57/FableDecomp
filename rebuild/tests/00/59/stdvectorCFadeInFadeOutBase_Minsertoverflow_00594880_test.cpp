// Behaviour test for 0x00594880 std::vector<CFadeInFadeOutBase>::_M_insert_overflow (model copy)
#include <cstdio>
#include <cstdlib>
#include <new>
#include <stddef.h>
struct Fade {
    bool Active; float FadeInTime; float FadeOutTime; bool Closing; bool Opening; float OpenTimer; float CloseTimer;
    unsigned char ToColour[4];
    Fade(const Fade& o) : Active(o.Active), FadeInTime(o.FadeInTime), FadeOutTime(o.FadeOutTime), Closing(o.Closing),
        Opening(o.Opening), OpenTimer(o.OpenTimer), CloseTimer(o.CloseTimer)
    { for (int i = 0; i < 4; ++i) ToColour[i] = o.ToColour[i]; }
    Fade(float t) : Active(true), FadeInTime(t), FadeOutTime(2 * t), Closing(false), Opening(true), OpenTimer(0), CloseTimer(0)
    { ToColour[0] = (unsigned char)t; ToColour[1] = ToColour[2] = ToColour[3] = 0; }
};
static Fade* ucopy(const Fade* f, const Fade* l, Fade* r) { Fade* c = r; for (; f != l; ++f, ++c) new (c) Fade(*f); return c; }
static Fade* ufill_n(Fade* f, size_t n, const Fade& x) { for (; n > 0; --n, ++f) new (f) Fade(x); return f; }
template <class T> const T& tmax(const T& a, const T& b) { return a < b ? b : a; }
struct Vec {
    Fade* start; Fade* finish; Fade* eos;
    size_t size() const { return size_t(finish - start); }
    void insert_overflow(Fade* pos, const Fade& x, size_t fill_len, bool atend)
    {
        const size_t old = size();
        const size_t len = old + tmax(old, fill_len);
        Fade* ns = len != 0 ? (Fade*)malloc(len * sizeof(Fade)) : 0;
        Fade* nf = ucopy(start, pos, ns);
        if (fill_len == 1) { new (nf) Fade(x); ++nf; } else nf = ufill_n(nf, fill_len, x);
        if (!atend) nf = ucopy(pos, finish, nf);
        if (start) free(start);
        start = ns; finish = nf; eos = ns + len;
    }
};
int main()
{
    Vec v; v.start = v.finish = v.eos = 0;
    v.insert_overflow(v.finish, Fade(1.0f), 1, true);
    v.insert_overflow(v.finish, Fade(2.0f), 3, true);       // 1 2 2 2, capacity 1 + 3
    v.insert_overflow(v.start + 1, Fade(5.0f), 1, false);   // 1 5 2 2 2, capacity 4 + 4
    float want[5] = { 1.0f, 5.0f, 2.0f, 2.0f, 2.0f };
    if (v.size() != 5 || v.eos - v.start != 8) { std::printf("00594880_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (v.start[i].FadeInTime != want[i] || v.start[i].FadeOutTime != 2 * want[i] || v.start[i].ToColour[0] != (unsigned char)want[i]) { std::printf("00594880_TEST FAIL\n"); return 1; }
    free(v.start);
    std::printf("00594880_TEST PASS\n");
    return 0;
}