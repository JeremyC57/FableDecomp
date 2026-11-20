// Behaviour test for CReverbPreDelay::Process (0x00C76E90): the input block lands in the ring at the write position
// (wrapping), each output sample is the linear interpolation of the two ring samples around the LFO-swung delay, the LFO
// phase advances by 1 / rate (wrapping at 0.75) and the write position advances by one per sample (wrapping).
#include <cstdio>
#include <cstring>
#include <cmath>
static long FtoLNear(float f) { double x = f; double fl = std::floor(x); double d = x - fl; long r = (long)fl; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long RoundHalfDownToLong(float v) { long r = FtoLNear(v - 0.5f); return r < 0 ? r + 1 : r; }
struct Params { float m_modulationRate, m_delayLength; };
struct PreDelay {
    float* m_data; unsigned int m_size, m_pos; float m_phase;
    void Process(const Params* params, const float* input, float* output, unsigned int count)
    {
        float* data = m_data; unsigned int pos = m_pos, first = count;
        if (pos + count > m_size) first = m_size - pos;
        std::memcpy(data + pos, input, first * sizeof(float));
        if (first < count) std::memcpy(data, input + first, (count - first) * sizeof(float));
        float step = params->m_modulationRate > 1.0f ? 1.0f / params->m_modulationRate : 1.0f;
        for (unsigned int i = 0; i < count; ++i) {
            float lfo = m_phase < 0.25f ? m_phase * m_phase * 8.0f : (8.0f - m_phase * 8.0f) * m_phase - 1.0f;
            float delay = (float)pos - lfo * params->m_delayLength;
            if (delay < 0.0f) delay += (float)m_size;
            long previous = RoundHalfDownToLong(delay) - 1, next = RoundHalfDownToLong(delay);
            if (previous == -1) previous = m_size - 1;
            float fraction = delay - (float)RoundHalfDownToLong(delay);
            output[i] = (1.0f - fraction) * data[previous] + fraction * data[next];
            m_phase += step; if (m_phase >= 0.75f) m_phase -= 1.0f;
            if (pos + 1 < m_size) pos = pos + 1; else pos += 1 - m_size;
        }
        m_pos = pos;
    }
};
int main()
{
    bool ok = true;
    float ring[16]; std::memset(ring, 0, sizeof ring);
    PreDelay d; d.m_data = ring; d.m_size = 16; d.m_pos = 12; d.m_phase = 0.0f;
    float in[8], out[8]; for (int i = 0; i < 8; ++i) in[i] = (float)(i + 1);
    Params p = { 1.0f, 0.0f };                 // no modulation: read one sample behind the write position
    d.Process(&p, in, out, 8);
    ok = ok && ring[12] == 1.0f && ring[15] == 4.0f && ring[0] == 5.0f && ring[3] == 8.0f && d.m_pos == 4;
    ok = ok && out[0] == 0.0f && out[1] == 1.0f && out[4] == 4.0f && out[5] == 5.0f;   // data[pos - 1] (pos 12 reads ring[11])
    // the phase advanced by 1 per sample and wrapped below 0.75 each time
    ok = ok && d.m_phase == 0.0f;
    // modulated: rate 4 samples per cycle, depth 2: phase steps 0.25 and the delay swings
    std::memset(ring, 0, sizeof ring); for (int i = 0; i < 16; ++i) ring[i] = (float)(i * 10);
    d.m_pos = 8; d.m_phase = 0.0f; Params q = { 4.0f, 2.0f };
    d.Process(&q, in, out, 2);
    // sample 0: phase 0 -> lfo 0, delay 8 -> previous 7, next 8, fraction 0 -> ring[7] = 70
    ok = ok && out[0] == 70.0f && d.m_phase == 0.5f && d.m_pos == 10;
    // sample 1: phase 0.25 -> lfo 0.5, delay 9 - 1 = 8 -> ring[7] = 70
    ok = ok && out[1] == 70.0f;
    if (!ok) { std::printf("00c76e90_TEST FAIL\n"); return 1; }
    std::printf("00c76e90_TEST PASS\n");
    return 0;
}