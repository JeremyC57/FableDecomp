// Behaviour test for 0x00C891A0 CReverbDiffuserStage::Process: the model and body with working x87-style DSP kernels, checked
// against a sample-by-sample reference (four wrapped history taps, scaled copy, own-line feedback, gain write-back split at
// the ring end, scaled mix, write-position advance).
#include <cstdio>
#include <cstring>
#include <cmath>
struct CRingBuffer {
    float* m_data; unsigned int m_size; unsigned int m_pos;
    void Advance(unsigned int count) { m_pos += count; if (m_pos >= m_size) m_pos -= m_size; }
};
struct CDspOps {
    virtual long GetKind() { return 0; }
    virtual void ScaleRampCopy(float*, const float*, float, float, unsigned int) {}
    virtual void ScaleRamp(float*, float, float, unsigned int) {}
    virtual void ScaleCopy(float* dst, const float* src, float gain, unsigned int count) { for (unsigned int i = 0; i < count; ++i) dst[i] = src[i] * gain; }
    virtual void Scale(float* buffer, float gain, unsigned int count) { for (unsigned int i = 0; i < count; ++i) buffer[i] *= gain; }
    virtual void Add(float* dst, const float* src, unsigned int count) { for (unsigned int i = 0; i < count; ++i) dst[i] += src[i]; }
    virtual void MixRamp(float*, const float*, float, float, unsigned int) {}
    virtual void Mix(float* dst, const float* src, float gain, unsigned int count) { for (unsigned int i = 0; i < count; ++i) dst[i] += src[i] * gain; }
    void ReadRing(float* dst, const CRingBuffer* ring, unsigned int pos, unsigned int count)
    { for (unsigned int i = 0; i < count; ++i) dst[i] = ring->m_data[(pos + i) % ring->m_size]; }
    void AddFromRing(float* dst, const CRingBuffer* ring, unsigned int pos, unsigned int count)
    { for (unsigned int i = 0; i < count; ++i) dst[i] += ring->m_data[(pos + i) % ring->m_size]; }
};
struct CDspObject { static CDspOps* s_pOps; };
CDspOps* CDspObject::s_pOps = 0;
struct SGainParams { float m_master; float m_current[3]; float m_target[3]; };
struct CDspGain : CDspObject {
    float m_gain[4];
    int calls;
    void Apply(const SGainParams* params, const float* source, float* block, int mode, unsigned int count)
    { ++calls; if (mode != 1) calls += 100; for (unsigned int i = 0; i < count; ++i) block[i] = source[i] * params->m_master; }
};
struct SDiffuserStageParams {
    unsigned int m_taps[4]; float m_tapGain; float m_directGain; SGainParams m_gain; unsigned int m_delay; float m_mixGain;
};
struct CReverbDiffuserStage : CDspObject {
    CRingBuffer m_ring; CDspGain m_gain; unsigned int m_index;
    void Process(const SDiffuserStageParams* params, const CRingBuffer* history, float* output, float* mix, unsigned int count);
};
void CReverbDiffuserStage::Process(const SDiffuserStageParams* params, const CRingBuffer* history, float* output, float* mix, unsigned int count)
{
    float block[128];
    unsigned int start = history->m_pos >= count ? history->m_pos - count : history->m_size - count + history->m_pos;
    float* data = m_ring.m_data;
    unsigned int pos = m_ring.m_pos;
    unsigned int tap = params->m_taps[0];
    s_pOps->ReadRing(block, history, start >= tap ? start - tap : history->m_size - tap + start, count);
    for (unsigned int i = 1; i < 4; ++i)
    {
        tap = params->m_taps[i];
        s_pOps->AddFromRing(block, history, start >= tap ? start - tap : history->m_size - tap + start, count);
    }
    s_pOps->ScaleCopy(output, block, params->m_directGain, count);
    s_pOps->Scale(block, params->m_tapGain, count);
    unsigned int delay = params->m_delay;
    s_pOps->AddFromRing(block, &m_ring, pos >= delay ? pos - delay : m_ring.m_size - delay + pos, count);
    unsigned int first = count;
    if (pos + count > m_ring.m_size)
        first = m_ring.m_size - pos;
    m_gain.Apply(&params->m_gain, block, data + pos, 1, first);
    if (first < count)
        m_gain.Apply(&params->m_gain, block + first, data, 1, count - first);
    s_pOps->Mix(mix, block, params->m_mixGain, count);
    m_ring.Advance(count);
}
static int run(unsigned int histPos, unsigned int ownPos, unsigned int count)
{
    float hist[64]; float own[24]; float ownRef[24];
    for (int i = 0; i < 64; ++i) hist[i] = (float)((i * 37) % 23) - 11.0f;
    for (int i = 0; i < 24; ++i) own[i] = ownRef[i] = (float)((i * 13) % 7) * 0.5f;
    CRingBuffer history; history.m_data = hist; history.m_size = 64; history.m_pos = histPos;
    SDiffuserStageParams p;
    p.m_taps[0] = 3; p.m_taps[1] = 10; p.m_taps[2] = 0; p.m_taps[3] = 50;
    p.m_tapGain = 0.5f; p.m_directGain = 2.0f; p.m_gain.m_master = 0.25f; p.m_delay = 5; p.m_mixGain = 3.0f;
    CReverbDiffuserStage st; st.m_ring.m_data = own; st.m_ring.m_size = 24; st.m_ring.m_pos = ownPos; st.m_gain.calls = 0;
    float out[16]; float mix[16];
    for (int i = 0; i < 16; ++i) { out[i] = -99.0f; mix[i] = 1.0f; }
    st.Process(&p, &history, out, mix, count);
    /* reference */
    unsigned int start = (histPos + 64 - count) % 64;
    for (unsigned int i = 0; i < count; ++i)
    {
        float sum = 0.0f;
        for (int t = 0; t < 4; ++t) sum += hist[(start + 64 - p.m_taps[t] + i) % 64];
        if (std::fabs(out[i] - sum * 2.0f) > 1e-4f) return 0;
        float b = sum * 0.5f + ownRef[(ownPos + 24 - 5 + i) % 24];
        float w = b * 0.25f;
        if (std::fabs(own[(ownPos + i) % 24] - w) > 1e-4f) return 0;
        if (std::fabs(mix[i] - (1.0f + b * 3.0f)) > 1e-4f) return 0;
    }
    if (st.m_ring.m_pos != (ownPos + count) % 24) return 0;
    if (st.m_gain.calls != ((ownPos + count > 24) ? 2 : 1)) return 0;
    return 1;
}
int main()
{
    CDspOps ops; CDspObject::s_pOps = &ops;
    int fails = 0;
    if (!run(20, 2, 8)) fails |= 1;        /* no wrap anywhere */
    if (!run(3, 20, 8)) fails |= 2;        /* history start wraps, own line splits at its end */
    if (!run(40, 16, 8)) fails |= 4;       /* own write ends exactly at the ring end: one Apply, position wraps to 0 */
    if (!run(8, 0, 16)) fails |= 8;        /* start exactly 0 after the subtraction, delay wraps backwards */
    std::printf(fails ? "00c891a0_TEST FAIL %d\n" : "00c891a0_TEST PASS\n", fails);
    return fails != 0;
}