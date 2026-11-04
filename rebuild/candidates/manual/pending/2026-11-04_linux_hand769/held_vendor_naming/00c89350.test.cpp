// Behaviour test for CReverbDiffuserStage::ProcessRamped 0x00C89350: eight history taps (old record fading out from
// rampLength/512 by -1/512, new record fading in from 1 - that by +1/512), read positions wrapped at the history ring;
// the direct copy and the scaled block use blended gains with the per-sample (new - old)/512 step; the stage's own
// delays mixed the same way (wrapped at its ring), the mix ramp, the crossfade split at the own ring's end, the ring
// advanced, and the old record replaced by the new once count reaches rampLength.
#include <cstdio>
#include <cstring>
#include <cmath>

struct CRingBuffer { float* m_data; unsigned int m_size, m_pos;
    void Advance(unsigned int c) { m_pos += c; if (m_pos >= m_size) m_pos -= m_size; } };
struct Call { int op; const void* ring; unsigned int pos; float gain, step; unsigned int count; const float* src; float* dst; };
static Call g_calls[64]; static int g_n;
static void Rec(int op, const void* ring, unsigned int pos, float g, float s, unsigned int c, const float* src, float* dst)
{ if (g_n < 64) { Call k = { op, ring, pos, g, s, c, src, dst }; g_calls[g_n] = k; } ++g_n; }
struct Ops {
    void ScaleRampCopy(float* d, const float* s, float g, float st, unsigned int c) { Rec(1, 0, 0, g, st, c, s, d); }
    void MixRamp(float* d, const float* s, float g, float st, unsigned int c) { Rec(6, 0, 0, g, st, c, s, d); }
    void ReadRingScaledRamp(float* d, const CRingBuffer* r, unsigned int p, float g, float st, unsigned int c) { Rec(10, r, p, g, st, c, 0, d); }
    void MixRampFromRing(float* d, const CRingBuffer* r, unsigned int p, float g, float st, unsigned int c) { Rec(11, r, p, g, st, c, 0, d); }
};
static Ops g_ops; static Ops* s_pOps = &g_ops;
struct SGainParams { float m_master, m_current[3], m_target[3]; };
struct Xf { const SGainParams* o; const SGainParams* n; const float* src; float* dst; int mode; unsigned int count, remaining; };
static Xf g_xf[4]; static int g_nx;
struct CDspGain { void Crossfade(SGainParams* o, const SGainParams* n, const float* s, float* d, int m, unsigned int c, unsigned int r)
    { Xf x = { o, n, s, d, m, c, r }; g_xf[g_nx++] = x; } };
