// Behaviour test for CStereoWidener::Process (0x00C547D0): each channel adds the one-pole low-passed, inverted output of the other
// channel from ten samples earlier; checked against an independent reference over two blocks.
#include <cstdio>
#include <cstring>
struct CStereoWidener {
    long m_history[2][16]; long m_filter[2]; unsigned long m_index;
    void Process(long* left, long* right)
    {
        for (int i = 0; i < 128; ++i) {
            m_filter[0] += (m_filter[0] * -335 - m_history[0][m_index] * 258) >> 12;
            m_filter[1] += (m_filter[1] * -335 - m_history[1][m_index] * 258) >> 12;
            left[i] += m_filter[1];
            m_history[0][m_index] = left[i];
            right[i] += m_filter[0];
            m_history[1][m_index] = right[i];
            if (++m_index == 10) m_index = 0;
        }
    }
};
int main()
{
    CStereoWidener w; std::memset(&w, 0, sizeof w);
    // reference: outputs kept in full arrays, delayed by 10
    static long outL[256], outR[256]; long fL = 0, fR = 0;
    bool ok = true;
    for (int block = 0; block < 2; ++block) {
        long left[128], right[128];
        for (int i = 0; i < 128; ++i) { left[i] = (i * 977 + block * 31) % 20000 - 10000; right[i] = (i * 461 + 7) % 16000 - 8000; }
        long inL[128], inR[128]; std::memcpy(inL, left, sizeof inL); std::memcpy(inR, right, sizeof inR);
        w.Process(left, right);
        for (int i = 0; i < 128; ++i) {
            int n = block * 128 + i;
            long dl = n >= 10 ? outL[n - 10] : 0, dr = n >= 10 ? outR[n - 10] : 0;
            fL += (fL * -335 - dl * 258) >> 12;
            fR += (fR * -335 - dr * 258) >> 12;
            outL[n] = inL[i] + fR; outR[n] = inR[i] + fL;
            ok = ok && left[i] == outL[n] && right[i] == outR[n];
        }
    }
    ok = ok && w.m_index == 256 % 10;
    if (!ok) { std::printf("00c547d0_TEST FAIL\n"); return 1; }
    std::printf("00c547d0_TEST PASS\n");
    return 0;
}