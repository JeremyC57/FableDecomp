// Behaviour test for 00c449c0: C2DAudioObject::Open -- base open, clear the mix record, maybe keep a software voice, build speaker tables
#include <cstdio>
#include <string.h>

struct SVoiceCaps { unsigned int m_id, m_available, m_objectCaps, m_formatCaps, m_words[2]; };
static SVoiceCaps g_voiceCaps[4];
static int g_error = 0, g_allocResult = 0, g_allocs = 0, g_closes = 0, g_maps = 0;
static bool g_baseOpen = true;
struct CVoice { int id; };
static CVoice g_voice;
struct S2DMixParams { int m_value0, m_value1, m_level, m_reserved[4]; };

struct C2DAudioObject {
    unsigned int m_caps, m_flags;
    CVoice* m_pVoice;
    S2DMixParams m_mix;
    bool BaseOpen(unsigned int caps) { m_caps = caps; m_flags = 0x100000; return g_baseOpen; }
    void Close() { ++g_closes; }
    bool InitSpeakerMaps() { ++g_maps; return true; }
    int Allocate(CVoice** ppVoice) { ++g_allocs; if (g_allocResult) return g_allocResult; *ppVoice = &g_voice; return 0; }
    bool Open(unsigned int caps)
    {
        if (!BaseOpen(caps))
            return false;
        memset(&m_mix, 0, sizeof(m_mix));
        m_mix.m_value0 = 0;
        m_mix.m_value1 = 0;
        if (caps & 0x80)
            m_mix.m_level = 0;
        else
            m_mix.m_level = -10000;
        if ((!g_voiceCaps[0].m_available || (caps & ~g_voiceCaps[0].m_objectCaps)) && g_voiceCaps[2].m_available == 0xFFFFFFFFu) {
            int result = Allocate(&m_pVoice);
            if (result) {
                g_error = result;
                Close();
                return false;
            }
            m_flags |= 0x20000;
        }
        return InitSpeakerMaps();
    }
};

int main()
{
    g_voiceCaps[0].m_available = 4; g_voiceCaps[0].m_objectCaps = 0x81;
    g_voiceCaps[2].m_available = 0xFFFFFFFFu;
    C2DAudioObject o; memset(&o, 0x5A, sizeof(o)); o.m_pVoice = 0;
    int ok = o.Open(0x81);                                    // hardware can serve: no voice kept
    ok &= o.m_mix.m_level == 0 && o.m_mix.m_reserved[3] == 0 && g_allocs == 0 && o.m_flags == 0x100000 && g_maps == 1;
    ok &= o.Open(0x2);                                        // hardware lacks cap 2, software unlimited
    ok &= o.m_mix.m_level == -10000 && g_allocs == 1 && o.m_pVoice == &g_voice && o.m_flags == 0x120000;
    g_voiceCaps[2].m_available = 8;
    ok &= o.Open(0x2) && g_allocs == 1;                       // software limited: no pre-allocation
    g_voiceCaps[2].m_available = 0xFFFFFFFFu; g_allocResult = -9;
    ok &= !o.Open(0x2) && g_error == -9 && g_closes == 1 && g_maps == 3;
    g_baseOpen = false;
    ok &= !o.Open(0x81) && g_maps == 3;
    if (!ok) { std::printf("00c449c0_TEST FAIL\n"); return 1; }
    std::printf("00c449c0_TEST PASS\n");
    return 0;
}