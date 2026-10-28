// behaviour test for 0x004701CE std::vector<CAppearanceModifierGraphics::CEntry>::_M_insert_overflow (STLport 4.6, __false_type: grow to size + max(size, n))
#include <cstdio>
#include <cstdlib>
#include <new>
struct Rec { unsigned long W[6]; };                     // the 6-dword element
static Rec make(unsigned long v) { Rec r; for (int i = 0; i < 6; ++i) r.W[i] = v * 100 + i; return r; }
struct Vec {
    Rec* Start; Rec* Finish; Rec* End;
    void init(unsigned n) { Start = (Rec*)std::malloc(n * sizeof(Rec)); for (unsigned i = 0; i < n; ++i) new (Start + i) Rec(make(i + 1)); Finish = End = Start + n; }
    void insert_overflow(Rec* pos, const Rec& x, unsigned fill_len, bool atend)
    {
        const unsigned old = (unsigned)(Finish - Start);
        const unsigned len = old + (old < fill_len ? fill_len : old);
        Rec* ns = len != 0 ? (Rec*)std::malloc(len * sizeof(Rec)) : 0;
        Rec* nf = ns;
        for (Rec* p = Start; p != pos; ++p, ++nf) new (nf) Rec(*p);
        if (fill_len == 1) { new (nf) Rec(x); ++nf; }
        else for (unsigned n = fill_len; n > 0; --n, ++nf) new (nf) Rec(x);
        if (!atend) for (Rec* p = pos; p != Finish; ++p, ++nf) new (nf) Rec(*p);
        if (Start) std::free(Start);
        Start = ns; Finish = nf; End = ns + len;
    }
};
static bool is(const Rec& r, unsigned long v) { for (int i = 0; i < 6; ++i) if (r.W[i] != v * 100 + i) return false; return true; }
int main()
{
    Vec a; a.init(3); a.insert_overflow(a.Start + 1, make(9), 1, false);   // one copy in the middle
    bool ok = (a.Finish - a.Start) == 4 && (a.End - a.Start) == 6 && is(a.Start[0], 1) && is(a.Start[1], 9) && is(a.Start[2], 2) && is(a.Start[3], 3);
    Vec b; b.init(2); b.insert_overflow(b.Finish, make(7), 5, true);       // push of 5 at the end
    ok = ok && (b.Finish - b.Start) == 7 && (b.End - b.Start) == 7 && is(b.Start[1], 2) && is(b.Start[2], 7) && is(b.Start[6], 7);
    std::free(a.Start); std::free(b.Start);
    std::printf(ok ? "004701ce_TEST PASS\n" : "004701ce_TEST FAIL\n");
    return ok ? 0 : 1;
}