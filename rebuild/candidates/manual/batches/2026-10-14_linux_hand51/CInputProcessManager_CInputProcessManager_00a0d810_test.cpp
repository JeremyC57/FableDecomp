#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CBase { virtual void v00() { } ~CBase() { std::strcat(g_seq, "B"); } }; struct CInputProcessManager : CBase { virtual void v00() { } void RemoveAllInputProcesses() { std::strcat(g_seq, "R"); } ~CInputProcessManager() { RemoveAllInputProcesses(); } };

int main() {
    { CInputProcessManager m; } if (std::strcmp(g_seq, "RB")) { std::printf("00a0d810_TEST FAIL\n"); return 1; }
    std::printf("00a0d810_TEST PASS\n"); return 0;
}
