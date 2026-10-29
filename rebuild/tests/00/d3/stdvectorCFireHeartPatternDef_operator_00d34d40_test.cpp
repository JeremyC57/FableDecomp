// Behaviour test for 0x00D34D40 std::vector<CFireHeartPatternDef>::operator=: STLport copy assignment -- self-assignment is a no-op; a
// longer source reallocates (copy-construct into new storage, the old elements destroyed and the buffer freed); a shorter or
// equal source assigns over the prefix and destroys the surplus; a longer source within capacity assigns the prefix and
// copy-constructs the tail. Element constructions and destructions balance.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live, g_assigns, g_frees;
class CFireHeartPatternDef { public:
    int Value;
    CFireHeartPatternDef(int v) : Value(v) { ++g_live; }
    CFireHeartPatternDef(const CFireHeartPatternDef& o) : Value(o.Value) { ++g_live; }
    CFireHeartPatternDef& operator=(const CFireHeartPatternDef& o) { Value = o.Value; ++g_assigns; return *this; }
    ~CFireHeartPatternDef() { --g_live; }
};
struct Vec {
    CFireHeartPatternDef* _M_start; CFireHeartPatternDef* _M_finish; CFireHeartPatternDef* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    static void Destroy(CFireHeartPatternDef* f, CFireHeartPatternDef* l) { for (; f != l; ++f) f->~CFireHeartPatternDef(); }
    static CFireHeartPatternDef* UCopy(const CFireHeartPatternDef* f, const CFireHeartPatternDef* l, CFireHeartPatternDef* r) { for (; f != l; ++f, ++r) new (r) CFireHeartPatternDef(*f); return r; }
    static CFireHeartPatternDef* Copy(const CFireHeartPatternDef* f, const CFireHeartPatternDef* l, CFireHeartPatternDef* r) { for (int n = (int)(l - f); n > 0; --n) { *r = *f; ++f; ++r; } return r; }
    void _M_clear() { Destroy(_M_start, _M_finish); if (_M_start) { std::free(_M_start); ++g_frees; } }
    CFireHeartPatternDef* _M_allocate_and_copy(unsigned int n, const CFireHeartPatternDef* f, const CFireHeartPatternDef* l)
    { CFireHeartPatternDef* r = n ? (CFireHeartPatternDef*)std::malloc(n * sizeof(CFireHeartPatternDef)) : 0; UCopy(f, l, r); return r; }
    Vec& operator=(const Vec& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                CFireHeartPatternDef* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
                _M_clear();
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                CFireHeartPatternDef* i = Copy(x._M_start, x._M_finish, _M_start);
                Destroy(i, _M_finish);
            } else {
                Copy(x._M_start, x._M_start + size(), _M_start);
                UCopy(x._M_start + size(), x._M_finish, _M_finish);
            }
            _M_finish = _M_start + xlen;
        }
        return *this;
    }
};
static void Make(Vec& v, unsigned int n, unsigned int cap, int base)
{
    v._M_start = (CFireHeartPatternDef*)std::malloc(cap * sizeof(CFireHeartPatternDef));
    for (unsigned int i = 0; i < n; ++i) new (v._M_start + i) CFireHeartPatternDef(base + (int)i);
    v._M_finish = v._M_start + n; v._M_end_of_storage = v._M_start + cap;
}
static int Same(const Vec& a, const Vec& b)
{
    if (a.size() != b.size()) return 0;
    for (unsigned int i = 0; i < a.size(); ++i) if (a._M_start[i].Value != b._M_start[i].Value) return 0;
    return 1;
}

int main()
{
    int ok = 1;
    Vec a, b;
    Make(a, 2, 2, 0); Make(b, 5, 5, 100);
    a = a; ok = ok && a.size() == 2 && g_assigns == 0;
    a = b; ok = ok && Same(a, b) && a.capacity() == 5 && g_frees == 1 && g_live == 10 && g_assigns == 0;      // reallocate
    Vec c; Make(c, 3, 3, 200);
    a = c; ok = ok && Same(a, c) && a.capacity() == 5 && g_live == 11 && g_assigns == 3;                       // shrink
    a = b; ok = ok && Same(a, b) && a.capacity() == 5 && g_live == 13 && g_assigns == 6 && g_frees == 1;       // grow in place
    if (!ok) { std::printf("00d34d40_TEST FAIL\n"); return 1; }
    std::printf("00d34d40_TEST PASS\n");
    return 0;
}