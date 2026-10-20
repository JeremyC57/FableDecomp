#include <cstdio>
#include <cstdlib>
#include <cstring>
static const unsigned long g_bitMasks[33] = {
    0x0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff, 0x1ff, 0x3ff, 0x7ff, 0xfff, 0x1fff, 0x3fff, 0x7fff, 0xffff,
    0x1ffff, 0x3ffff, 0x7ffff, 0xfffff, 0x1fffff, 0x3fffff, 0x7fffff, 0xffffff, 0x1ffffff, 0x3ffffff, 0x7ffffff,
    0xfffffff, 0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff };
struct CBitstream {
    long m_bytePos; long m_bitPos; unsigned char* m_pBuffer; unsigned char* m_pCurrent; long m_capacity;
    void WriteBits(unsigned long value, long count)
    {
        if (m_bytePos + 4 >= m_capacity) {
            m_pBuffer = (unsigned char*)std::realloc(m_pBuffer, m_capacity + 0x100);
            std::memset(m_pBuffer + m_capacity, 0, 0x100);
            m_capacity += 0x100;
            m_pCurrent = m_pBuffer + m_bytePos;
        }
        value &= g_bitMasks[count];
        unsigned char low = (unsigned char)(value << m_bitPos);
        count += m_bitPos;
        *m_pCurrent |= low;
        if (count >= 8) {
            m_pCurrent[1] = (unsigned char)(value >> (8 - m_bitPos));
            if (count >= 16) {
                m_pCurrent[2] = (unsigned char)(value >> (16 - m_bitPos));
                if (count >= 24) {
                    m_pCurrent[3] = (unsigned char)(value >> (24 - m_bitPos));
                    if (count >= 32) {
                        if (m_bitPos) m_pCurrent[4] = (unsigned char)(value >> (32 - m_bitPos));
                        else m_pCurrent[4] = 0;
                    }
                }
            }
        }
        long bytes = count / 8;
        m_bitPos = count & 7;
        m_bytePos += bytes;
        m_pCurrent += bytes;
    }
};
int main()
{
    CBitstream b = { 0, 0, 0, 0, 0 };
    b.WriteBits(0x5, 3);        // 101
    b.WriteBits(0xff3, 4);      // masked to 0011
    b.WriteBits(0x1, 1);        // completes byte 0: 1 0011 101 = 0x9d
    b.WriteBits(0xabcd, 16);
    b.WriteBits(0x3, 2); b.WriteBits(0x12345678, 32);
    if (b.m_pBuffer[0] != 0x9d || b.m_pBuffer[1] != 0xcd || b.m_pBuffer[2] != 0xab) { std::printf("00c90fc0_TEST FAIL\n"); return 1; }
    if (b.m_pBuffer[3] != (unsigned char)(0x3 | (0x78 << 2)) || b.m_bytePos != 7 || b.m_bitPos != 2 || b.m_capacity != 0x100) { std::printf("00c90fc0_TEST FAIL\n"); return 1; }
    std::printf("00c90fc0_TEST PASS\n");
    return 0;
}
