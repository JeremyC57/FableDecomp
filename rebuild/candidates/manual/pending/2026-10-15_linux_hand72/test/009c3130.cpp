#include <cstdio>
// Behaviour model of 0x009C3130: the installer entry record (two names copied, size, done cleared, two flags).
static int g_copies = 0;
struct CWideString { const unsigned short* m_pData; CWideString(const unsigned short* p) : m_pData(p) {} CWideString(const CWideString& o) : m_pData(o.m_pData) { ++g_copies; } };
struct CFileInstallerEntry {
    CWideString m_source; CWideString m_destination; long m_size; bool m_done; bool m_flagA; bool m_flagB;
    CFileInstallerEntry(const CWideString& s, const CWideString& d, long size, bool a, bool b)
        : m_source(s), m_destination(d), m_size(size), m_done(false), m_flagA(a), m_flagB(b) {}
};
int main()
{
    static const unsigned short src[] = { 'a', 0 }, dst[] = { 'b', 0 };
    CWideString s(src), d(dst);
    CFileInstallerEntry e(s, d, 1234, true, false);
    if (e.m_source.m_pData != src || e.m_destination.m_pData != dst || e.m_size != 1234 || e.m_done || !e.m_flagA || e.m_flagB || g_copies != 2)
        { std::printf("009c3130_TEST FAIL\n"); return 1; }
    std::printf("009c3130_TEST PASS\n");
    return 0;
}
