#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_cleared = 0; static void* g_freed = 0;
static void FreeBlock_00c3c370(void* b) { g_freed = b; }
struct CMarkedBuffer_00c44550 { void* m_pBlock; void* m_pCursor; void* m_pBuffer; void ClearMarks() { ++g_cleared; }
    void Release() { if (m_pBuffer) { ClearMarks(); FreeBlock_00c3c370(m_pBlock); m_pBlock = 0; m_pCursor = 0; m_pBuffer = 0; } } };
int main() {
    CMarkedBuffer_00c44550 b; int blk = 0; b.m_pBlock = &blk; b.m_pCursor = &blk; b.m_pBuffer = 0; b.Release(); if (g_cleared || g_freed || b.m_pBlock != &blk) { std::printf("00c44550_TEST FAIL\n"); return 1; }
    b.m_pBuffer = &blk; b.Release(); if (g_cleared != 1 || g_freed != &blk || b.m_pBlock || b.m_pCursor || b.m_pBuffer) { std::printf("00c44550_TEST FAIL\n"); return 1; }
    std::printf("00c44550_TEST PASS\n"); return 0;
}
