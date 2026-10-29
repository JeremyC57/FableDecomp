// Behaviour test for 0x00F2D010 std::vector<CFeatDef>::operator=: STLport copy assignment -- self-assignment is a no-op; a
// longer source reallocates (copy-construct into new storage, the old elements destroyed and the buffer freed); a shorter or
// equal source assigns over the prefix and destroys the surplus; a longer source within capacity assigns the prefix and
// copy-constructs the tail. Element constructions and destructions balance.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live, g_assigns, g_frees;
class CFeatDef { public:
    int Value;
    CFeatDef(int v) : Value(v) { ++g_live; }
    CFeatDef(const CFeatDef& o) : Value(o.Value) { ++g_live; }
    CFeatDef& operator=(const CFeatDef& o) { Value = o.Value; ++g_assigns; return *this; }
    ~CFeatDef() { --g_live; }
};
struct Vec {
    CFeatDef* _M_start; CFeatDef* _M_finish; CFeatDef* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    static void Destroy(CFeatDef* f, CFeatDef* l) { for (; f != l; ++f) f->~CFeatDef(); }
    static CFeatDef* UCopy(const CFeatDef* f, const CFeatDef* l, CFeatDef* r) { for (; f != l; ++f, ++r) new (r) CFeatDef(*f); return r; }
    static CFeatDef* Copy(const CFeatDef* f, const CFeatDef* l, CFeatDef* r) { for (int n = (int)(l - f); n > 0; --n) { *r = *f; ++f; ++r; } return r; }
    void _M_clear() { Destroy(_M_start, _M_finish); if (_M_start) { std::free(_M_start); ++g_frees; } }
    CFeatDef* _M_allocate_and_copy(unsigned int n, const CFeatDef* f, const CFeatDef* l)
    { CFeatDef* r = n ? (CFeatDef*)std::malloc(n * sizeof(CFeatDef)) : 0; UCopy(f, l, r); return r; }
    Vec& operator=(const Vec& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                CFeatDef* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
                _M_clear();
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                CFeatDef* i = Copy(x._M_start, x._M_finish, _M_start);
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
    v._M_start = (CFeatDef*)std::malloc(cap * sizeof(CFeatDef));
    for (unsigned int i = 0; i < n; ++i) new (v._M_start + i) CFeatDef(base + (int)i);
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
    if (!ok) { std::printf("00f2d010_TEST FAIL\n"); return 1; }
    std::printf("00f2d010_TEST PASS\n");
    return 0;
}