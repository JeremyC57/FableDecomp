// Behaviour test for CReverbDiffuser::UpdateGain (0x00C756C0): the gain reads at (block start - tap delay) and writes at
// (block start - aligned line offset) in the shared ring; a block clear of the ring's end is one Apply call, one that
// crosses it is split into chunks that end where the first of the two positions wraps.
#include <cstdio>

struct CRingBuffer { float* m_data; unsigned int m_size; unsigned int m_pos; };
struct SGainParams { float m_master; float m_current[3]; float m_target[3]; };
struct SDiffuserParams { unsigned int m_taps[3]; SGainParams m_gains[3]; };
struct CReverbConfig { unsigned int m_00, m_channels, m_rate, m_0C, m_tapBase, m_tapSpacing; };

static float g_ring[1000];
static int g_calls = 0;
static long g_src[8], g_dst[8]; static unsigned int g_cnt[8]; static const SGainParams* g_par[8];

struct CDspGain {
    void Apply(const SGainParams* params, const float* source, float* block, int mode, unsigned int count)
    {
        if (g_calls < 8 && mode == 1) { g_par[g_calls] = params; g_src[g_calls] = (long)(source - g_ring);
            g_dst[g_calls] = (long)(block - g_ring); g_cnt[g_calls] = count; }
        ++g_calls;
    }
};

struct CReverbDiffuser {
    CDspGain m_gains[3];
    CReverbConfig* m_pConfig;
    void UpdateGain(unsigned int index, const SDiffuserParams* params, const CRingBuffer* history, unsigned int count)
    {
        float* data = history->m_data;
        unsigned int start = history->m_pos >= count ? history->m_pos - count : history->m_size - count + history->m_pos;
        unsigned int tap = params->m_taps[index];
        unsigned int source = start >= tap ? start - tap : history->m_size - tap + start;
        unsigned int offset = ((index + 1) * m_pConfig->m_tapSpacing / 4 + m_pConfig->m_tapBase + 3) & ~3u;
        unsigned int block = start >= offset ? start - offset : history->m_size - offset + start;
        if (block + count < history->m_size && source + count < history->m_size) {
            m_gains[index].Apply(&params->m_gains[index], data + source, data + block, 1, count);
            return;
        }
        unsigned int remaining = count;
        do {
            unsigned int n1 = remaining;
            unsigned int n = remaining;
            if (source + remaining > history->m_size) n1 = history->m_size - source;
            if (block + remaining > history->m_size) n = history->m_size - block;
            if (n >= n1) n = n1;
            m_gains[index].Apply(&params->m_gains[index], data + source, data + block, 1, n);
            source = source + n < history->m_size ? source + n : source + (n - history->m_size);
            block = block + n < history->m_size ? block + n : block + (n - history->m_size);
            remaining -= n;
        } while (remaining);
    }
};

int main()
{
    int ok = 1;
    CReverbConfig cfg = { 0, 2, 44100, 0, 10, 20 };            // offsets: (i+1)*5 + 10 rounded up to 4
    CReverbDiffuser d; d.m_pConfig = &cfg;
    SDiffuserParams p; p.m_taps[0] = 7; p.m_taps[1] = 50; p.m_taps[2] = 3;
    CRingBuffer r = { g_ring, 1000, 600 };
    d.UpdateGain(1, &p, &r, 100);                              // start 500, read 450, write 500 - 20 = 480
    ok &= g_calls == 1 && g_src[0] == 450 && g_dst[0] == 480 && g_cnt[0] == 100 && g_par[0] == &p.m_gains[1];
    g_calls = 0; r.m_pos = 50;                                 // start 950: read 943, write 950 - 16 = 934
    d.UpdateGain(0, &p, &r, 100);
    ok &= g_calls == 3 && g_src[0] == 943 && g_dst[0] == 934 && g_cnt[0] == 57 && g_src[1] == 0 && g_dst[1] == 991
        && g_cnt[1] == 9 && g_src[2] == 9 && g_dst[2] == 0 && g_cnt[2] == 34;   // read wraps first, then the write
    g_calls = 0; r.m_pos = 10;                                 // start 910: read 907, write 910 - 28 = 882
    d.UpdateGain(2, &p, &r, 100);
    ok &= g_calls == 2 && g_src[0] == 907 && g_dst[0] == 882 && g_cnt[0] == 93 && g_src[1] == 0 && g_dst[1] == 975
        && g_cnt[1] == 7;
    if (!ok) { std::printf("00c756c0_TEST FAIL\n"); return 1; }
    std::printf("00c756c0_TEST PASS\n");
    return 0;
}