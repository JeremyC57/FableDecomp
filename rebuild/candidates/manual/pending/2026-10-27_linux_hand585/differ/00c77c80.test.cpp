// Behaviour test for CReverbProcessor::UpdateLineAndTapLengths (0x00C77C80): line all-pass lengths and tap delay lengths.
#include <cstdio>
#include <cmath>
struct SMixFormat { unsigned int m_flags, m_channels, m_sampleRate; };
struct SReverbParams { float m_size; };
struct SReverbLine { int m_allpassLength; float m_allpassCoef; };
struct SReverbTap { int m_delayLength[2]; };
struct SReverbState { SReverbLine m_lines[4]; SReverbTap m_taps[4]; };
struct SReverbLineDesc { float m_scale, m_allpassCoef, m_allpassDelayMs, m_unk0c, m_unk10; };
static const SReverbLineDesc g_reverbLineDescs[4] = {
    { 1.05f, -0.4f, 3.651f, 915.6f, 0.1052f }, { 1.14f, 0.4f, 3.515f, 872.5f, 0.0673f },
    { 0.84f, -0.4f, 3.795f, 902.1f, 0.1737f }, { 0.93f, -0.4f, 3.946f, 922.4f, 0.2404f } };
static const float g_reverbTapDelays[4][2] = {
    { 0.0f, 0.002449f }, { 0.000839f, 0.002299f }, { 0.004864f, 0.009637f }, { 0.005624f, 0.00917f } };
// the retail fld/fistp helper rounds to nearest (even) under the default FPU mode
static int FloatToLong(float v) { double r = std::floor((double)v + 0.5); if (r - v == 0.5 && std::fmod(r, 2.0) != 0) r -= 1; return (int)r; }
static int RoundHalfDownToLong(float v) { int r = FloatToLong(v - 0.5f); if (r < 0) ++r; return r; }
struct CReverbProcessor {
    SMixFormat* m_pFormat;
    void UpdateLineAndTapLengths(SReverbState* state, const SReverbParams* params)
    {
        float allpassScale = params->m_size * 20.0f + 1.0f;
        float size = params->m_size;
        unsigned int i, j;
        for (i = 0; i < 4; i++) {
            state->m_lines[i].m_allpassCoef = g_reverbLineDescs[i].m_allpassCoef;
            state->m_lines[i].m_allpassLength =
                (RoundHalfDownToLong((float)m_pFormat->m_sampleRate * g_reverbLineDescs[i].m_allpassDelayMs * allpassScale * 0.001f) + 3) & ~3;
        }
        for (i = 0; i < 4; i++)
            for (j = 0; j < 2; j++)
                state->m_taps[i].m_delayLength[j] =
                    (RoundHalfDownToLong((size + g_reverbTapDelays[i][j]) * (float)m_pFormat->m_sampleRate) + 3) & ~3;
    }
};
int main()
{
    SMixFormat fmt = { 0, 2, 44100 };
    CReverbProcessor p; p.m_pFormat = &fmt;
    SReverbParams prm = { 0.1f };
    SReverbState st;
    p.UpdateLineAndTapLengths(&st, &prm);
    // line 0: 44100 * 3.651 * 3.0 * 0.001 = 483.03 -> round(482.53) = 483 -> 484
    int bad = 0;
    if (st.m_lines[0].m_allpassLength != 484 || std::fabs(st.m_lines[0].m_allpassCoef + 0.4f) > 1e-6f) bad = 1;
    for (int i = 0; i < 4; i++) {
        if (st.m_lines[i].m_allpassLength % 4 != 0) bad = 2;
        for (int j = 0; j < 2; j++) if (st.m_taps[i].m_delayLength[j] % 4 != 0) bad = 3;
    }
    // tap 0/0: 0.1 * 44100 = 4410 -> round(4409.5) = 4410 (even) -> 4412
    if (st.m_taps[0].m_delayLength[0] != 4412) bad = 4;
    if (st.m_lines[1].m_allpassCoef != 0.4f) bad = 5;
    if (bad) { std::printf("00c77c80_TEST FAIL %d\n", bad); return 1; }
    std::printf("00c77c80_TEST PASS\n");
    return 0;
}
