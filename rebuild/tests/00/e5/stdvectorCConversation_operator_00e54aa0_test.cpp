// Behaviour test for 0x00E54AA0 std::vector<CConversation>::operator=: STLport copy assignment -- self-assignment is a no-op; a
// longer source reallocates (copy-construct into new storage, the old elements destroyed and the buffer freed); a shorter or
// equal source assigns over the prefix and destroys the surplus; a longer source within capacity assigns the prefix and
// copy-constructs the tail. Element constructions and destructions balance.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_live, g_assigns, g_frees;
class CConversation { public:
    int Value;
    CConversation(int v) : Value(v) { ++g_live; }
    CConversation(const CConversation& o) : Value(o.Value) { ++g_live; }
    CConversation& operator=(const CConversation& o) { Value = o.Value; ++g_assigns; return *this; }
    ~CConversation() { --g_live; }
};
struct Vec {
    CConversation* _M_start; CConversation* _M_finish; CConversation* _M_end_of_storage;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    static void Destroy(CConversation* f, CConversation* l) { for (; f != l; ++f) f->~CConversation(); }
    static CConversation* UCopy(const CConversation* f, const CConversation* l, CConversation* r) { for (; f != l; ++f, ++r) new (r) CConversation(*f); return r; }
    static CConversation* Copy(const CConversation* f, const CConversation* l, CConversation* r) { for (int n = (int)(l - f); n > 0; --n) { *r = *f; ++f; ++r; } return r; }
    void _M_clear() { Destroy(_M_start, _M_finish); if (_M_start) { std::free(_M_start); ++g_frees; } }
    CConversation* _M_allocate_and_copy(unsigned int n, const CConversation* f, const CConversation* l)
    { CConversation* r = n ? (CConversation*)std::malloc(n * sizeof(CConversation)) : 0; UCopy(f, l, r); return r; }
    Vec& operator=(const Vec& x)
    {
        if (&x != this) {
            const unsigned int xlen = x.size();
            if (xlen > capacity()) {
                CConversation* tmp = _M_allocate_and_copy(xlen, x._M_start, x._M_finish);
                _M_clear();
                _M_start = tmp;
                _M_end_of_storage = _M_start + xlen;
            } else if (size() >= xlen) {
                CConversation* i = Copy(x._M_start, x._M_finish, _M_start);
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
    v._M_start = (CConversation*)std::malloc(cap * sizeof(CConversation));
    for (unsigned int i = 0; i < n; ++i) new (v._M_start + i) CConversation(base + (int)i);
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
    if (!ok) { std::printf("00e54aa0_TEST FAIL\n"); return 1; }
    std::printf("00e54aa0_TEST PASS\n");
    return 0;
}