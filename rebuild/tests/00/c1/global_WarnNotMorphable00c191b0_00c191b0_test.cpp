#include <cstdio>
#include <new>
#include <cstring>
static char g_buf[256]; static int g_d = 0; static void* g_sunk = 0; struct Ll { void* b; Ll(void* x) : b(x) { ((char*)b)[0] = 0; } ~Ll() { ++g_d; } Ll* Append(const char* t) { std::strcat((char*)b, t); return this; } Ll* AppendNumber(long v) { char n[16]; std::sprintf(n, "%ld", v); std::strcat((char*)b, n); return this; } }; typedef void (*Sink)(void*); static Sink g_sink = 0; static void Sk(void* b) { g_sunk = b; }
static void W(void* u) { Ll l(g_buf); l.Append(".\\PKCTRRedblackTree.cpp")->Append("(")->AppendNumber(18)->Append(") : ")->Append("Warning: This object is not morphable")->Append("\n"); if (g_sink) g_sink(l.b); }
static bool Check() { W(0); if (g_sunk || g_d != 1) return false; g_sink = Sk; W(0); return g_sunk == g_buf && g_d == 2 && std::strcmp(g_buf, ".\\PKCTRRedblackTree.cpp(18) : Warning: This object is not morphable\n") == 0; }

int main()
{
    if (!Check()) { std::printf("00c191b0_TEST FAIL\n"); return 1; }
    std::printf("00c191b0_TEST PASS\n");
    return 0;
}