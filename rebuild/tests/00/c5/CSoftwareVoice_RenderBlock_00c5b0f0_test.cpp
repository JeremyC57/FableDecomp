// Behaviour test for 0x00C5B0F0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct CSoftwareMixer { int id; };
struct CCodaSystem { CSoftwareMixer* m_pMixer; };
static CSoftwareMixer g_mixer; static CCodaSystem g_system; CCodaSystem* g_pCodaSystem = &g_system;
struct CAudioRingBuffer { unsigned int m_channels; unsigned int committed; void CommitRead(unsigned int f) { committed += f; } };
struct CSoftwareVoice {
    CAudioRingBuffer* m_pSource; int mixCalls; CSoftwareMixer* mixArg; unsigned int targets[8]; unsigned int sources[8]; int renders;
    virtual void MixBlock(CSoftwareMixer* mixer, CAudioRingBuffer* ring, unsigned int channels) { mixCalls++; mixArg = mixer; }
    virtual void RenderChannel(CAudioRingBuffer* ring, unsigned int channel, unsigned int target) { sources[renders] = channel; targets[renders] = target; renders++; }
    virtual ~CSoftwareVoice() {}
    void RenderBlock(CAudioRingBuffer* ring, unsigned int channels, bool mix)
    {
        if (mix)
            MixBlock(g_pCodaSystem->m_pMixer, ring, channels);
        for (unsigned int i = 0; i < m_pSource->m_channels; ++i)
            RenderChannel(ring, i, channels == 1 ? 0 : i);
        ring->CommitRead(0x80);
    } };
int main()
{
    g_system.m_pMixer = &g_mixer;
    CAudioRingBuffer src; src.m_channels = 2; src.committed = 0;
    CAudioRingBuffer out; out.m_channels = 6; out.committed = 0;
    CSoftwareVoice v; v.m_pSource = &src; v.mixCalls = 0; v.renders = 0; v.mixArg = 0;
    v.RenderBlock(&out, 1, true);
    if (v.mixCalls != 1 || v.mixArg != &g_mixer || v.renders != 2 || v.targets[0] != 0 || v.targets[1] != 0 || v.sources[1] != 1 || out.committed != 0x80) { std::printf("00c5b0f0_TEST FAIL\n"); return 1; }
    v.renders = 0;
    v.RenderBlock(&out, 2, false);
    if (v.mixCalls != 1 || v.renders != 2 || v.targets[1] != 1 || out.committed != 0x100 || src.committed != 0) { std::printf("00c5b0f0_TEST FAIL\n"); return 1; }
    std::printf("00c5b0f0_TEST PASS\n");
    return 0;
}