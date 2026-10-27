#include <cstdio>
// CVtable_012bf014::GetLength: one hour of frames at the sample rate.
struct Tone { float m_step; float m_phase; int m_sampleRate; int GetLength() { return m_sampleRate * 3600; } };
int main()
{
    Tone t; t.m_step = 0.0626894f; t.m_phase = 0.0f; t.m_sampleRate = 44100;
    if (t.GetLength() != 158760000) { std::printf("00c73eb0_TEST FAIL\n"); return 1; }
    t.m_sampleRate = 22050;
    if (t.GetLength() != 79380000) { std::printf("00c73eb0_TEST FAIL\n"); return 1; }
    std::printf("00c73eb0_TEST PASS\n");
    return 0;
}