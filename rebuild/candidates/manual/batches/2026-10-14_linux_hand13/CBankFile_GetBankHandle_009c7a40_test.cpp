#include <cstdio>
static int g_copies = 0;
struct CWideString { void* PStringData; CWideString(void* p) : PStringData(p) {} CWideString(const CWideString& o) : PStringData(o.PStringData) { ++g_copies; } };
struct CBankFile { unsigned char _pad[0x28]; CWideString m_name; CBankFile() : m_name((void*)0x42) {} CWideString GetBankHandle() const { return m_name; } };
int main()
{
    CBankFile b;
    CWideString h = b.GetBankHandle();
    if (h.PStringData != (void*)0x42 || g_copies < 1 || &h == &b.m_name) { std::printf("009c7a40_TEST FAIL\n"); return 1; }
    std::printf("009c7a40_TEST PASS\n");
    return 0;
}
