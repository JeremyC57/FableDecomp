// Behaviour test for CReverbStage94::Process 0x00C75CE0: SIMD path (ReadRing x4, one FilterGain4 over the four blocks
// with the tap parameter/state arrays, MixHistory x4 with channel j % n) or x87 path (Process x4), then Feedback x4 with
// the matrix rows. Stand-in callees record what they were given.
#include <cstdio>

struct SGainParams { float m_master; float m_current[3]; float m_target[3]; };
struct SReverbTapParams { unsigned int d[2]; float g[2]; SGainParams m_filter; float out; };
struct SStageSettings94 { SReverbTapParams m_taps[4]; };
struct Ring { float* data; unsigned int size, pos; };
static int g_kind, g_log[64], g_n;
static float (*g_filterBlocks)[128]; static float* g_states[4]; static const SGainParams* g_params[4];
static const float* g_chan[4]; static const float* g_rows[4];
struct Tap {
    void* vp; float gain[5]; Ring ring; unsigned int index; int id;
    void ReadRing(float* block, unsigned int) { g_log[g_n++] = 10 + id; block[0] = (float)id; }
    void MixHistory(const SReverbTapParams*, const Ring*, float*, float* ch, unsigned int) { g_log[g_n++] = 20 + id; g_chan[id] = ch; }
    void Process(const SReverbTapParams*, const Ring*, float*, const float* in, unsigned int) { g_log[g_n++] = 30 + id; g_chan[id] = in; }
    void Feedback(float (*)[128], float*, const float* row, unsigned int) { g_log[g_n++] = 40 + id; g_rows[id] = row; }
};
static const float g_matrix[4][4] = { {0.5f,-0.5f,-0.5f,-0.5f}, {-0.5f,0.5f,-0.5f,-0.5f}, {-0.5f,-0.5f,0.5f,-0.5f}, {-0.5f,-0.5f,-0.5f,0.5f} };
static void FilterGain4(float (*dst)[128], float (*)[128], float* const* states, const SGainParams* const* params, unsigned int)
{
    g_log[g_n++] = 99; g_filterBlocks = dst;
    for (int i = 0; i < 4; ++i) { g_states[i] = states[i]; g_params[i] = params[i]; }
}
struct Stage {
    void* vp; Tap m_taps[4]; unsigned int m_channels;
    void Process(const SStageSettings94* current, Ring* line, float* output, const float* const* inputs, unsigned long count)
    {
        static float blocks[4][128];
        if (g_kind == 1) {
            const SGainParams* params[4]; float* states[4];
            for (unsigned int i = 0; i < 4; ++i) {
                states[i] = &m_taps[i].gain[1];
                params[i] = &current->m_taps[i].m_filter;
                m_taps[i].ReadRing(blocks[i], count);
            }
            FilterGain4(blocks, blocks, states, params, count);
            for (unsigned int j = 0; j < 4; ++j)
                m_taps[j].MixHistory(&current->m_taps[j], line, blocks[j], const_cast<float*>(inputs[j % m_channels]), count);
        } else {
            for (unsigned int j = 0; j < 4; ++j)
                m_taps[j].Process(&current->m_taps[j], line, blocks[j], inputs[j % m_channels], count);
        }
        for (unsigned int k = 0; k < 4; ++k) m_taps[k].Feedback(blocks, output, g_matrix[k], count);
    }
};
int main()
{
    Stage st; for (int i = 0; i < 4; ++i) st.m_taps[i].id = i; st.m_channels = 2;
    SStageSettings94 cur; Ring line; float out[128]; float c0[4], c1[4]; const float* in[2] = { c0, c1 };
    bool ok = true;
    g_kind = 1; g_n = 0; st.Process(&cur, &line, out, in, 128);
    int want1[] = { 10, 11, 12, 13, 99, 20, 21, 22, 23, 40, 41, 42, 43 };
    ok = ok && g_n == 13; for (int i = 0; i < 13 && ok; ++i) ok = g_log[i] == want1[i];
    ok = ok && g_params[2] == &cur.m_taps[2].m_filter && g_states[3] == &st.m_taps[3].gain[1];
    ok = ok && g_chan[0] == c0 && g_chan[1] == c1 && g_chan[2] == c0 && g_chan[3] == c1 && g_rows[2] == g_matrix[2];
    g_kind = 0; g_n = 0; st.Process(&cur, &line, out, in, 128);
    int want0[] = { 30, 31, 32, 33, 40, 41, 42, 43 };
    ok = ok && g_n == 8; for (int i = 0; i < 8 && ok; ++i) ok = g_log[i] == want0[i];
    if (!ok) { std::printf("00c75ce0_TEST FAIL\n"); return 1; }
    std::printf("00c75ce0_TEST PASS\n");
    return 0;
}