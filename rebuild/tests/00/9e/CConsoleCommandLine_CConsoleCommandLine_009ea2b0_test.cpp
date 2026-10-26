#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_base = 0; static const char* g_text = 0; static long g_len = 0; namespace N_0099a2d0 { struct CBase { virtual void v00() { } CBase() { ++g_base; } }; } using N_0099a2d0::CBase; class CCharString { public: void* m_p; CCharString(const char* t, long n) : m_p(0) { g_text = t; g_len = n; } };
class CConsoleCommandLine : public CBase { public: virtual void v00() { } CCharString m_line; bool UserEntry; long CursorPosition; CConsoleCommandLine() : CBase(), m_line("", -1), UserEntry(false), CursorPosition(0) { } };

int main() {
    CConsoleCommandLine c; if (g_base != 1 || std::strcmp(g_text, "") || g_len != -1 || c.UserEntry || c.CursorPosition) { std::printf("009ea2b0_TEST FAIL\n"); return 1; }
    std::printf("009ea2b0_TEST PASS\n"); return 0;
}