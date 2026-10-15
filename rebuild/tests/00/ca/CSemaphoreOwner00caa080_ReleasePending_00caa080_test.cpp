// Behaviour test for 00caa080 (CSemaphoreOwner_00caa080::ReleasePending): pending releases are handed to the semaphore once and the counter cleared; zero does nothing.
#include <cstdio>

static int   g_calls = 0;
static void* g_sem = 0;
static long  g_count = 0;

static int ReleaseSemaphoreStub(void* semaphore, long releaseCount, long* previousCount)
{
    ++g_calls; g_sem = semaphore; g_count = releaseCount;
    return previousCount == 0;
}

struct CSemaphoreOwner {
    void* m_hSemaphore;
    long  m_pendingReleases;
    void ReleasePending()
    {
        if (m_pendingReleases) {
            ReleaseSemaphoreStub(m_hSemaphore, m_pendingReleases, 0);
            m_pendingReleases = 0;
        }
    }
};

int main()
{
    long handle = 0;
    CSemaphoreOwner o; o.m_hSemaphore = &handle; o.m_pendingReleases = 0;
    o.ReleasePending();
    if (g_calls != 0) { std::printf("00caa080_TEST FAIL\n"); return 1; }
    o.m_pendingReleases = 3;
    o.ReleasePending();
    if (g_calls != 1 || g_sem != &handle || g_count != 3 || o.m_pendingReleases != 0) { std::printf("00caa080_TEST FAIL\n"); return 1; }
    std::printf("00caa080_TEST PASS\n");
    return 0;
}