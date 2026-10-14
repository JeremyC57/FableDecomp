#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_set = 0; static void* g_events[4]; static int SetEvent(void* e) { g_events[g_set++] = e; return 1; }
struct SInstallBuffer_009c3330 { long m_0; bool m_inUse; bool m_pending; };
struct CFileInstaller { unsigned char _pad[0x30]; long m_freeBuffers; long m_pendingBuffers; unsigned char _pad38[0x4c]; void* m_hBufferFreedEvent; void* m_hWakeEvent;
    void FreeBuffer(SInstallBuffer_009c3330* b) { if (b->m_inUse) { b->m_inUse = false; ++m_freeBuffers; m_pendingBuffers -= (b->m_pending ? 1 : 0); SetEvent(m_hBufferFreedEvent); SetEvent(m_hWakeEvent); } } };
int main() {
    CFileInstaller f; std::memset(&f, 0, sizeof f); f.m_hBufferFreedEvent = (void*)1; f.m_hWakeEvent = (void*)2; f.m_pendingBuffers = 5; SInstallBuffer_009c3330 b; b.m_inUse = true; b.m_pending = true;
    f.FreeBuffer(&b); if (b.m_inUse || f.m_freeBuffers != 1 || f.m_pendingBuffers != 4 || g_set != 2 || g_events[0] != (void*)1 || g_events[1] != (void*)2) { std::printf("009c3330_TEST FAIL\n"); return 1; }
    f.FreeBuffer(&b); if (f.m_freeBuffers != 1 || g_set != 2) { std::printf("009c3330_TEST FAIL\n"); return 1; }
    b.m_inUse = true; b.m_pending = false; f.FreeBuffer(&b); if (f.m_freeBuffers != 2 || f.m_pendingBuffers != 4 || g_set != 4) { std::printf("009c3330_TEST FAIL\n"); return 1; }
    std::printf("009c3330_TEST PASS\n"); return 0;
}