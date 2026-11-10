// Behaviour test for CVtable_012bf0c8::StartBlend @ 0x00C77410: sets the three blend fields and nothing else.
#include <cstdio>
#include <cstring>
class CVtable_012bf0c8 {
public:
    void StartBlend();
    void StopBlend();
    void*         m_pSettings;
    void*         m_pTargetSettings;
    void*         m_pPendingSettings;
    unsigned long m_settingsSize;
    bool          m_blending;
    long          m_blendStep;
    long          m_blendCountdown;
    long          m_blendSteps;
    bool          m_dirty;
};
void CVtable_012bf0c8::StartBlend()
{
    m_blendStep = 0;
    m_blending = true;
    m_blendCountdown = 0x200;
}
int main()
{
    CVtable_012bf0c8 c;
    std::memset(&c, 0x5A, sizeof(c));
    c.m_blending = false; c.m_blendSteps = 7; c.m_dirty = true;
    c.StartBlend();
    bool ok = c.m_blending == true && c.m_blendStep == 0 && c.m_blendCountdown == 0x200 && c.m_blendSteps == 7 && c.m_dirty;
    if (!ok) { std::printf("00c77410_TEST FAIL\n"); return 1; }
    std::printf("00c77410_TEST PASS\n");
    return 0;
}