#include <cstdio>
static int g_n = 0, g_b = 0, g_r = 0; static char g_buf[4];
struct D { char* p; D(char* b) : p(b) {} D& __fastcall operator<<(const char*) { ++g_n; return *this; } D& __fastcall operator<<(long) { ++g_n; return *this; } ~D() { ++g_r; } };
struct B { B() { ++g_b; } }; static char* g_p = g_buf; static void __fastcall Rep(const char*) { g_r += 10; } static void (__fastcall* g_f)(const char*) = Rep;
static long __stdcall H(void*, void*) { { D s(g_p); s << "a" << "b" << 7 << "c"; if (g_f) g_f(s.p); } B(); return 0; }
int main()
{
    if (H(0, 0) != 0 || g_n != 4 || g_b != 1 || g_r != 11) { std::printf("00c0a300_TEST FAIL\n"); return 1; }

    std::printf("00c0a300_TEST PASS\n");
    return 0;
}