struct SDiffuserStageParams { unsigned int m_taps[4]; float m_tapGain, m_directGain; SGainParams m_gain; unsigned int m_delay; float m_mixGain; };
struct CReverbDiffuserStage {
    CRingBuffer m_ring; CDspGain m_gain; float* lastBlock;
    void ProcessRamped(SDiffuserStageParams* params, const SDiffuserStageParams* next, const CRingBuffer* history,
                       float* output, float* mix, unsigned int count, unsigned int rampLength)
    {
        float block[128]; lastBlock = block;
        unsigned int start = history->m_pos >= count ? history->m_pos - count : history->m_size - count + history->m_pos;
        float* data = m_ring.m_data; unsigned int pos = m_ring.m_pos;
        float blend = (float)rampLength * (1.0f / 512.0f), other = 1.0f - blend;
        for (unsigned int i = 0; i < 4; ++i) {
            unsigned int tap = params->m_taps[i];
            unsigned int p = start >= tap ? start - tap : history->m_size - tap + start;
            if (i == 0) s_pOps->ReadRingScaledRamp(block, history, p, blend, -1.0f / 512.0f, count);
            else s_pOps->MixRampFromRing(block, history, p, blend, -1.0f / 512.0f, count);
            tap = next->m_taps[i];
            s_pOps->MixRampFromRing(block, history, start >= tap ? start - tap : history->m_size - tap + start, other, 1.0f / 512.0f, count);
        }
        s_pOps->ScaleRampCopy(output, block, blend * params->m_directGain + other * next->m_directGain,
                              (next->m_directGain - params->m_directGain) * (1.0f / 512.0f), count);
        s_pOps->ScaleRampCopy(block, block, blend * params->m_tapGain + other * next->m_tapGain,
                              (next->m_tapGain - params->m_tapGain) * (1.0f / 512.0f), count);
        unsigned int d = params->m_delay;
        s_pOps->MixRampFromRing(block, &m_ring, pos >= d ? pos - d : m_ring.m_size - d + pos, blend, -1.0f / 512.0f, count);
        d = next->m_delay;
        s_pOps->MixRampFromRing(block, &m_ring, pos >= d ? pos - d : m_ring.m_size - d + pos, other, 1.0f / 512.0f, count);
        s_pOps->MixRamp(mix, block, blend * params->m_mixGain + other * next->m_mixGain,
                        (next->m_mixGain - params->m_mixGain) * (1.0f / 512.0f), count);
        unsigned int first = count;
        if (pos + count > m_ring.m_size) first = m_ring.m_size - pos;
        m_gain.Crossfade(&params->m_gain, &next->m_gain, block, data + pos, 1, first, rampLength);
        if (first < count) m_gain.Crossfade(&params->m_gain, &next->m_gain, block + first, data, 1, count - first, rampLength);
        m_ring.Advance(count);
        if (count >= rampLength) *params = *next;
    }
};
static bool Near(float a, float b) { return std::fabs(a - b) < 1e-6f; }
int main()
{
    bool ok = true;
    float own[100], hist[1000], out[64], mixb[64];
    CRingBuffer h = { hist, 1000, 10 };
    CReverbDiffuserStage st; std::memset(&st, 0, sizeof st); st.m_ring.m_data = own; st.m_ring.m_size = 100; st.m_ring.m_pos = 80;
    SDiffuserStageParams a, b; std::memset(&a, 0, sizeof a); std::memset(&b, 0, sizeof b);
    unsigned int ta[4] = { 5, 100, 300, 990 }, tb[4] = { 7, 200, 400, 2 };
    std::memcpy(a.m_taps, ta, sizeof ta); std::memcpy(b.m_taps, tb, sizeof tb);
    a.m_directGain = 1.0f; b.m_directGain = 0.0f; a.m_tapGain = 0.5f; b.m_tapGain = 0.25f; a.m_mixGain = 0.0f; b.m_mixGain = 2.0f;
    a.m_delay = 90; b.m_delay = 10; b.m_gain.m_master = 3.0f;
    g_n = 0; g_nx = 0;
    st.ProcessRamped(&a, &b, &h, out, mixb, 32, 256);
    float bl = 0.5f, ot = 0.5f;
    // start = 1000 - 32 + 10 = 978
    unsigned int expPos[8] = { 973, 971, 878, 778, 678, 578, 988, 976 };
    ok = ok && g_n == 13 && g_calls[0].op == 10;
    for (int i = 0; i < 8 && ok; ++i)
        ok = g_calls[i].ring == &h && g_calls[i].pos == expPos[i] && g_calls[i].count == 32 && (i == 0 || g_calls[i].op == 11)
             && Near(g_calls[i].gain, (i & 1) ? ot : bl) && Near(g_calls[i].step, (i & 1) ? 1.0f / 512 : -1.0f / 512);
    ok = ok && g_calls[8].op == 1 && g_calls[8].dst == out && Near(g_calls[8].gain, 0.5f) && Near(g_calls[8].step, -1.0f / 512);
    ok = ok && g_calls[9].op == 1 && g_calls[9].dst == st.lastBlock && Near(g_calls[9].gain, 0.375f) && Near(g_calls[9].step, -0.25f / 512);
    ok = ok && g_calls[10].ring == &st.m_ring && g_calls[10].pos == 90 && g_calls[11].pos == 70 && Near(g_calls[11].gain, ot);
    ok = ok && g_calls[12].op == 6 && g_calls[12].dst == mixb && Near(g_calls[12].gain, 1.0f) && Near(g_calls[12].step, 2.0f / 512);
    // own ring: pos 80 + 32 > 100: split 20 / 12
    ok = ok && g_nx == 2 && g_xf[0].dst == own + 80 && g_xf[0].count == 20 && g_xf[0].remaining == 256 && g_xf[0].mode == 1
            && g_xf[1].dst == own && g_xf[1].count == 12 && g_xf[1].src == st.lastBlock + 20 && st.m_ring.m_pos == 12;
    ok = ok && a.m_taps[0] == 5 && a.m_gain.m_master == 0.0f;              // ramp not finished: old record kept
    g_n = 0; g_nx = 0; st.m_ring.m_pos = 0; h.m_pos = 500;
    st.ProcessRamped(&a, &b, &h, out, mixb, 64, 64);
    ok = ok && g_nx == 1 && g_xf[0].count == 64 && st.m_ring.m_pos == 64 && Near(g_calls[0].gain, 0.125f) && Near(g_calls[1].gain, 0.875f)
            && g_calls[0].pos == 431 && a.m_taps[0] == 7 && a.m_gain.m_master == 3.0f && a.m_delay == 10;
    if (ok) std::printf("00c89350_TEST PASS\n"); else std::printf("00c89350_TEST FAIL\n");
    return ok ? 0 : 1;
}
