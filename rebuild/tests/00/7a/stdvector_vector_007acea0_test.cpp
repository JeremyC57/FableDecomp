// Behaviour test for 0x007ACEA0 vector<CCharString>::vector(const vector&): exact-size allocation + copy-construction.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) {}
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_copies; }
};

struct Vec {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end;
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    explicit Vec(const Vec& x) : _M_start(0), _M_finish(0), _M_end(0)
    {
        unsigned int n = x.size();
        _M_start = n != 0 ? (CCharString*)std::malloc(n * sizeof(CCharString)) : 0;
        _M_finish = _M_start;
        _M_end = _M_start + n;
        CCharString* cur = _M_start;
        for (const CCharString* f = x._M_start; f != x._M_finish; ++f, ++cur)
            if (cur) new (cur) CCharString(*f);
        _M_finish = cur;
    }
    Vec() : _M_start(0), _M_finish(0), _M_end(0) {}
};

int main()
{
    CCharString src[3] = { CCharString("a"), CCharString("bb"), CCharString("ccc") };
    Vec x; x._M_start = src; x._M_finish = src + 3; x._M_end = src + 3;
    g_copies = 0;
    Vec y(x);
    if (y.size() != 3 || y._M_end != y._M_start + 3 || g_copies != 3 || y._M_start[1].PStringData != src[1].PStringData) { std::printf("007acea0_TEST FAIL\n"); return 1; }
    Vec e; Vec z(e);
    if (z._M_start != 0 || z._M_finish != 0 || z._M_end != 0 || g_copies != 3) { std::printf("007acea0_TEST FAIL\n"); return 1; }
    std::free(y._M_start);
    std::printf("007acea0_TEST PASS\n");
    return 0;
}