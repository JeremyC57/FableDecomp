// Behaviour test for CSoftwareVoice::ApplyFilter (0x00C5AFC0): per-filter enable bits, the ramped kernel while
// 0xA0000 is set, the steady kernel otherwise, nothing for an index past 1.
#include <cstdio>
static int g_ramped[2], g_steady[2];
struct CVoiceFilter {
    int m_index;
    void Process(short*, unsigned int) { ++g_steady[m_index]; }
    void ProcessRamped(short*, unsigned int) { ++g_ramped[m_index]; }
};
struct CSoftwareVoice {
    unsigned int m_flags; CVoiceFilter m_filters[2];
    void ApplyFilter(short* samples, unsigned int frames, unsigned int index)
    {
        bool active;
        switch (index) {
        case 0: active = (m_flags & 0x60000) != 0; break;
        case 1: active = (m_flags & 0x180000) != 0; break;
        default: return;
        }
        if (!active) return;
        if (m_flags & 0xa0000) m_filters[index].ProcessRamped(samples, frames);
        else m_filters[index].Process(samples, frames);
    }
};
int main()
{
    short s[4] = { 0 };
    CSoftwareVoice v; v.m_filters[0].m_index = 0; v.m_filters[1].m_index = 1;
    v.m_flags = 0x40000; v.ApplyFilter(s, 4, 0); v.ApplyFilter(s, 4, 1); v.ApplyFilter(s, 4, 2);
    int ok = g_steady[0] == 1 && g_ramped[0] == 0 && g_steady[1] == 0 && g_ramped[1] == 0;
    v.m_flags = 0x100000 | 0x20000; v.ApplyFilter(s, 4, 0); v.ApplyFilter(s, 4, 1);   // 0x20000 enables 0 and ramps both
    ok = ok && g_ramped[0] == 1 && g_ramped[1] == 1 && g_steady[1] == 0;
    v.m_flags = 0x80000; v.ApplyFilter(s, 4, 1); v.ApplyFilter(s, 4, 0);
    ok = ok && g_ramped[1] == 2 && g_steady[0] == 1 && g_ramped[0] == 1;
    if (!ok) { std::printf("00c5afc0_TEST FAIL\n"); return 1; }
    std::printf("00c5afc0_TEST PASS\n");
    return 0;
}