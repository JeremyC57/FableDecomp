// Behaviour test for 009cbaa0: CBankFile::SplitSymbol -- *pName gets the whole symbol (as retail), *pSuffix the bracketed text or "".
#include <cstdio>
#include <cstring>
struct TStr {
    char s[64];
    TStr() { s[0] = 0; }
    TStr(const char* t) { std::strcpy(s, t); }
    long FindCharPos(char c) const { const char* p = std::strchr(s, c); return p ? (long)(p - s) : -1; }
    TStr GetSubString(unsigned long start, unsigned long end) const { TStr r; std::strncpy(r.s, s + start, end - start); r.s[end - start] = 0; return r; }
};
static void SplitSymbol(const TStr& symbol, TStr* pName, TStr* pSuffix)
{
    long open = symbol.FindCharPos('['); long close = symbol.FindCharPos(']');
    TStr name, suffix;
    if (open != -1 && close != -1) { name = symbol.GetSubString(0, open); suffix = symbol.GetSubString(open + 1, close); }
    else { name = symbol; suffix = ""; }
    if (pName) *pName = symbol;
    if (pSuffix) *pSuffix = suffix;
}
int main()
{
    TStr n, s; SplitSymbol(TStr("MESH[LOD1]"), &n, &s);
    if (std::strcmp(n.s, "MESH[LOD1]") || std::strcmp(s.s, "LOD1")) { std::printf("009cbaa0_TEST FAIL\n"); return 1; }
    SplitSymbol(TStr("PLAIN"), 0, &s);
    if (s.s[0] != 0) { std::printf("009cbaa0_TEST FAIL\n"); return 1; }
    std::printf("009cbaa0_TEST PASS\n");
    return 0;
}
