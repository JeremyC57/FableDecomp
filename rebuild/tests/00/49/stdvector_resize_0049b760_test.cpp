#include <cstdio>
#include <cstring>
#include <new>

// Behaviour model of STLport vector<CCharString>::resize(n) (0x0049B760): shrink through erase(begin()+n, end()),
// grow through insert(end(), n - size(), CCharString()).
static int g_ctors = 0, g_dtors = 0;

class CCharString { public:
    const char* PStringData;
    CCharString() : PStringData("") { ++g_ctors; }
    CCharString(const char* s) : PStringData(s) { ++g_ctors; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_ctors; }
    ~CCharString() { ++g_dtors; }
    const CCharString& operator=(const CCharString& o) { PStringData = o.PStringData; return *this; }
};

struct CCharStringVector {
    CCharString* _M_start;
    CCharString* _M_finish;
    CCharString* _M_end_of_storage;
    CCharString* begin() { return _M_start; }
    CCharString* end() { return _M_finish; }
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    CCharString* erase(CCharString* f, CCharString* l)
    {
        CCharString* d = f;
        for (CCharString* s = l; s != _M_finish; ++s, ++d) *d = *s;
        for (CCharString* p = d; p != _M_finish; ++p) p->~CCharString();
        _M_finish = d;
        return f;
    }
    void insert(CCharString* pos, unsigned int n, const CCharString& x)   // the test only appends at end() within capacity
    {
        if (pos != _M_finish || _M_finish + n > _M_end_of_storage) return;
        for (unsigned int i = 0; i < n; ++i) new (_M_finish++) CCharString(x);
    }
    void resize(unsigned int __new_size, const CCharString& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
    void resize(unsigned int __new_size) { resize(__new_size, CCharString()); }
};

static unsigned char g_raw[sizeof(CCharString) * 32];

int main()
{
    CCharString* buf = (CCharString*)g_raw;
    CCharStringVector v; v._M_start = v._M_finish = buf; v._M_end_of_storage = buf + 32;
    const char* names[] = { "a", "b", "c", "d", "e" };
    for (int i = 0; i < 5; ++i) new (v._M_finish++) CCharString(names[i]);
    g_ctors = g_dtors = 0;
    v.resize(8);                                                   // grow by three empty strings
    if (v.size() != 8 || g_ctors != 4 || g_dtors != 1) { std::printf("0049b760_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i) if (std::strcmp(buf[i].PStringData, names[i]) != 0) { std::printf("0049b760_TEST FAIL\n"); return 1; }
    for (int i = 5; i < 8; ++i) if (buf[i].PStringData[0] != 0) { std::printf("0049b760_TEST FAIL\n"); return 1; }
    g_ctors = g_dtors = 0;
    v.resize(2);                                                   // shrink: six destroyed + the temporary
    if (v.size() != 2 || g_ctors != 1 || g_dtors != 7) { std::printf("0049b760_TEST FAIL\n"); return 1; }
    if (std::strcmp(buf[0].PStringData, "a") != 0 || std::strcmp(buf[1].PStringData, "b") != 0) { std::printf("0049b760_TEST FAIL\n"); return 1; }
    g_ctors = g_dtors = 0;
    v.resize(2);                                                   // same size: insert of zero copies
    if (v.size() != 2 || g_ctors != 1 || g_dtors != 1) { std::printf("0049b760_TEST FAIL\n"); return 1; }
    std::printf("0049b760_TEST PASS\n");
    return 0;
}