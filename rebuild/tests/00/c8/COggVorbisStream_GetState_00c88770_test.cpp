#include <cstdio>
// COggVorbisStream::GetState: 2 when valid, -1 otherwise.
struct Stream { int m_valid; int GetState() { return m_valid ? 2 : -1; } };
int main()
{
    Stream s; s.m_valid = 0;
    if (s.GetState() != -1) { std::printf("00c88770_TEST FAIL\n"); return 1; }
    s.m_valid = 1;
    if (s.GetState() != 2) { std::printf("00c88770_TEST FAIL\n"); return 1; }
    s.m_valid = 0x100;
    if (s.GetState() != 2) { std::printf("00c88770_TEST FAIL\n"); return 1; }
    std::printf("00c88770_TEST PASS\n");
    return 0;
}