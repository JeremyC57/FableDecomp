// Behaviour test for 00695b70: std::make_pair<long,CCharString>(const long&, const CCharString&) -- returns a pair
// holding the key and a copy of the string.
#include <cstdio>
#include <cstring>

static int g_copies = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) { }
    CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
};
template <class T1, class T2> struct SPair {
    T1 first;
    T2 second;
    SPair(const T1& a, const T2& b) : first(a), second(b) { }
};
template <class T1, class T2> inline SPair<T1, T2> make_pair_model(const T1& x, const T2& y)
{
    return SPair<T1, T2>(x, y);
}

int main()
{
    CCharString name("Witchwood");
    SPair<long, CCharString> entry = make_pair_model(7L, name);
    if (entry.first != 7 || std::strcmp(entry.second.PStringData, "Witchwood") != 0 || g_copies < 1) {
        std::printf("00695b70_TEST FAIL\n");
        return 1;
    }
    std::printf("00695b70_TEST PASS\n");
    return 0;
}