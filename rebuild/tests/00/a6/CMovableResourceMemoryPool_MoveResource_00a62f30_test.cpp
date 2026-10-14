#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_reason = 0; static void* g_src = 0; static void* g_dst = 0; static unsigned long g_size = 0; struct CRes { virtual void v000() { } virtual void OnMoved(long r, void* s, void* d, unsigned long n) { g_reason = r; g_src = s; g_dst = d; g_size = n; } };
struct CPool { virtual void v000() { } virtual void MoveResource(void* res, long r, void* s, void* d, unsigned long n) { std::memmove(d, s, n); if (res) ((CRes*)res)->OnMoved(r, s, d, n); } };

int main() {
    char src[8] = "abcdefg", dst[8] = {0}; CPool p; CRes r; p.MoveResource(&r, 3, src, dst, 4); if (std::memcmp(dst, "abcd", 4) || g_reason != 3 || g_src != src || g_dst != dst || g_size != 4) { std::printf("00a62f30_TEST FAIL\n"); return 1; } g_reason = 0; p.MoveResource(0, 9, src, dst, 1); if (g_reason != 0) { std::printf("00a62f30_TEST FAIL\n"); return 1; }
    std::printf("00a62f30_TEST PASS\n"); return 0;
}