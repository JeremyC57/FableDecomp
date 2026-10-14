#include <cstdio>
#include <cstdlib>
static int g_freed = 0, g_deleted = 0; static void* g_buf = 0;
struct CBufferedReader { virtual void v00() { } unsigned char _pad04[0x38]; void* m_pBuffer; void FreeBuffer(void* b) { ++g_freed; g_buf = b; } virtual ~CBufferedReader() { if (m_pBuffer != 0) FreeBuffer(m_pBuffer); } void operator delete(void* p) { ++g_deleted; std::free(p); } };
int main() { int buf; CBufferedReader* r = new CBufferedReader; r->m_pBuffer = &buf; delete r; if (g_freed != 1 || g_buf != &buf || g_deleted != 1) { std::printf("00cab6d0_TEST FAIL\n"); return 1; } CBufferedReader* s = new CBufferedReader; s->m_pBuffer = 0; delete s; if (g_freed != 1) { std::printf("00cab6d0_TEST FAIL\n"); return 1; } std::printf("00cab6d0_TEST PASS\n"); return 0; }
