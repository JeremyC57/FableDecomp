#include <cstdio>
#include <string.h>
#include <stdlib.h>
static int g_parses = 0;
struct SForwardIteratorTag { SForwardIteratorTag() { } };
struct SCharRange { const char* begin; const char* end; };
struct CNetString_Retail {
    char* begin; char* end; char* capacity;
    void Construct(const char* first, const char* last, const SForwardIteratorTag&) { size_t n = last - first; begin = (char*)malloc(n + 1); memcpy(begin, first, n); begin[n] = 0; end = begin + n; capacity = end; }
    CNetString_Retail(const SCharRange& r) : begin(0), end(0), capacity(0) { Construct(r.begin, r.end, SForwardIteratorTag()); }
};
struct CHttpUrl_0083f420 {
    unsigned char _pad_0x00[0x2c]; SCharRange m_host; unsigned char _pad_0x34[0x4]; SCharRange m_path; unsigned char _pad_0x40[0x4]; unsigned short m_port;
    void Parse() { ++g_parses; }
    CNetString_Retail GetHost_0083f5c0() { Parse(); return CNetString_Retail(m_host); }
    CNetString_Retail GetPath_0083f600() { Parse(); return CNetString_Retail(m_path); }
};
static const char* g_url = "http://example.net/index.html";
int main()
{
    CHttpUrl_0083f420 u; u.m_host.begin = g_url + 7; u.m_host.end = g_url + 18; u.m_path.begin = g_url + 18; u.m_path.end = g_url + 29;
    CNetString_Retail s = u.GetHost_0083f5c0();
    if (g_parses != 1 || strcmp(s.begin, "example.net") != 0 || s.end - s.begin != 11 || s.capacity != s.end || (char*)&u.m_host - (char*)&u != 0x2c) { std::printf("0083f5c0_TEST FAIL\n"); return 1; }
    std::printf("0083f5c0_TEST PASS\n"); return 0;
}
