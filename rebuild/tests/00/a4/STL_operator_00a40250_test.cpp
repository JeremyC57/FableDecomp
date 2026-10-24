// Behaviour test for 0x00A40250 _STL::operator< over pair<CCharString, CCharString> (first strings, then second; null data sorts first, shared data is equal).
#include <cstdio>
#include <cstring>

static int g_dataCompares = 0;
template <class C> class CBasicString {
public:
    const C* s;
    bool operator<(const CBasicString& other) const { ++g_dataCompares; return std::strcmp(s, other.s) < 0; }   // stand-in for 0x00429950
};
class CCharString {
public:
    CBasicString<char>* PData;
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData) return false;
        if (PData == 0) return true;
        if (other.PData == 0) return false;
        return *PData < *other.PData;
    }
};
template <class T1, class T2> struct pair { T1 first; T2 second; };
template <class T1, class T2>
bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}
typedef pair<CCharString, CCharString> P;

static P make(CBasicString<char>* a, CBasicString<char>* b) { P p; p.first.PData = a; p.second.PData = b; return p; }

int main()
{
    CBasicString<char> apple = { "apple" }, apple2 = { "apple" }, berry = { "berry" }, cherry = { "cherry" };
    // first strings decide
    if (!(make(&apple, &cherry) < make(&berry, &apple)) || make(&berry, &apple) < make(&apple, &cherry)) { std::printf("00a40250_TEST FAIL\n"); return 1; }
    // equal first strings (same data, then equal text in different data) fall through to the second strings
    if (!(make(&apple, &berry) < make(&apple, &cherry)) || make(&apple, &cherry) < make(&apple, &berry)) { std::printf("00a40250_TEST FAIL\n"); return 1; }
    if (!(make(&apple, &berry) < make(&apple2, &cherry)) || make(&apple2, &cherry) < make(&apple, &berry)) { std::printf("00a40250_TEST FAIL\n"); return 1; }
    // null data sorts before everything; equal pairs are not less
    if (!(make(0, &cherry) < make(&apple, &apple)) || make(&apple, &apple) < make(0, &cherry)) { std::printf("00a40250_TEST FAIL\n"); return 1; }
    if (!(make(&apple, 0) < make(&apple, &apple)) || make(&apple, &berry) < make(&apple, &berry)) { std::printf("00a40250_TEST FAIL\n"); return 1; }
    g_dataCompares = 0;
    if (make(&apple, &berry) < make(&apple, &berry) || g_dataCompares != 0) { std::printf("00a40250_TEST FAIL\n"); return 1; }   // shared data: no text compare
    std::printf("00a40250_TEST PASS\n");
    return 0;
}