#include <cstdio>
static int g_count = -1; static unsigned char g_fill = 1;
struct Base { virtual void v00() {} Base() {} };
struct Al { };
struct BA { unsigned char* f; unsigned char* l; unsigned char* e; BA(unsigned long n, const unsigned char& v, const Al& = Al()) : f(0), l(0), e(0) { g_count = (int)n; g_fill = v; } };
struct WS { void* p; WS() : p(0) {} };
struct CP { void* a; void* b; CP() : a(0), b(0) {} };
struct F : Base { virtual void v00() {} long pos; long start; BA buf; WS path; CP disk; bool opened; bool valid;
    F(long n) : Base(), start(0), buf(n, 0), path(), disk(), opened(false), valid(false) {} };
int main()
{
    F f(4096);
    if (sizeof(F) != 0x28 || g_count != 4096 || g_fill != 0 || f.start || f.path.p || f.disk.a || f.opened || f.valid) { std::printf("00a5b590_TEST FAIL\n"); return 1; }
    std::printf("00a5b590_TEST PASS\n");
    return 0;
}
