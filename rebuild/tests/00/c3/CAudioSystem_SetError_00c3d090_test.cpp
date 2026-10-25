// Behaviour test for 00c3d090: CAudioSystem::SetError -- the first error is latched and reported once
#include <cstdio>
typedef void (__stdcall* TAudioErrorCallback)(long);
static long g_lastError = 0; static long g_seen = 0; static int g_calls = 0;
static void __stdcall cb(long code) { ++g_calls; g_seen = code; }
struct CAudioSystem {
    unsigned char _pad[0x7c]; TAudioErrorCallback m_pfnErrorCallback;
    void SetError(long code) { if (g_lastError == 0) { g_lastError = code; if (m_pfnErrorCallback) m_pfnErrorCallback(code); } }
};
int main()
{
    CAudioSystem s; s.m_pfnErrorCallback = 0;
    s.SetError(-6); if (g_lastError != -6 || g_calls) { std::printf("00c3d090_TEST FAIL\n"); return 1; }
    g_lastError = 0; s.m_pfnErrorCallback = cb; s.SetError(-14); if (g_lastError != -14 || g_calls != 1 || g_seen != -14) { std::printf("00c3d090_TEST FAIL\n"); return 1; }
    s.SetError(-15); if (g_lastError != -14 || g_calls != 1) { std::printf("00c3d090_TEST FAIL\n"); return 1; }
    std::printf("00c3d090_TEST PASS\n");
    return 0;
}
