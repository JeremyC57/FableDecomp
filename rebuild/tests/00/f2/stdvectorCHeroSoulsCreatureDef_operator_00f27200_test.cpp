// Behaviour test for 0x00F27200 std::vector<CHeroSoulsCreatureDef>::operator=: the three STLport branches -- reallocate
// (copy-construct all into new storage, destroy and free the old), shrink (assign the prefix, destroy the surplus through
// the virtual dtor) and grow within capacity (assign the live prefix, copy-construct the tail) -- and self-assignment.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_assigns, g_copies, g_dtors, g_frees;
class CHeroSoulsCreatureDef { public:
    long NumCreatures;
    CHeroSoulsCreatureDef(long n) : NumCreatures(n) { }
    CHeroSoulsCreatureDef(const CHeroSoulsCreatureDef& o) : NumCreatures(o.NumCreatures) { ++g_copies; }
    CHeroSoulsCreatureDef& operator=(const CHeroSoulsCreatureDef& o) { NumCreatures = o.NumCreatures; ++g_assigns; return *this; }
    virtual ~CHeroSoulsCreatureDef() { ++g_dtors; }
};
struct Vec {
    CHeroSoulsCreatureDef* _M_start; CHeroSoulsCreatureDef* _M_finish; CHeroSoulsCreatureDef* _M_end;
    unsigned size() const { return (unsigned)(_M_finish - _M_start); }
    unsigned capacity() const { return (unsigned)(_M_end - _M_start); }
    static CHeroSoulsCreatureDef* alloc(unsigned n) { return (CHeroSoulsCreatureDef*)std::malloc(n * sizeof(CHeroSoulsCreatureDef)); }
    void init(unsigned cap, unsigned n, long base)
    {
        _M_start = alloc(cap); _M_finish = _M_start; _M_end = _M_start + cap;
        for (unsigned i = 0; i < n; ++i) { new (_M_finish) CHeroSoulsCreatureDef(base + (long)i); ++_M_finish; }
    }
    void clear() { for (CHeroSoulsCreatureDef* p = _M_start; p != _M_finish; ++p) p->~CHeroSoulsCreatureDef(); std::free(_M_start); ++g_frees; }
    Vec& operator=(const Vec& x)
    {
        if (&x != this) {
            const unsigned xlen = x.size();
            if (xlen > capacity()) {
                CHeroSoulsCreatureDef* tmp = alloc(xlen);
                for (unsigned i = 0; i < xlen; ++i) new (tmp + i) CHeroSoulsCreatureDef(x._M_start[i]);
                clear();
                _M_start = tmp; _M_end = _M_start + xlen;
            } else if (size() >= xlen) {
                CHeroSoulsCreatureDef* i = _M_start;
                for (const CHeroSoulsCreatureDef* s = x._M_start; s != x._M_finish; ++s, ++i) *i = *s;
                for (; i != _M_finish; ++i) i->~CHeroSoulsCreatureDef();
            } else {
                unsigned n = size(); CHeroSoulsCreatureDef* d = _M_start;
                for (unsigned k = 0; k < n; ++k) d[k] = x._M_start[k];
                CHeroSoulsCreatureDef* cur = _M_finish;
                for (const CHeroSoulsCreatureDef* s = x._M_start + n; s != x._M_finish; ++s, ++cur) if (cur) new (cur) CHeroSoulsCreatureDef(*s);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};
static void reset() { g_assigns = g_copies = g_dtors = g_frees = 0; }

int main()
{
    int ok = 1;
    Vec a, b;
    // reallocate: a has capacity 2, b has 4 elements
    a.init(2, 1, 100); b.init(4, 4, 10); reset();
    a = b;
    ok = ok && a.size() == 4 && a.capacity() == 4 && a._M_start[3].NumCreatures == 13 && g_copies == 4 && g_dtors == 1 && g_frees == 1 && g_assigns == 0;
    // shrink: a has 4, assign a 2-element vector
    Vec c; c.init(5, 2, 50); reset();
    a = c;
    ok = ok && a.size() == 2 && a._M_start[1].NumCreatures == 51 && g_assigns == 2 && g_dtors == 2 && g_copies == 0;
    // grow within capacity: a has 2 (capacity 4), assign 3 elements
    Vec d; d.init(3, 3, 70); reset();
    a = d;
    ok = ok && a.size() == 3 && a._M_start[2].NumCreatures == 72 && a._M_start[0].NumCreatures == 70 && g_assigns == 2 && g_copies == 1 && g_dtors == 0;
    // self-assignment
    reset(); a = a;
    ok = ok && a.size() == 3 && g_assigns == 0 && g_copies == 0 && g_dtors == 0;
    if (!ok) { std::printf("00f27200_TEST FAIL\n"); return 1; }
    std::printf("00f27200_TEST PASS\n");
    return 0;
}