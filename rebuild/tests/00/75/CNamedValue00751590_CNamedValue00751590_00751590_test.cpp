#include <cstdio>
#include <cstring>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
static long g_strings = 0;
struct CCharString { void* storage_; CCharString() { storage_ = 0; ++g_strings; } };
struct CNamedValue { long m_value; CCharString m_name; long m_reserved; CCharString m_text; CNamedValue() : m_value(0), m_name(), m_text() { } };
int main()
{
    CNamedValue v;
    if (g_strings != 2 || v.m_value != 0 || v.m_name.storage_ != 0 || v.m_text.storage_ != 0 || (g_is32 && sizeof(v) != 0x10) || (g_is32 && (char*)&v.m_text - (char*)&v != 0x0c)) { std::printf("00751590_TEST FAIL\n"); return 1; }
    std::printf("00751590_TEST PASS\n");
    return 0;
}