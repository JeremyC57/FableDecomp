#include <cstdio>
static int g_a = 0, g_b = 0;
struct CBaseIntelligentPointer { unsigned char b[8]; CBaseIntelligentPointer() { ++g_a; } ~CBaseIntelligentPointer() {} };
struct CString { const char* p; CString(const char* s) : p(s) {} CString(const CString& o) : p(o.p) { ++g_b; } ~CString() {} };
struct CItem { ~CItem() {} };
struct CHashedItem : CItem { virtual void v() {} unsigned long m_hash; CBaseIntelligentPointer m_pointer; CHashedItem(unsigned long h) : m_hash(h), m_pointer() {} };
struct CNamedItem : CHashedItem { CString m_name; CNamedItem(const CString& n, unsigned long h) : CHashedItem(h), m_name(n) {} };
int main()
{
    CString n("Sword"); CNamedItem x(n, 0x1234u);
    if (x.m_hash != 0x1234u || x.m_name.p != n.p || g_a != 1 || g_b != 1) { std::printf("00c20230_TEST FAIL\n"); return 1; }
    std::printf("00c20230_TEST PASS\n");
    return 0;
}
