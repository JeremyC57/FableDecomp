#include <cstdio>
#include <cstring>
// CVtable_012bf0c8::SetSettings: immediate -> current block; else pending block + dirty flag.
struct Settings {
    void* m_pSettings; void* m_pTargetSettings; void* m_pPendingSettings; unsigned int m_settingsSize; bool m_dirty;
    void SetSettings(const void* settings, bool immediate)
    {
        if (immediate) {
            std::memcpy(m_pSettings, settings, m_settingsSize);
        } else {
            std::memcpy(m_pPendingSettings, settings, m_settingsSize);
            m_dirty = true;
        }
    }
};
int main()
{
    char cur[8] = "aaaaaaa", tgt[8] = "bbbbbbb", pend[8] = "ccccccc";
    Settings s; s.m_pSettings = cur; s.m_pTargetSettings = tgt; s.m_pPendingSettings = pend; s.m_settingsSize = 5; s.m_dirty = false;
    s.SetSettings("12345", true);
    if (std::memcmp(cur, "12345aa", 7) != 0 || s.m_dirty || std::memcmp(pend, "ccccccc", 7) != 0) { std::printf("00c774a0_TEST FAIL\n"); return 1; }
    s.SetSettings("xyzwv", false);
    if (std::memcmp(pend, "xyzwvcc", 7) != 0 || !s.m_dirty || std::memcmp(cur, "12345aa", 7) != 0) { std::printf("00c774a0_TEST FAIL\n"); return 1; }
    std::printf("00c774a0_TEST PASS\n");
    return 0;
}