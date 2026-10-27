#include <cstdio>

// Behaviour model of CAudioFormat::GetLoopEnd (0x00C2D910): the loop end, else the sample length.
struct Format { unsigned int m_length, m_loopStart, m_loopEnd; unsigned int GetLoopEnd() { return m_loopEnd ? m_loopEnd : m_length; } };

int main()
{

    Format f; f.m_length = 1000; f.m_loopStart = 100; f.m_loopEnd = 0;
    if (f.GetLoopEnd() != 1000) { std::printf("00c2d910_TEST FAIL\n"); return 1; }
    f.m_loopEnd = 800;
    if (f.GetLoopEnd() != 800) { std::printf("00c2d910_TEST FAIL\n"); return 1; }
    std::printf("00c2d910_TEST PASS\n");
    return 0;
}