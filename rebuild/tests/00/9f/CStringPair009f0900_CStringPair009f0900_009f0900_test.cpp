// Behaviour test for 0x009F0900 CStringPair_009f0900::CStringPair_009f0900(long unused): two empty CCharStrings
#include <cstdio>
static long g_charStrings = 0, g_wideStrings = 0;
class CCharString {
public:
    void* storage_;
    CCharString() : storage_(0) { ++g_charStrings; }
    ~CCharString() { --g_charStrings; }
};
class CWideString {
public:
    void* storage_;
    CWideString() : storage_(0) { ++g_wideStrings; }
    ~CWideString() { --g_wideStrings; }
};

struct CStringPair_009f0900 {
    CCharString m_first;
    CCharString m_second;
    CStringPair_009f0900(long unused);
};
CStringPair_009f0900::CStringPair_009f0900(long unused)
{
}
int main()
{
    {
        CStringPair_009f0900 p(3);
        if (p.m_first.storage_ != 0 || p.m_second.storage_ != 0 || g_charStrings != 2) { std::printf("009f0900_TEST FAIL\n"); return 1; }
    }
    if (g_charStrings != 0) { std::printf("009f0900_TEST FAIL\n"); return 1; }
    std::printf("009f0900_TEST PASS\n");
    return 0;
}