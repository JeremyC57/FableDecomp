#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CWorldMap { long m_count; long GetNoMaps() { return m_count; } }; static long g_resizeN = 0, g_resizeValue = 0, g_args[4];
struct CRetailVector_long { long* first; long* last; long* end; void Resize(unsigned long n, const long& value) { std::strcat(g_seq, "R"); g_resizeN = n; g_resizeValue = value; last = first + n; } unsigned long Size() { return last - first; } };
static long g_stamp = 0; static CRetailVector_long g_vec;
struct CSegment { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void CollectCollidedTriangles(long a, long b, long c, long d) { std::strcat(g_seq, "C"); g_args[0] = a; g_args[1] = b; g_args[2] = c; g_args[3] = d; }
    void GetCollidedWithTriangles(CWorldMap* map, long a, long b, long c, long d) { long count = map->GetNoMaps(); if (g_vec.Size() < (unsigned long)count) g_vec.Resize(count, g_stamp); ++g_stamp; CollectCollidedTriangles(a, b, c, d); } };

int main() {
    CSegment s; CWorldMap m; m.m_count = 3; long stamps[8]; g_vec.first = stamps; g_vec.last = stamps + 1; g_stamp = 10;
    s.GetCollidedWithTriangles(&m, 1, 2, 3, 4); if (std::strcmp(g_seq, "RC") || g_resizeN != 3 || g_resizeValue != 10 || g_stamp != 11 || g_args[0] != 1 || g_args[3] != 4) { std::printf("00a86ea0_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; g_vec.last = stamps + 3; s.GetCollidedWithTriangles(&m, 5, 6, 7, 8); if (std::strcmp(g_seq, "C") || g_stamp != 12 || g_args[0] != 5) { std::printf("00a86ea0_TEST FAIL\n"); return 1; }
    std::printf("00a86ea0_TEST PASS\n"); return 0;
}