// Behaviour test for 00695250: std::pair<long,CCharString>::pair(const long&, const CCharString&) -- key copied, string
// copied through its copy constructor once.
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

int main()
{
    long key = -42;
    CCharString name("Bowerstone");
    SPair<long, CCharString> entry(key, name);
    if (entry.first != -42 || std::strcmp(entry.second.PStringData, "Bowerstone") != 0 || g_copies != 1) {
        std::printf("00695250_TEST FAIL\n");
        return 1;
    }
    std::printf("00695250_TEST PASS\n");
    return 0;
}