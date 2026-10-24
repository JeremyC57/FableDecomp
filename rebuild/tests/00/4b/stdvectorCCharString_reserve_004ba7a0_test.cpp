// Behaviour test for 0x004BA7A0 vector<CCharString>::reserve(n): nothing when capacity >= n; otherwise a non-empty
// vector copies into a new n-slot block and clears the old one, an empty one just allocates; size is preserved.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct CCharString { int v; };
static int g_clears = 0, g_copies = 0;

struct Vec {
    CCharString* _M_start; CCharString* _M_finish; CCharString* _M_end;
    unsigned int size() const { return unsigned(_M_finish - _M_start); }
    unsigned int capacity() const { return unsigned(_M_end - _M_start); }
    CCharString* _M_allocate_and_copy(unsigned int n, const CCharString* f, const CCharString* l)
    {
        CCharString* r = n ? static_cast<CCharString*>(std::malloc(n * sizeof(CCharString))) : 0;
        CCharString* c = r; for (; f != l; ++f, ++c) { *c = *f; ++g_copies; }
        return r;
    }
    void _M_clear() { ++g_clears; std::free(_M_start); }
    void reserve(unsigned int n)
    {
        if (capacity() < n) {
            const unsigned int old = size();
            CCharString* tmp;
            if (_M_start) { tmp = _M_allocate_and_copy(n, _M_start, _M_finish); _M_clear(); }
            else tmp = n ? static_cast<CCharString*>(std::malloc(n * sizeof(CCharString))) : 0;
            _M_start = tmp; _M_finish = tmp + old; _M_end = tmp + n;
        }
    }
};

int main()
{
    Vec e = { 0, 0, 0 };
    e.reserve(4);
    if (e._M_start == 0 || e.size() != 0 || e.capacity() != 4 || g_clears != 0) { std::printf("004ba7a0_TEST FAIL\n"); return 1; }
    e._M_start[0].v = 11; e._M_start[1].v = 22; e._M_finish = e._M_start + 2;
    e.reserve(3);                                      // capacity 4 >= 3: no change
    if (e.capacity() != 4 || g_clears != 0) { std::printf("004ba7a0_TEST FAIL\n"); return 1; }
    e.reserve(10);
    if (e.capacity() != 10 || e.size() != 2 || e._M_start[0].v != 11 || e._M_start[1].v != 22 || g_clears != 1 || g_copies != 2) {
        std::printf("004ba7a0_TEST FAIL\n"); return 1;
    }
    std::free(e._M_start);
    std::printf("004ba7a0_TEST PASS\n");
    return 0;
}