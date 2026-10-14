#include <cstdio>
static int g_locks = 0, g_unlocks = 0; static void* g_buf = 0; static long g_flags = -1; static int g_renders = 0;
struct CProfileLock { void* m_pSection; long m_entryTick; CProfileLock(void* s) : m_pSection(s) { ++g_locks; } ~CProfileLock() { ++g_unlocks; } };
static unsigned char g_section[1];
static void RenderBuffer(void* b, long f) { ++g_renders; g_buf = b; g_flags = f; }
struct CRenderQueue { unsigned char _pad[0x0c]; void* m_pBuffer; long m_pendingCount; unsigned char _pad14[0x10]; unsigned char m_suspended;
    void Flush() { CProfileLock lock(g_section); m_pendingCount = 0; if (!m_suspended) RenderBuffer(m_pBuffer, 0); } };
int main() { int buf; CRenderQueue q; q.m_pBuffer = &buf; q.m_pendingCount = 5; q.m_suspended = 0; q.Flush();
    if (q.m_pendingCount != 0 || g_renders != 1 || g_buf != &buf || g_flags != 0 || g_locks != 1 || g_unlocks != 1) { std::printf("00c279a0_TEST FAIL\n"); return 1; }
    q.m_suspended = 1; q.m_pendingCount = 2; q.Flush(); if (q.m_pendingCount != 0 || g_renders != 1 || g_locks != 2 || g_unlocks != 2) { std::printf("00c279a0_TEST FAIL\n"); return 1; } std::printf("00c279a0_TEST PASS\n"); return 0; }