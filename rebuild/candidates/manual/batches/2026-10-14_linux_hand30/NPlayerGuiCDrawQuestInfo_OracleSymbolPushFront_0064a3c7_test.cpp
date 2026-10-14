#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
struct CCharString { const char* s; };
static long g_pushed[4]; static int g_npushed = 0;
struct CRetailLongList { void* head; void push_back(const long& v) { g_pushed[g_npushed++] = v; } void push_front(const long& v) { g_pushed[g_npushed++] = -v; } };
struct CDrawQuestInfo { unsigned char p[0x40]; CRetailLongList OracleSymbols; long GetScriptSpriteBySymbol(const CCharString& s) { return std::atoi(s.s); } void OracleSymbolPushFront(const CCharString& s) { long sprite = GetScriptSpriteBySymbol(s); if (sprite > 0) OracleSymbols.push_front(sprite); } };

int main() { CDrawQuestInfo q; CCharString a = {"7"}, b = {"0"}, c = {"-3"}; q.OracleSymbolPushFront(a); q.OracleSymbolPushFront(b); q.OracleSymbolPushFront(c);
    if (g_npushed != 1 || g_pushed[0] != -7) { std::printf("0064a3c7_TEST FAIL\n"); return 1; } std::printf("0064a3c7_TEST PASS\n"); return 0; }