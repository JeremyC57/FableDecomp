// Behaviour test for 0x00C774F0 CVtable_012bf0c8::CommitAndProcess_00c774f0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>
struct Effect {
    void* m_pSettings; void* m_pTargetSettings; void* m_pPendingSettings; unsigned int m_settingsSize;
    bool m_blending; int m_blendStep; int m_blendCountdown; int m_blendSteps; bool m_dirty;
    int processed; bool result;
    bool Process(unsigned int count) { processed += (int)count; return result; }
    bool CommitAndProcess(unsigned int count)
    {
        if (m_dirty && !m_blending) {
            std::memcpy(m_pTargetSettings, m_pPendingSettings, m_settingsSize);
            m_dirty = false; m_blendStep = 0; m_blending = true; m_blendCountdown = 0x200;
        }
        bool r = Process(count);
        if (m_blending) {
            m_blendCountdown -= (int)count;
            if (m_blendCountdown <= 0) {
                if (++m_blendStep == m_blendSteps) { m_blendStep = -1; m_blending = false; m_blendCountdown = -1; }
                else m_blendCountdown = 0x200;
            }
        }
        return r;
    }
};
int main()
{
    char target[4] = {0, 0, 0, 0}, pending[4] = {1, 2, 3, 4};
    Effect e; std::memset(&e, 0, sizeof(e));
    e.m_pTargetSettings = target; e.m_pPendingSettings = pending; e.m_settingsSize = 4; e.m_blendSteps = 2; e.m_dirty = true; e.result = true;
    if (!e.CommitAndProcess(128) || target[3] != 4 || e.m_dirty || !e.m_blending || e.m_blendCountdown != 0x200 - 128) { std::printf("00c774f0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) e.CommitAndProcess(128);       // countdown reaches 0 -> step 1, countdown 0x200
    if (e.m_blendStep != 1 || e.m_blendCountdown != 0x200 || !e.m_blending) { std::printf("00c774f0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) e.CommitAndProcess(128);       // step 2 == steps -> blend over
    if (e.m_blending || e.m_blendStep != -1 || e.m_blendCountdown != -1 || e.processed != 8 * 128) { std::printf("00c774f0_TEST FAIL\n"); return 1; }
    std::printf("00c774f0_TEST PASS\n");
    return 0;
}