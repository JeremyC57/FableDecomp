#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CSuspendable { virtual ~CSuspendable() { std::strcat(g_seq, "D"); } };

int main() {
    CSuspendable* p = new CSuspendable; delete p; if (std::strcmp(g_seq, "D")) { std::printf("00a44a40_TEST FAIL\n"); return 1; }
    std::printf("00a44a40_TEST PASS\n"); return 0;
}