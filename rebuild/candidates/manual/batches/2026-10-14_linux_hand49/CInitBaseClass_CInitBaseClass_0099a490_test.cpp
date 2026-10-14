#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static long g_count = 0; struct CBaseClass { virtual ~CBaseClass() { std::strcat(g_seq, "B"); } }; struct CInitBaseClass : CBaseClass { bool m_initialised; CInitBaseClass() : m_initialised(false) { } virtual ~CInitBaseClass() { if (m_initialised == true) { --g_count; m_initialised = false; } } };

int main() {
    g_count = 5; CInitBaseClass* a = new CInitBaseClass; a->m_initialised = true; delete a; if (g_count != 4 || std::strcmp(g_seq, "B")) { std::printf("0099a490_TEST FAIL\n"); return 1; } g_seq[0] = 0; CInitBaseClass* b = new CInitBaseClass; delete b; if (g_count != 4 || std::strcmp(g_seq, "B")) { std::printf("0099a490_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; { CInitBaseClass c; c.m_initialised = true; } if (g_count != 3 || std::strcmp(g_seq, "B")) { std::printf("0099a490_TEST FAIL\n"); return 1; }
    std::printf("0099a490_TEST PASS\n"); return 0;
}
