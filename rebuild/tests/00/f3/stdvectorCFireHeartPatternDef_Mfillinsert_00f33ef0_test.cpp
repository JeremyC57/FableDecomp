// Behaviour test for 0x00F33EF0 std::vector<CFireHeartPatternDef>::_M_fill_insert (STLport 4.6): n == 0 is a no-op; a short insert with a long tail
// copy-constructs the last n into the raw end, assigns the rest backwards and assigns x over the gap; a long insert
// fills the raw end with x first, copy-constructs the tail after it and assigns x over the old tail; no spare capacity
// takes the overflow path (position, x, n, atend false). x_copy is made and destroyed once per in-capacity insert.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies, g_assigns, g_dtors, g_overflow, g_ovN;
class CFireHeartPatternDef { public:
    long Lines;
    CFireHeartPatternDef(long n) : Lines(n) { }
    CFireHeartPatternDef(const CFireHeartPatternDef& o) : Lines(o.Lines) { ++g_copies; }
    CFireHeartPatternDef& operator=(const CFireHeartPatternDef& o) { Lines = o.Lines; ++g_assigns; return *this; }
    virtual ~CFireHeartPatternDef() { ++g_dtors; }
};
static CFireHeartPatternDef* UninitFillN(CFireHeartPatternDef* first, unsigned n, const CFireHeartPatternDef& x)
{
    for (; n > 0; --n, ++first) if (first) new (first) CFireHeartPatternDef(x);
    return first;
}
struct Vec {
    CFireHeartPatternDef* _M_start; CFireHeartPatternDef* _M_finish; CFireHeartPatternDef* _M_end;
    void init(unsigned cap, unsigned n)
    {
        _M_start = (CFireHeartPatternDef*)std::malloc(cap * sizeof(CFireHeartPatternDef)); _M_finish = _M_start; _M_end = _M_start + cap;
        for (unsigned i = 0; i < n; ++i) { new (_M_finish) CFireHeartPatternDef((long)i); ++_M_finish; }
    }
    void _M_insert_overflow(CFireHeartPatternDef*, const CFireHeartPatternDef&, unsigned n, bool atend) { ++g_overflow; g_ovN = atend ? -1 : (int)n; }
    void _M_fill_insert(CFireHeartPatternDef* position, unsigned n, const CFireHeartPatternDef& x)
    {
        if (n != 0) {
            if ((unsigned)(_M_end - _M_finish) >= n) {
                CFireHeartPatternDef x_copy = x;
                const unsigned elems_after = (unsigned)(_M_finish - position);
                CFireHeartPatternDef* old_finish = _M_finish;
                if (elems_after > n) {
                    CFireHeartPatternDef* cur = _M_finish;
                    for (CFireHeartPatternDef* f = _M_finish - n; f != _M_finish; ++f, ++cur) if (cur) new (cur) CFireHeartPatternDef(*f);
                    _M_finish += n;
                    CFireHeartPatternDef* last = old_finish - n; CFireHeartPatternDef* result = old_finish;
                    for (int k = (int)(last - position); k > 0; --k) *--result = *--last;
                    for (CFireHeartPatternDef* p = position; p != position + n; ++p) *p = x_copy;
                } else {
                    UninitFillN(_M_finish, n - elems_after, x_copy);
                    _M_finish += n - elems_after;
                    CFireHeartPatternDef* cur = _M_finish;
                    for (CFireHeartPatternDef* f = position; f != old_finish; ++f, ++cur) if (cur) new (cur) CFireHeartPatternDef(*f);
                    _M_finish += elems_after;
                    for (CFireHeartPatternDef* p = position; p != old_finish; ++p) *p = x_copy;
                }
            } else
                _M_insert_overflow(position, x, n, false);
        }
    }
};
static void reset() { g_copies = g_assigns = g_dtors = g_overflow = 0; g_ovN = 0; }
static int Expect(const Vec& v, const long* want, unsigned n)
{
    if ((unsigned)(v._M_finish - v._M_start) != n) return 0;
    for (unsigned i = 0; i < n; ++i) if (v._M_start[i].Lines != want[i]) return 0;
    return 1;
}

int main()
{
    int ok = 1;
    CFireHeartPatternDef x(99);
    Vec a; a.init(10, 5); reset();
    a._M_fill_insert(a._M_start + 1, 0, x);
    ok = ok && g_copies == 0 && g_dtors == 0 && a._M_finish - a._M_start == 5;
    // short insert (n 2) with a long tail (4): 0 99 99 1 2 3 4
    reset(); a._M_fill_insert(a._M_start + 1, 2, x);
    { long w[] = { 0, 99, 99, 1, 2, 3, 4 }; ok = ok && Expect(a, w, 7); }
    ok = ok && g_copies == 3 && g_assigns == 4 && g_dtors == 1;
    // long insert (n 3) with a short tail (1): 0 99 99 1 2 3 7 7 7 4
    CFireHeartPatternDef y(7);
    reset(); a._M_fill_insert(a._M_start + 6, 3, y);
    { long w[] = { 0, 99, 99, 1, 2, 3, 7, 7, 7, 4 }; ok = ok && Expect(a, w, 10); }
    ok = ok && g_copies == 4 && g_assigns == 1 && g_dtors == 1;
    // full: overflow
    reset(); a._M_fill_insert(a._M_start, 2, x);
    ok = ok && g_overflow == 1 && g_ovN == 2 && g_copies == 0;
    if (!ok) { std::printf("00f33ef0_TEST FAIL\n"); return 1; }
    std::printf("00f33ef0_TEST PASS\n");
    return 0;
}