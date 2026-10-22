#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
static void* g_discarded = 0; static long g_buffers = 0; struct CSub { virtual void v00() { } ~CSub() { std::strcat(g_seq, "S"); } }; struct CAsync { void* m_p; ~CAsync() { std::strcat(g_seq, "a"); g_discarded = this; } }; struct CLeaf : CSub { long m_4, m_8; CAsync m_asyncData; char* m_pBuffer; CLeaf() { m_pBuffer = new char; } virtual ~CLeaf() { delete m_pBuffer; std::strcat(g_seq, "b"); } };

int main() {
    CLeaf* p = new CLeaf; delete p; if (std::strcmp(g_seq, "baS")) { std::printf("00850670_TEST FAIL\n"); return 1; }
    std::printf("00850670_TEST PASS\n"); return 0;
}