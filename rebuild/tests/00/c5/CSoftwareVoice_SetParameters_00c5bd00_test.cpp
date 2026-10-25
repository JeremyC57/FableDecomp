// Behaviour test for CSoftwareVoice::SetParameters (0x00C5BD00): the capability mask (requested flags, default format
// 0x800 / 0x400 / 2 when none of 0xE02 is requested, +4, +8 with an envelope, +0x10 with a pitch envelope) and the
// restarts it performs (envelope, pitch envelope inline, vibrato, pitch LFO inline).
#include <cstdio>
static long SecondsToMixBlocks(float seconds) { return (long)(seconds * 10.0f); }
struct SEnvelopeParams { float delay, attack, hold, decay, sustain, release; };
struct SVibratoParams { float delay, rate, depth; };
struct SVoiceDesc { unsigned int m_flags, m_filter0, m_filter1; const SEnvelopeParams* m_pEnvelope;
                    const SEnvelopeParams* m_pPitchEnvelope; const SVibratoParams* m_pVibrato; const SVibratoParams* m_pModulation; };
struct CCodaBuffer { unsigned int m_formatFlags; };
static int g_env, g_vib, g_mod;
struct CSoftwareVoice {
    unsigned int m_flags; float m_pitchEnvLevel, m_pitchEnvScale; int m_pitchEnvCounter, m_modulationCounter;
    void StartEnvelope(const SEnvelopeParams*) { ++g_env; }
    void StartVibrato(const SVibratoParams*) { ++g_vib; }
    void SetModulation(const SVibratoParams*) { ++g_mod; }
    void StartPitchEnvelope(const SEnvelopeParams* env)
    { m_flags &= ~0x7e20u; if (env) m_pitchEnvCounter = SecondsToMixBlocks(env->delay); m_pitchEnvLevel = 1.0f; m_pitchEnvScale = 1.0f; }
    void StartModulation(const SVibratoParams* mod)
    { SetModulation(mod); m_flags &= ~0x18000u; if (mod) m_modulationCounter = SecondsToMixBlocks(mod->delay); }
    unsigned int SetParameters(CCodaBuffer* buffer, const SVoiceDesc* desc)
    {
        unsigned int format = buffer->m_formatFlags;
        unsigned int caps = desc->m_flags;
        if (!(caps & 0xe02)) {
            if (format & 0x40000) caps |= 0x800;
            else if (format & 0x20000) caps |= 0x400;
            else caps |= 2;
        }
        caps |= 4;
        StartEnvelope(desc->m_pEnvelope);
        if (desc->m_pEnvelope) caps |= 8;
        StartPitchEnvelope(desc->m_pPitchEnvelope);
        if (desc->m_pPitchEnvelope) caps |= 0x10;
        StartVibrato(desc->m_pVibrato);
        StartModulation(desc->m_pModulation);
        return caps;
    }
};
int main()
{
    SEnvelopeParams env = { 0.5f, 0, 0, 0, 0, 0 };
    SVibratoParams mod = { 0.3f, 0, 0 };
    SVoiceDesc d = { 0, 0, 0, 0, 0, 0, 0 };
    CCodaBuffer b = { 0x40000 };
    CSoftwareVoice v; v.m_flags = 0xffffffffu; v.m_pitchEnvCounter = -1; v.m_modulationCounter = -1;
    int ok = v.SetParameters(&b, &d) == (0x800u | 4) && v.m_flags == (0xffffffffu & ~0x7e20u & ~0x18000u)
          && v.m_pitchEnvCounter == -1 && v.m_modulationCounter == -1 && v.m_pitchEnvLevel == 1.0f && g_env == 1 && g_mod == 1;
    b.m_formatFlags = 0x20000;
    ok = ok && v.SetParameters(&b, &d) == (0x400u | 4);
    b.m_formatFlags = 0;
    d.m_pEnvelope = &env; d.m_pPitchEnvelope = &env; d.m_pModulation = &mod;
    ok = ok && v.SetParameters(&b, &d) == (2u | 4 | 8 | 0x10) && v.m_pitchEnvCounter == 5 && v.m_modulationCounter == 3;
    d.m_flags = 0x200 | 0x1;
    ok = ok && v.SetParameters(&b, &d) == (0x201u | 4 | 8 | 0x10) && g_vib == 4;
    if (!ok) { std::printf("00c5bd00_TEST FAIL\n"); return 1; }
    std::printf("00c5bd00_TEST PASS\n");
    return 0;
}