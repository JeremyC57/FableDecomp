// Behaviour test for 0x0049CFE0 CNamedValue_0049cfe0::CNamedValue_0049cfe0(): an empty CCharString name and a zero value
#include <cstdio>
#include <new>
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

struct CNamedValue_0049cfe0 {
    CCharString m_name;
    long m_value;
    CNamedValue_0049cfe0();
};
CNamedValue_0049cfe0::CNamedValue_0049cfe0()
{
    m_value = 0;
}
int main()
{
    unsigned char raw[sizeof(CNamedValue_0049cfe0)];
    unsigned int i;
    for (i = 0; i < sizeof(raw); ++i) raw[i] = 0xcd;
    CNamedValue_0049cfe0* v = new (raw) CNamedValue_0049cfe0();
    if (v->m_value != 0 || v->m_name.storage_ != 0 || g_charStrings != 1) { std::printf("0049cfe0_TEST FAIL\n"); return 1; }
    v->~CNamedValue_0049cfe0();
    std::printf("0049cfe0_TEST PASS\n");
    return 0;
}