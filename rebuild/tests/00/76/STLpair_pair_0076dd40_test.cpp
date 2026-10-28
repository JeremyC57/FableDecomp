// Behaviour test for 0x0076DD40 pair<CCharString, CCharString>::pair(const CCharString&, const CCharString&).
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    static int s_copies;
    static int s_assigns;
    CCharString(const char* s) : PData(s) {}
    CCharString(const CCharString& o) : PData(o.PData) { ++s_copies; }
    const CCharString& operator=(const CCharString& o) { PData = o.PData; ++s_assigns; return *this; }
};
int CCharString::s_copies = 0;
int CCharString::s_assigns = 0;
static bool Same(const CCharString& s, const char* t) { return std::strcmp(s.PData, t) == 0; }
namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}

int main()
{
    CCharString k("KEY"), v("VALUE");
    CCharString::s_copies = 0;
    stlp::pair<CCharString, CCharString> p(k, v);
    if (!(CCharString::s_copies == 2 && Same(p.first, "KEY") && Same(p.second, "VALUE"))) {
        std::printf("0076dd40_TEST FAIL\n");
        return 1;
    }
    std::printf("0076dd40_TEST PASS\n");
    return 0;
}