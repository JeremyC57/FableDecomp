// Behaviour test for SBufferedStreamState00c8d970::Reset @ 0x00C8D970: cursors/counters cleared, current entry -1, buffers kept.
#include <cstdio>

struct SBufferedStreamState00c8d970 {
    unsigned char* m_pBuffer; unsigned long m_bufferSize, m_readPosition, m_writePosition;
    unsigned char* m_pWorkBuffer; void* m_pEntries; unsigned long m_entryCapacity, m_entryCount, m_counter20, m_counter24;
    unsigned char _table28[0x11c];
    unsigned long m_counter144, m_counter148, m_counter14c, m_owner; long m_currentEntry;
    unsigned long m_counter158, m_counter15c, m_counter160, m_counter164;
    void Reset()
    {
        m_readPosition = 0; m_writePosition = 0; m_entryCount = 0; m_counter20 = 0; m_counter24 = 0;
        m_counter144 = 0; m_counter148 = 0; m_counter14c = 0; m_currentEntry = -1;
        m_counter158 = 0; m_counter15c = 0; m_counter160 = 0; m_counter164 = 0;
    }
};

int main()
{
    static unsigned char buffer[16], work[16];
    SBufferedStreamState00c8d970 s;
    s.m_pBuffer = buffer; s.m_bufferSize = 0x4000; s.m_pWorkBuffer = work; s.m_pEntries = work; s.m_entryCapacity = 0x400; s.m_owner = 55;
    s.m_readPosition = 1; s.m_writePosition = 2; s.m_entryCount = 3; s.m_counter20 = 4; s.m_counter24 = 5; s.m_counter144 = 6;
    s.m_counter148 = 7; s.m_counter14c = 8; s.m_currentEntry = 9; s.m_counter158 = 10; s.m_counter15c = 11; s.m_counter160 = 12; s.m_counter164 = 13;
    s.Reset();
    if (s.m_readPosition || s.m_writePosition || s.m_entryCount || s.m_counter20 || s.m_counter24 || s.m_counter144 || s.m_counter148 ||
        s.m_counter14c || s.m_currentEntry != -1 || s.m_counter158 || s.m_counter15c || s.m_counter160 || s.m_counter164 ||
        s.m_pBuffer != buffer || s.m_bufferSize != 0x4000 || s.m_pWorkBuffer != work || s.m_entryCapacity != 0x400 || s.m_owner != 55) {
        std::printf("00c8d970_TEST FAIL\n"); return 1;
    }
    std::printf("00c8d970_TEST PASS\n");
    return 0;
}