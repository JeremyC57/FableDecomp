// Behaviour test for 0x005B1A7B CWideNamedValue_005b1a7b::CWideNamedValue_005b1a7b(long unused): an empty CWideString and a zero value, the argument ignored
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

struct CWideNamedValue_005b1a7b {
    CWideString m_name;
    long m_value;
    CWideNamedValue_005b1a7b(long unused);
};
CWideNamedValue_005b1a7b::CWideNamedValue_005b1a7b(long unused)
{
    m_value = 0;
}
int main()
{
    CWideNamedValue_005b1a7b a(12345);
    CWideNamedValue_005b1a7b b(-1);
    if (a.m_value != 0 || b.m_value != 0 || a.m_name.storage_ != 0 || g_wideStrings != 2) { std::printf("005b1a7b_TEST FAIL\n"); return 1; }
    std::printf("005b1a7b_TEST PASS\n");
    return 0;
}