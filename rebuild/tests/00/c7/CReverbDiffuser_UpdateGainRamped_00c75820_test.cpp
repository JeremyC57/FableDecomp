// Behaviour test for CReverbDiffuser::UpdateGainRamped (0x00C75820): the old tap is read scaled from rampLength/512
// (step -1/512), the new tap mixed in from 1 - rampLength/512 (step +1/512), both from (block start - tap) in the ring;
// the gain cross-fades the scratch block in place, the block is written back at (block start - line offset); the new
// tap delay is adopted only when the fade ends inside this block (count >= rampLength).
#include <cstdio>

struct CRingBuffer { float* m_data; unsigned int m_size; unsigned int m_pos; };
struct SGainParams { float m_master; float m_current[3]; float m_target[3]; };
struct SDiffuserParams { unsigned int m_taps[3]; SGainParams m_gains[3]; };
struct CReverbConfig { unsigned int m_00, m_channels, m_rate, m_0C, m_tapBase, m_tapSpacing; };

static unsigned int g_readPos, g_mixPos, g_writePos; static float g_readGain, g_readStep, g_mixGain, g_mixStep;
static int g_crossfade; static const float* g_written;

struct CDspOps {
    void ReadRingScaledRamp(float* dst, const CRingBuffer*, unsigned int pos, float gain, float step, unsigned int count)
    { for (unsigned int i = 0; i < count; ++i) dst[i] = 1.0f; g_readPos = pos; g_readGain = gain; g_readStep = step; }
    void MixRampFromRing(float*, const CRingBuffer*, unsigned int pos, float gain, float step, unsigned int)
    { g_mixPos = pos; g_mixGain = gain; g_mixStep = step; }
    void WriteRing(CRingBuffer*, unsigned int pos, const float* src, unsigned int) { g_writePos = pos; g_written = src; }
};
static CDspOps g_ops; static CDspOps* s_pOps = &g_ops;

struct CDspGain {
    void Crossfade(SGainParams*, const SGainParams*, const float* source, float* block, int mode, unsigned int, unsigned int)
    { if (source == block && mode == 1) ++g_crossfade; }
};

struct CReverbDiffuser {
    CDspGain m_gains[3];
    CReverbConfig* m_pConfig;
    void UpdateGainRamped(unsigned int index, SDiffuserParams* params, const SDiffuserParams* next, CRingBuffer* history,
                          unsigned int count, unsigned int rampLength)
    {
        float block[128];
        unsigned int start = history->m_pos >= count ? history->m_pos - count : history->m_size - count + history->m_pos;
        float ramp = (float)rampLength;
        float fade = ramp * (1.0f / 512.0f);
        float rise = 1.0f - fade;
        unsigned int tap = params->m_taps[index];
        s_pOps->ReadRingScaledRamp(block, history, start >= tap ? start - tap : history->m_size - tap + start, fade,
                                   -1.0f / 512.0f, count);
        tap = next->m_taps[index];
        s_pOps->MixRampFromRing(block, history, start >= tap ? start - tap : history->m_size - tap + start, rise,
                                1.0f / 512.0f, count);
        m_gains[index].Crossfade(&params->m_gains[index], &next->m_gains[index], block, block, 1, count, rampLength);
        unsigned int offset = (index + 1) * m_pConfig->m_tapSpacing / 4 + m_pConfig->m_tapBase;
        s_pOps->WriteRing(history, start >= offset ? start - offset : history->m_size - offset + start, block, count);
        if (count >= rampLength)
            params->m_taps[index] = next->m_taps[index];
    }
};

int main()
{
    int ok = 1;
    static float ring[1000];
    CReverbConfig cfg = { 0, 2, 44100, 0, 10, 20 };
    CReverbDiffuser d; d.m_pConfig = &cfg;
    SDiffuserParams p, n; p.m_taps[1] = 40; n.m_taps[1] = 70;
    CRingBuffer r = { ring, 1000, 600 };
    d.UpdateGainRamped(1, &p, &n, &r, 128, 256);               // start 472
    ok &= g_readPos == 432 && g_mixPos == 402 && g_writePos == 472 - 20 && g_crossfade == 1;
    ok &= g_readGain == 0.5f && g_readStep == -1.0f / 512.0f && g_mixGain == 0.5f && g_mixStep == 1.0f / 512.0f;
    ok &= p.m_taps[1] == 40;                                    // fade not finished
    r.m_pos = 50;                                               // start 922 (wrapped)
    d.UpdateGainRamped(1, &p, &n, &r, 128, 128);
    ok &= g_readPos == 882 && g_mixPos == 852 && g_writePos == 902 && g_readGain == 0.25f && g_mixGain == 0.75f;
    ok &= p.m_taps[1] == 70 && g_crossfade == 2;                // fade ends in this block: adopt the new tap
    r.m_pos = 10;                                               // start 882; a tap longer than the start wraps to the ring end
    p.m_taps[1] = 900; n.m_taps[1] = 990;
    d.UpdateGainRamped(1, &p, &n, &r, 128, 512);
    ok &= g_readPos == 982 && g_mixPos == 892 && g_readGain == 1.0f && g_mixGain == 0.0f && p.m_taps[1] == 900;
    if (!ok) { std::printf("00c75820_TEST FAIL\n"); return 1; }
    std::printf("00c75820_TEST PASS\n");
    return 0;
}