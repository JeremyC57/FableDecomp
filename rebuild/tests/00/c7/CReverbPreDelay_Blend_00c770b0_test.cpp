// Behaviour test for CReverbPreDelay::Blend (0x00C770B0): with equal parameter sets the blend output equals a single read;
// with different delay lengths the output is blend * current read + (1 - blend) * target read, blend starting at
// blendPosition / 512 and stepping down by 1/512; the current parameters become the target once count >= blendPosition.
#include <cstdio>
#include <cstring>
#include <cmath>
static long FtoLNear(float f) { double x = f; double fl = std::floor(x); double d = x - fl; long r = (long)fl; if (d > 0.5 || (d == 0.5 && (r & 1))) ++r; return r; }
static long RoundHalfDownToLong(float v) { long r = FtoLNear(v - 0.5f); return r < 0 ? r + 1 : r; }
struct Params { float m_modulationRate, m_delayLength; };
struct PreDelay {
    float* m_data; unsigned int m_size, m_pos; float m_phase;
    float Read(const float* data, float position, float offset)
    {
        float delay = position >= offset ? position - offset : position - offset + (float)m_size;
        long previous = RoundHalfDownToLong(delay) - 1, next = RoundHalfDownToLong(delay);
        if (previous == -1) previous = m_size - 1;
        float fraction = delay - (float)RoundHalfDownToLong(delay);
        return (1.0f - fraction) * data[previous] + fraction * data[next];
    }
    void Blend(Params* current, const Params* target, const float* input, float* output, unsigned int count, unsigned int blendPosition)
    {
        float* data = m_data; unsigned int pos = m_pos, first = count;
        if (pos + count > m_size) first = m_size - pos;
        std::memcpy(data + pos, input, first * sizeof(float));
        if (first < count) std::memcpy(data, input + first, (count - first) * sizeof(float));
        float blend = (float)blendPosition * (1.0f / 512.0f), inverse = 1.0f - blend;
        float step = 1.0f / (blend * current->m_modulationRate + inverse * target->m_modulationRate);
        for (unsigned int i = 0; i < count; ++i) {
            float lfo = m_phase < 0.25f ? m_phase * m_phase * 8.0f : (8.0f - m_phase * 8.0f) * m_phase - 1.0f;
            float position = (float)pos;
            float a = Read(data, position, lfo * current->m_delayLength), b = Read(data, position, lfo * target->m_delayLength);
            output[i] = a * blend + b * inverse;
            m_phase += step; if (m_phase >= 0.75f) m_phase -= 1.0f;
            if (pos + 1 < m_size) pos = pos + 1; else pos += 1 - m_size;
            if (blend > 0.0f) { blend -= 1.0f / 512.0f; inverse = 1.0f - blend; step = 1.0f / (blend * current->m_modulationRate + inverse * target->m_modulationRate); }
        }
        m_pos = pos;
        if (count >= blendPosition) *current = *target;
    }
};
int main()
{
    bool ok = true;
    float ring[16]; for (int i = 0; i < 16; ++i) ring[i] = (float)(i * 10);
    PreDelay d; d.m_data = ring; d.m_size = 16; d.m_pos = 4; d.m_phase = 0.25f;
    float in[2] = { 1.0f, 2.0f }, out[2];
    Params cur = { 4.0f, 0.0f }, tgt = { 4.0f, 2.0f };
    d.Blend(&cur, &tgt, in, out, 2, 256);
    // sample 0: phase 0.25 -> lfo 0.5; current offset 0 -> delay 4 -> ring[3] = 30; target offset 1 -> delay 3 -> ring[2] = 20
    ok = ok && out[0] == 30.0f * 0.5f + 20.0f * 0.5f;
    ok = ok && cur.m_delayLength == 0.0f && d.m_pos == 6;            // 2 < 256: still blending
    d.Blend(&cur, &tgt, in, out, 2, 2);
    ok = ok && cur.m_delayLength == 2.0f;                            // blend complete: adopted
    if (!ok) { std::printf("00c770b0_TEST FAIL\n"); return 1; }
    std::printf("00c770b0_TEST PASS\n");
    return 0;
}