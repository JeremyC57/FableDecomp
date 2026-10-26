// Behaviour test for 0x0076F740 std::__uninitialized_fill_n over pair<CCharString,CCharString>
#include <cstdio>
#include <new>

static int g_copies = 0;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
};

struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
typedef pair<CCharString, CCharString> CStringPair;

static CStringPair* UninitializedFillN(CStringPair* first, unsigned int n, const CStringPair& x, const __false_type&)
{
    CStringPair* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CStringPair(x);
    return cur;
}

int main()
{
    static unsigned char raw[sizeof(CStringPair) * 4];
    CStringPair* slots = (CStringPair*)raw;
    CStringPair value(CCharString("key"), CCharString("val"));
    g_copies = 0;
    CStringPair* end = UninitializedFillN(slots, 3, value, __false_type());
    if (end != slots + 3 || g_copies != 6) { std::printf("0076f740_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (slots[i].first.PStringData != value.first.PStringData || slots[i].second.PStringData != value.second.PStringData) {
            std::printf("0076f740_TEST FAIL\n"); return 1;
        }
    if (UninitializedFillN(slots, 0, value, __false_type()) != slots || g_copies != 6) { std::printf("0076f740_TEST FAIL\n"); return 1; }
    std::printf("0076f740_TEST PASS\n");
    return 0;
}