// Behaviour test for 00c448d0: C2DAudioObject::InitSpeakerMaps -- one {speaker bit, level} entry per output speaker for each requested table
#include <cstdio>

static int g_speakerCount = 3;
static unsigned int g_speakerMask = 0x2C;      // speakers at bits 2, 3, 5
struct SSpeakerLevel { unsigned int m_speaker; float m_level; };
struct SSpeakerTable { int m_count; unsigned int m_reserved; };
struct C2DAudioObject {
    unsigned int m_caps;
    SSpeakerTable* m_pGains; SSpeakerTable* m_pLevels;
    SSpeakerTable m_gains, m_levels;
    SSpeakerLevel m_gainEntries[8], m_levelEntries[8];
    bool InitSpeakerMaps()
    {
        if (m_caps & 0x40000) {
            m_pGains = &m_gains;
            unsigned int bit = 1;
            for (int i = 0; i < g_speakerCount; ++i) {
                while (!(bit & g_speakerMask))
                    bit <<= 1;
                m_gainEntries[i].m_speaker = bit;
                m_gainEntries[i].m_level = 1.0f;
                bit <<= 1;
            }
            m_gains.m_count = g_speakerCount;
        }
        if (m_caps & 0x20000) {
            m_pLevels = &m_levels;
            unsigned int bit = 1;
            for (int i = 0; i < g_speakerCount; ++i) {
                while (!(bit & g_speakerMask))
                    bit <<= 1;
                m_levelEntries[i].m_speaker = bit;
                m_levelEntries[i].m_level = 0.0f;
                bit <<= 1;
            }
            m_levels.m_count = g_speakerCount;
        }
        return true;
    }
};

int main()
{
    C2DAudioObject o = { 0 };
    o.m_levels.m_count = 77;
    o.m_caps = 0x40000;
    int ok = o.InitSpeakerMaps();
    ok &= o.m_pGains == &o.m_gains && o.m_gains.m_count == 3 && o.m_pLevels == 0 && o.m_levels.m_count == 77;
    ok &= o.m_gainEntries[0].m_speaker == 4 && o.m_gainEntries[1].m_speaker == 8 && o.m_gainEntries[2].m_speaker == 0x20;
    ok &= o.m_gainEntries[2].m_level > 0.999f && o.m_gainEntries[2].m_level < 1.001f;
    o.m_caps = 0x20000; o.m_levelEntries[1].m_level = 5.0f;
    ok &= o.InitSpeakerMaps();
    ok &= o.m_pLevels == &o.m_levels && o.m_levels.m_count == 3 && o.m_levelEntries[1].m_speaker == 8;
    ok &= o.m_levelEntries[1].m_level > -0.001f && o.m_levelEntries[1].m_level < 0.001f;
    if (!ok) { std::printf("00c448d0_TEST FAIL\n"); return 1; }
    std::printf("00c448d0_TEST PASS\n");
    return 0;
}