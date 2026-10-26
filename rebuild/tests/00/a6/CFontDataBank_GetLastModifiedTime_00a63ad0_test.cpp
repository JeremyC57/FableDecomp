#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_args[7];
class CCharString { public: int x; };
class CDateAndTime { public: long m_year; unsigned char _body[12]; CDateAndTime(long y, long mo, long d, long h, long mi, long s, long ms) : m_year(y) { g_args[0] = y; g_args[1] = mo; g_args[2] = d; g_args[3] = h; g_args[4] = mi; g_args[5] = s; g_args[6] = ms; } };
class CFontDataBank { public: virtual void v00() { } CDateAndTime Get(const CCharString& n) { return GetLastModifiedTime(n); } protected: virtual CDateAndTime GetLastModifiedTime(const CCharString&) { return CDateAndTime(1, 1, 1, 1, 1, 0, 0); } };
int main() {
    CFontDataBank b; CCharString s; CDateAndTime t = b.Get(s); if (g_args[0] != 1 || g_args[4] != 1 || g_args[5] != 0 || g_args[6] != 0 || t.m_year != 1) { std::printf("00a63ad0_TEST FAIL\n"); return 1; }
    std::printf("00a63ad0_TEST PASS\n"); return 0;
}