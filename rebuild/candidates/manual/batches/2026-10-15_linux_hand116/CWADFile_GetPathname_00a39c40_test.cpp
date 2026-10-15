#include <cstdio>
#include <cstring>
static int g_live;
struct CWideString { long m_value; };
struct CCharString {
    long m_value;
    CCharString(long v) : m_value(v) { ++g_live; }
    CCharString(const CCharString& o) : m_value(o.m_value) { ++g_live; }
    ~CCharString() { --g_live; }
    CWideString ToWideString() const { CWideString w; w.m_value = m_value * 10; return w; }
};
struct CBankFileAsync { CCharString GetEntrySymbol(unsigned long index) { return CCharString((long)index + 1); } };
struct CWADFile { void* vt; CBankFileAsync* Bank; unsigned long Index; long FilePos;
    CWideString GetPathname() { return Bank->GetEntrySymbol(Index).ToWideString(); }
};
int main()
{
    int ok = 1;
    CBankFileAsync bank;
    CWADFile f; f.vt = 0; f.Bank = &bank; f.Index = 4; f.FilePos = 0;
    CWideString w = f.GetPathname();
    if (w.m_value != 50 || g_live != 0) ok = 0;
    if (!ok) { std::printf("00a39c40_TEST FAIL\n"); return 1; }
    std::printf("00a39c40_TEST PASS\n");
    return 0;
}
