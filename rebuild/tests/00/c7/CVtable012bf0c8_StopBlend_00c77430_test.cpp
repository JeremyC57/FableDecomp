// Behaviour test for CVtable_012bf0c8::StopBlend @ 0x00C77430: sets the three blend fields and nothing else.
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
void CVtable_012bf0c8::StopBlend()
{
    m_blendStep = -1;
    m_blending = false;
    m_blendCountdown = -1;
}
int main()
{
    CVtable_012bf0c8 c;
    std::memset(&c, 0x5A, sizeof(c));
    c.m_blending = true; c.m_blendSteps = 7; c.m_dirty = true;
    c.StopBlend();
    bool ok = c.m_blending == false && c.m_blendStep == -1 && c.m_blendCountdown == -1 && c.m_blendSteps == 7 && c.m_dirty;
    if (!ok) { std::printf("00c77430_TEST FAIL\n"); return 1; }
    std::printf("00c77430_TEST PASS\n");
    return 0;
}