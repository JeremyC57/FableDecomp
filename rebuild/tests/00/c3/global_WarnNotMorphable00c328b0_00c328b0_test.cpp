#include <cstdio>
#include <new>
static int g_dtor = 0, g_app = 0, g_num = 0; static void* g_sunk = 0; struct Ln { void* b; Ln(void* B) : b(B) {} ~Ln() { ++g_dtor; } Ln* Append(const char* t) { ++g_app; return this; } Ln* AppendNumber(long v) { g_num = (int)v; return this; } };
typedef void (*PF)(void*); static void Sink(void* b) { g_sunk = b; } static void* g_buf = (void*)&g_app; static PF g_pfn = 0;
static void Warn(void* unused) { Ln line(g_buf); line.Append(".\\PKCTRArray.cpp")->Append("(")->AppendNumber(24)->Append(") : ")->Append("Warning: This object is not morphable")->Append("\n"); if (g_pfn) g_pfn(line.b); }
static bool Check() { Warn(0); if (g_sunk) return false; g_pfn = Sink; Warn(0); return g_app == 10 && g_num == 24 && g_sunk == g_buf && g_dtor == 2; }

int main()
{
    if (!Check()) { std::printf("00c328b0_TEST FAIL\n"); return 1; }
    std::printf("00c328b0_TEST PASS\n");
    return 0;
}