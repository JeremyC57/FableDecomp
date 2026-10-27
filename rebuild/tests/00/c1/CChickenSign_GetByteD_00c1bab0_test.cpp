#include <cstdio>

// Behaviour model of CChickenSign::GetByteD (0x00C1BAB0): the +3 byte widened to unsigned short.
struct CChickenSign {
    unsigned char  m_level, m_offset, m_flags, m_byteD;
    short          m_wordB;
    unsigned short m_wordC;
    unsigned short GetByteD() const { return m_byteD; }
};

int main()
{

    CChickenSign s; s.m_level = 1; s.m_offset = 2; s.m_flags = 3; s.m_byteD = 0xF0; s.m_wordB = -1; s.m_wordC = 0xFFFF;
    if (s.GetByteD() != 0xF0) { std::printf("00c1bab0_TEST FAIL\n"); return 1; }
    s.m_byteD = 0;
    if (s.GetByteD() != 0) { std::printf("00c1bab0_TEST FAIL\n"); return 1; }
    std::printf("00c1bab0_TEST PASS\n");
    return 0;
}