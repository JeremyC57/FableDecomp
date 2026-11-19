// Behaviour test for 0x00C75FF0 CCombFilter::Process (libpk reverb comb): output += line tap 0; block = comb ring read back
// by its delay * feedback, + line taps 0 and 1; block written back into the comb ring at the old position; input += block *
// gain; the comb position advances with wrap.
#include <cstdio>

struct Ring {
    float* data; unsigned int size; unsigned int pos;
    unsigned int Back(unsigned int p, unsigned int n) const { return p >= n ? p - n : size - n + p; }
};
struct Settings { unsigned int historyDelay[2]; unsigned int delay; float feedback; float gain; };

// CDspOps stand-ins (the x87 table's semantics)
static void AddFromRing(float* dst, const Ring* r, unsigned int p, unsigned int n) { for (unsigned int i = 0; i < n; ++i) dst[i] += r->data[(p + i) % r->size]; }
static void ReadRingScaled(float* dst, const Ring* r, unsigned int p, float g, unsigned int n) { for (unsigned int i = 0; i < n; ++i) dst[i] = r->data[(p + i) % r->size] * g; }
static void WriteRing(Ring* r, unsigned int p, const float* src, unsigned int n) { for (unsigned int i = 0; i < n; ++i) r->data[(p + i) % r->size] = src[i]; }
static void Mix(float* dst, const float* src, float g, unsigned int n) { for (unsigned int i = 0; i < n; ++i) dst[i] += src[i] * g; }

struct Comb {
    Ring m_ring;
    void Process(const Settings* current, Ring* line, float* output, const float* input, unsigned int count)
    {
        float block[128];
        unsigned int pos = m_ring.pos;
        unsigned int read = m_ring.Back(pos, current->delay);
        unsigned int start = line->Back(line->pos, count);
        AddFromRing(output, line, line->Back(start, current->historyDelay[0]), count);
        ReadRingScaled(block, &m_ring, read, current->feedback, count);
        for (unsigned int i = 0; i < 2; ++i)
            AddFromRing(block, line, line->Back(start, current->historyDelay[i]), count);
        WriteRing(&m_ring, pos, block, count);
        Mix((float*)input, block, current->gain, count);
        m_ring.pos = pos + count < m_ring.size ? pos + count : pos - m_ring.size + count;
    }
};

static bool near_(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    float lineData[16], combData[8];
    for (int i = 0; i < 16; ++i) lineData[i] = (float)(i + 1);
    for (int i = 0; i < 8; ++i) combData[i] = (float)(10 * i);
    Ring line = { lineData, 16, 4 };                // block start = 4 - 4 wraps to 0
    Comb comb; comb.m_ring.data = combData; comb.m_ring.size = 8; comb.m_ring.pos = 6;
    Settings s = { { 2, 3 }, 3, 0.5f, 2.0f };
    float out[4] = { 0, 0, 0, 0 }, in[4] = { 1, 1, 1, 1 };
    comb.Process(&s, &line, out, in, 4);
    // tap 0 starts at Back(0, 2) = 14: line 15,16,1,2
    if (!near_(out[0], 15) || !near_(out[1], 16) || !near_(out[2], 1) || !near_(out[3], 2)) { std::printf("00c75ff0_TEST FAIL\n"); return 1; }
    // block[i] = comb[(3+i)%8]*0.5 + tap0 + tap1 (Back(0,3) = 13: 14,15,16,1)
    float expect[4] = { 15 + 15 + 14, 20 + 16 + 15, 25 + 1 + 16, 30 + 2 + 1 };
    for (int i = 0; i < 4; ++i) {
        if (!near_(combData[(6 + i) % 8], expect[i]) || !near_(in[i], 1 + expect[i] * 2)) { std::printf("00c75ff0_TEST FAIL\n"); return 1; }
    }
    if (comb.m_ring.pos != 2) { std::printf("00c75ff0_TEST FAIL\n"); return 1; }
    std::printf("00c75ff0_TEST PASS\n");
    return 0;
}