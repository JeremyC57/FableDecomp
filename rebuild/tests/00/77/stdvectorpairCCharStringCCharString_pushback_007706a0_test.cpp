// Behaviour test for 0x007706A0 vector<pair<CCharString,CCharString> >::push_back
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
struct CCharString {
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) { }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_copies; }
};
struct __false_type { __false_type() { } };
struct SPair {
    CCharString first, second;
    SPair(const char* a, const char* b) : first(a), second(b) { }
    SPair(const SPair& o) : first(o.first), second(o.second) { }
};
struct CPairVector {
    SPair* _M_start;
    SPair* _M_finish;
    SPair* _M_end_of_storage;
    int overflows;
    void _M_insert_overflow(SPair* pos, const SPair& x, const __false_type&, unsigned int fill, bool atend)
    {   // stand-in for 0x007704A0 (append only)
        ++overflows;
        unsigned int oldSize = (unsigned int)(_M_finish - _M_start);
        unsigned int len = oldSize + (oldSize > fill ? oldSize : fill);
        SPair* s = (SPair*)std::malloc(len * sizeof(SPair));
        SPair* f = s;
        for (SPair* p = _M_start; p != pos; ++p, ++f) new (f) SPair(*p);
        new (f) SPair(x); ++f;
        if (!atend) { std::printf("unexpected\n"); }
        std::free(_M_start);
        _M_start = s; _M_finish = f; _M_end_of_storage = s + len;
    }
    void push_back(const SPair& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) SPair(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};

int main()
{
    CPairVector v = { 0, 0, 0, 0 };
    SPair a("hand", "tip"), b("hilt", "blade"), c("x", "y");
    v.push_back(a);           // overflow 0 -> 1
    v.push_back(b);           // overflow 1 -> 2
    v.push_back(c);           // overflow 2 -> 4
    v.push_back(a);           // in place
    if (v.overflows != 3 || v._M_finish - v._M_start != 4 || v._M_end_of_storage - v._M_start != 4) { std::printf("007706a0_TEST FAIL\n"); return 1; }
    if (v._M_start[1].first.PStringData != b.first.PStringData || v._M_start[3].second.PStringData != a.second.PStringData
        || v._M_start[2].first.PStringData != c.first.PStringData) { std::printf("007706a0_TEST FAIL\n"); return 1; }
    std::printf("007706a0_TEST PASS\n");
    return 0;
}