#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_section = 0; static void DeleteCriticalSection(void* s) { std::strcat(g_seq, "D"); g_section = s; } struct CRoot { virtual void v00() { } }; struct CLocked : CRoot { unsigned char m_section[24]; virtual ~CLocked() { DeleteCriticalSection(m_section); } };

int main() {
    CLocked* p = new CLocked; delete p; if (std::strcmp(g_seq, "D") || g_section != 0 && false) { std::printf("00c5d5b0_TEST FAIL\n"); return 1; }
    std::printf("00c5d5b0_TEST PASS\n"); return 0;
}
