// Behaviour test for std::pair<CCharString,CSymbolMap::CSymbolInfo>::~pair (0x006FB460): EntryName, GroupName, then the key string.
#include <cstdio>
static int g_log[16]; static int g_n = 0; static int g_base = 0;
class CCharString { public: int id; ~CCharString() { g_log[g_n++] = id; } };
struct CSymbolInfo { CCharString GroupName; CCharString EntryName; long Val; };
template<class T1, class T2> struct TPair { T1 first; T2 second; };
int main()
{
    { TPair<CCharString, CSymbolInfo> e; e.first.id = 1; e.second.GroupName.id = 2; e.second.EntryName.id = 3; e.second.Val = 9; }
    if (!(g_n == 3 && g_log[0] == 3 && g_log[1] == 2 && g_log[2] == 1 && g_base == 0)) { std::printf("006fb460_TEST FAIL\n"); return 1; }
    std::printf("006fb460_TEST PASS\n");
    return 0;
}