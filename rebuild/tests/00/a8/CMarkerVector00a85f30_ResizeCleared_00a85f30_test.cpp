#include <cstdio>
static unsigned int g_count = 0; static int g_zero = 0, g_dtors = 0;
struct SMarker { long m_words[9]; unsigned char _pad24; unsigned char m_flag; unsigned char _pad26[2];
    SMarker() { for (int i = 0; i < 9; ++i) m_words[i] = 0; m_flag = 0; } ~SMarker() { ++g_dtors; } };
struct CMarkerVector { void resize(unsigned int c, const SMarker& v) { g_count = c; g_zero = 1; for (int i = 0; i < 9; ++i) if (v.m_words[i]) g_zero = 0; if (v.m_flag) g_zero = 0; }
    void ResizeCleared(unsigned int count) { SMarker blank; resize(count, blank); } };
int main() { CMarkerVector v; v.ResizeCleared(4); if (g_count != 4 || !g_zero || g_dtors != 1) { std::printf("00a85f30_TEST FAIL\n"); return 1; } std::printf("00a85f30_TEST PASS\n"); return 0; }