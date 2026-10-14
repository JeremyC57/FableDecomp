#include <cstdio>
struct CSampler { virtual void v00() { } float m_scale; long m_position; long m_sampleRate; CSampler(long) { m_position = 0; m_sampleRate = 44100; m_scale = 0.062689379f; } };
int main()
{
    CSampler s(99); unsigned long bits = *(unsigned long*)&s.m_scale;
    if (s.m_position != 0 || s.m_sampleRate != 44100 || bits != 0x3D80634A) { std::printf("00c73ef0_TEST FAIL\n"); return 1; }
    std::printf("00c73ef0_TEST PASS\n"); return 0;
}
