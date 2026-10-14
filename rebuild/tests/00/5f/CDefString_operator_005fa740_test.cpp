#include <cstdio>
#include <string.h>
static const char* g_strings[] = { "alpha", "beta" };
struct CDefStringTable { const char* GetStringAsCharArray(long id) { return g_strings[id]; } };
CDefStringTable g_DefStringTable;
struct CDefString { long TablePos; bool operator!=(const char* text) const { return strcmp(g_DefStringTable.GetStringAsCharArray(TablePos), text) != 0; } };
int main()
{
    CDefString a; a.TablePos = 0;
    if ((a != "alpha") || !(a != "beta") || !(a != "alph") || !(a != "alphas")) { std::printf("005fa740_TEST FAIL\n"); return 1; }
    std::printf("005fa740_TEST PASS\n"); return 0;
}