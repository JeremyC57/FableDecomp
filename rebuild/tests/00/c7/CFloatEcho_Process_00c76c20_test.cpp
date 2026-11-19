// Behaviour test for CReverbDelay::Blend (0x00C76C20): cross-faded feedback read, ring write/feedback add, ramped wet mix,
// ring advance with wrap, and adoption of the target parameters when the blend finishes.
#include <cstdio>
#include <cmath>
struct CRingBuffer { float* m_data; unsigned int m_size; unsigned int m_pos; };
struct CDspOps {
    void ReadRingScaledRamp(float* d, const CRingBuffer* r, unsigned int p, float g, float s, unsigned int n)
    { for (unsigned int i = 0; i < n; ++i, g += s) d[i] = r->m_data[(p + i) % r->m_size] * g; }
    void MixRampFromRing(float* d, const CRingBuffer* r, unsigned int p, float g, float s, unsigned int n)
    { for (unsigned int i = 0; i < n; ++i, g += s) d[i] += r->m_data[(p + i) % r->m_size] * g; }
    void WriteRing(CRingBuffer* r, unsigned int p, const float* s, unsigned int n)
    { for (unsigned int i = 0; i < n; ++i) r->m_data[(p + i) % r->m_size] = s[i]; }
    void AddToRing(CRingBuffer* r, unsigned int p, const float* s, unsigned int n)
    { for (unsigned int i = 0; i < n; ++i) r->m_data[(p + i) % r->m_size] += s[i]; }
    void MixRamp(float* d, const float* s, float g, float st, unsigned int n)
    { for (unsigned int i = 0; i < n; ++i, g += st) d[i] += s[i] * g; }
};
static CDspOps g_ops;
struct SReverbDelayParams { unsigned int m_delay; float m_feedback; float m_wet; };
struct CReverbDelay {
    void* vtbl; CRingBuffer m_ring; unsigned int m_index;
    void Blend(SReverbDelayParams* current, const SReverbDelayParams* target, const float* input, float* output,
               unsigned int count, unsigned int blendPosition)
    {
        float delayed[128];
        CDspOps* s_pOps = &g_ops;
        unsigned int pos = m_ring.m_pos;
        unsigned int readCurrent = pos >= current->m_delay ? pos - current->m_delay : m_ring.m_size - current->m_delay + pos;
        unsigned int readTarget = pos >= target->m_delay ? pos - target->m_delay : m_ring.m_size - target->m_delay + pos;
        float blend = (float)blendPosition * (1.0f / 512.0f);
        s_pOps->ReadRingScaledRamp(delayed, &m_ring, readCurrent, blend * current->m_feedback, current->m_feedback * (-1.0f / 512.0f), count);
        s_pOps->MixRampFromRing(delayed, &m_ring, readTarget, (1.0f - blend) * target->m_feedback, target->m_feedback * (1.0f / 512.0f), count);
        s_pOps->WriteRing(&m_ring, pos, input, count);
        s_pOps->AddToRing(&m_ring, pos, delayed, count);
        float wetStep = (target->m_wet - current->m_wet) * (1.0f / 512.0f);
        s_pOps->MixRamp(output, delayed, current->m_wet + (float)(512 - blendPosition) * wetStep, wetStep, count);
        m_ring.m_pos += count;
        if (m_ring.m_pos >= m_ring.m_size) m_ring.m_pos -= m_ring.m_size;
        if (count >= blendPosition) *current = *target;
    }
};
static int Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    float data[16];
    int i;
    for (i = 0; i < 16; ++i) data[i] = (float)(i + 1);
    CReverbDelay d; d.vtbl = 0; d.m_ring.m_data = data; d.m_ring.m_size = 16; d.m_ring.m_pos = 2; d.m_index = 0;
    SReverbDelayParams cur = { 4, 0.5f, 0.0f }, tgt = { 1, 0.25f, 1.0f };
    float in[4] = { 100.f, 200.f, 300.f, 400.f }, out[4] = { 0, 0, 0, 0 };
    d.Blend(&cur, &tgt, in, out, 4, 256);           // blend half way: weights 0.5 / 0.5 at sample 0
    // sample 0: current reads (2-4) wrapped = index 14 -> 15.0 * 0.5*0.5; target reads index 1 -> 2.0 * 0.5*0.25
    float s0 = 15.0f * 0.25f + 2.0f * 0.125f;
    if (!Near(data[2], 100.f + s0)) { std::printf("00c76c20_TEST FAIL\n"); return 1; }
    if (!Near(out[0], s0 * 0.5f)) { std::printf("00c76c20_TEST FAIL\n"); return 1; }  // wet starts at 256/512
    if (d.m_ring.m_pos != 6 || cur.m_delay != 4) { std::printf("00c76c20_TEST FAIL\n"); return 1; }
    d.m_ring.m_pos = 14;
    d.Blend(&cur, &tgt, in, out, 4, 4);             // finishes in this block: wraps and adopts the target
    if (d.m_ring.m_pos != 2 || cur.m_delay != 1 || cur.m_feedback != 0.25f || cur.m_wet != 1.0f) { std::printf("00c76c20_TEST FAIL\n"); return 1; }
    std::printf("00c76c20_TEST PASS\n");
    return 0;
}