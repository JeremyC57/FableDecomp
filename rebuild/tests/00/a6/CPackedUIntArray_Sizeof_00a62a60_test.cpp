#include <cstdio>

// CPackedUIntArray::Sizeof: 0x10 header bytes plus the packed storage rounded up to whole words.
struct CPackedUIntArray {
    unsigned long* m_pData;
    unsigned long  m_count;
    unsigned char  m_bitsPerEntry;
    unsigned long  m_capacity;
    unsigned long Sizeof() const;
};
unsigned long CPackedUIntArray::Sizeof() const
{
    return 0x10 + ((m_bitsPerEntry * m_count + 31) >> 5) * 4;
}

int main()
{
    CPackedUIntArray a;
    a.m_pData = 0; a.m_capacity = 0;
    a.m_count = 0;  a.m_bitsPerEntry = 5;
    bool ok = a.Sizeof() == 0x10;
    a.m_count = 1;                                  // 5 bits -> one word
    ok = ok && a.Sizeof() == 0x14;
    a.m_count = 7;  a.m_bitsPerEntry = 9;           // 63 bits -> two words
    ok = ok && a.Sizeof() == 0x18;
    a.m_count = 32; a.m_bitsPerEntry = 32;          // 1024 bits -> 32 words
    ok = ok && a.Sizeof() == 0x10 + 32 * 4;
    if (!ok) {
        std::printf("00a62a60_TEST FAIL\n");
        return 1;
    }
    std::printf("00a62a60_TEST PASS\n");
    return 0;
}