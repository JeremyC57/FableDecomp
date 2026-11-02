// Behaviour test for 0x00C77470 CVtable_012bf0c8::ReleaseBlock(): with a settings block it frees +4 once through the libpk
// free hook and clears the three block pointers; with no block it frees nothing and leaves the other pointers alone.
#include <cstdio>

static int g_freeCalls = 0;
static void* g_lastFreed = 0;

class CPKObject {
public:
    static void operator delete(void* p) { ++g_freeCalls; g_lastFreed = p; }
};

class CVtable_012bf0c8 {
public:
    void ReleaseBlock();
    void* m_pSettings;
    void* m_pTargetSettings;
    void* m_pPendingSettings;
    unsigned int m_settingsSize;
};

void CVtable_012bf0c8::ReleaseBlock()
{
    if (m_pSettings) {
        CPKObject::operator delete(m_pSettings);
        m_pSettings = 0;
        m_pTargetSettings = 0;
        m_pPendingSettings = 0;
    }
}

int main()
{
    char block[48];
    CVtable_012bf0c8 s;
    s.m_pSettings = block; s.m_pTargetSettings = block + 16; s.m_pPendingSettings = block + 32; s.m_settingsSize = 16;
    s.ReleaseBlock();
    if (g_freeCalls != 1 || g_lastFreed != block || s.m_pSettings || s.m_pTargetSettings || s.m_pPendingSettings || s.m_settingsSize != 16) {
        std::printf("00c77470_TEST FAIL\n"); return 1;
    }
    CVtable_012bf0c8 e;
    e.m_pSettings = 0; e.m_pTargetSettings = block; e.m_pPendingSettings = block; e.m_settingsSize = 4;
    e.ReleaseBlock();
    if (g_freeCalls != 1 || e.m_pTargetSettings != block || e.m_pPendingSettings != block) { std::printf("00c77470_TEST FAIL\n"); return 1; }
    std::printf("00c77470_TEST PASS\n");
    return 0;
}