#include <cstdio>
#include <cmath>
// CVtable_012bf014::Read: sin(phase)*32767 to every channel of each frame, phase += step per frame.
struct SAudioBuffer { short* m_data; unsigned int m_channels; };
struct Tone {
    float m_step; float m_phase; int m_sampleRate;
    int Read(SAudioBuffer* buffer, unsigned int frames)
    {
        short* out = buffer->m_data;
        for (unsigned int i = 0; i < frames; ++i) {
            short sample = (short)(std::sin(m_phase) * 32767.0f);
            for (unsigned int c = 0; c < buffer->m_channels; ++c)
                *out++ = sample;
            m_phase += m_step;
        }
        return (int)frames;
    }
};
int main()
{
    Tone t; t.m_step = 0.5f; t.m_phase = 0.0f; t.m_sampleRate = 44100;
    short data[8] = { 0, 0, 0, 0, 0, 0, 0, 77 };
    SAudioBuffer b; b.m_data = data; b.m_channels = 2;
    if (t.Read(&b, 3) != 3) { std::printf("00c73f10_TEST FAIL\n"); return 1; }
    for (int f = 0; f < 3; ++f) {
        short want = (short)(std::sin(0.5f * f) * 32767.0f);
        if (data[f * 2] != want || data[f * 2 + 1] != want) { std::printf("00c73f10_TEST FAIL\n"); return 1; }
    }
    if (data[6] != 0 || data[7] != 77 || std::fabs(t.m_phase - 1.5f) > 1e-6f) { std::printf("00c73f10_TEST FAIL\n"); return 1; }
    if (t.Read(&b, 0) != 0) { std::printf("00c73f10_TEST FAIL\n"); return 1; }
    std::printf("00c73f10_TEST PASS\n");
    return 0;
}