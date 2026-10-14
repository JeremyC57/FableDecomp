#include <cstdio>
static int g_charAlive = 0; static long g_lookedUp = -1;
struct CWideString { void* PStringData; CWideString(void* p) : PStringData(p) { } CWideString(const CWideString& o) : PStringData(o.PStringData) { } ~CWideString() { } };
struct CCharString {
    void* PStringData;
    CCharString(void* p) : PStringData(p) { ++g_charAlive; }
    CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_charAlive; }
    ~CCharString() { --g_charAlive; }
    CWideString ToWideString() const { return CWideString(PStringData); }
};
static char g_text[] = "hello";
struct CDefStringTable { CCharString GetString(long id) { g_lookedUp = id; return CCharString(g_text); } };
CDefStringTable g_DefStringTable;
struct CDefString { long TablePos; CWideString ToWideString() const { CCharString text = g_DefStringTable.GetString(TablePos); return text.ToWideString(); } };
int main()
{
    CDefString s; s.TablePos = 17;
    CWideString w = s.ToWideString();
    if (g_lookedUp != 17 || w.PStringData != g_text || g_charAlive != 0) { std::printf("00415d90_TEST FAIL\n"); return 1; }
    std::printf("00415d90_TEST PASS\n"); return 0;
}
