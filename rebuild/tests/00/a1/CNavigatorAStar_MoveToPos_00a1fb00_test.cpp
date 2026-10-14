#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_args[13]; static long g_n = 0; static float g_eps = 1e-4f;
struct CAStar { void Goal(long a, long b, long c, long d, long e, long f, long g, long h, long i, long j, long k, long l, long m) { long v[13] = {a, b, c, d, e, f, g, h, i, j, k, l, m}; std::memcpy(g_args, v, sizeof v); g_n = 13; }
  void MoveToPos(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11) { Goal(a1, a2, *(long*)&g_eps, a3, a4, 0, a5, a6, a7, a8, a9, a10, a11); }
  void MoveToPosProximity(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11, long a12) { Goal(a1, a2, a3, a4, a5, 0, a6, a7, a8, a9, a10, a11, a12); }
  void MoveToNavLineOfSightProximity(long a1, long a2, long a3, long a4, long a5, long a6, long a7, long a8, long a9, long a10, long a11, long a12) { Goal(a1, a2, a3, a4, a5, 1, a6, a7, a8, a9, a10, a11, a12); } };

int main() {
    CAStar s; s.MoveToPos(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11); long exp[13] = {1, 2, *(long*)&g_eps, 3, 4, 0, 5, 6, 7, 8, 9, 10, 11}; if (g_n != 13 || std::memcmp(g_args, exp, sizeof exp)) { std::printf("00a1fb00_TEST FAIL\n"); return 1; }
    std::printf("00a1fb00_TEST PASS\n"); return 0;
}