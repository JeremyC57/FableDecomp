#include <cstdio>
// Behaviour of func_00ac20e0 (0x00AC20E0): the indexed 16-bit word, zero-extended.
struct CWordArrayOwner_Retail { unsigned short m_words[4]; long func_00ac20e0(long index) const { return m_words[index]; } };
int main()
{
    CWordArrayOwner_Retail o; o.m_words[0] = 5; o.m_words[1] = 0xFFFF; o.m_words[2] = 300; o.m_words[3] = 0;
    if (o.func_00ac20e0(0) != 5 || o.func_00ac20e0(1) != 65535 || o.func_00ac20e0(2) != 300 || o.func_00ac20e0(3) != 0) { std::printf("00ac20e0_TEST FAIL\n"); return 1; }
    std::printf("00ac20e0_TEST PASS\n"); return 0;
}
