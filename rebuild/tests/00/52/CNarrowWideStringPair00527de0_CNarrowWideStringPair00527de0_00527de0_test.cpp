// Behaviour test for 0x00527DE0 CNarrowWideStringPair_00527de0::CNarrowWideStringPair_00527de0(long unused): an empty CCharString and an empty CWideString
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

struct CNarrowWideStringPair_00527de0 {
    CCharString m_narrow;
    CWideString m_wide;
    CNarrowWideStringPair_00527de0(long unused);
};
CNarrowWideStringPair_00527de0::CNarrowWideStringPair_00527de0(long unused)
{
}
int main()
{
    CNarrowWideStringPair_00527de0 p(7);
    if (p.m_narrow.storage_ != 0 || p.m_wide.storage_ != 0 || g_charStrings != 1 || g_wideStrings != 1) { std::printf("00527de0_TEST FAIL\n"); return 1; }
    std::printf("00527de0_TEST PASS\n");
    return 0;
}