// Behaviour test for 0x00C3A9E0 CReaderBase_00c4b300::Read
#include <cstdio>
#include <cstring>
struct CReader {
    const unsigned char* m_source;
    bool m_fail;
    int m_resets;
    unsigned long m_segmentStart;
    unsigned long m_segmentSize;
    unsigned long m_position;
    void Reset() { ++m_resets; m_segmentStart = 0; m_segmentSize = 0; m_position = 0; }
    bool ReadBlock(void* buffer, unsigned long size)
    {
        if (m_fail) return false;
        std::memcpy(buffer, m_source + m_position, size);
        return true;
    }
    long Read(void* buffer, unsigned long size)
    {
        unsigned long remaining = m_segmentSize - m_position;
        if (size > remaining)
            size = remaining;
        if (size == 0)
            return 0;
        if (!ReadBlock(buffer, size)) {
            Reset();
            return -1;
        }
        m_position += size;
        return size;
    }
};
int main()
{
    const unsigned char data[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    unsigned char out[16];
    CReader r; r.m_source = data; r.m_fail = false; r.m_resets = 0;
    r.m_segmentStart = 0; r.m_segmentSize = 10; r.m_position = 0;
    int ok = r.Read(out, 4) == 4 && out[3] == 4 && r.m_position == 4;
    ok = ok && r.Read(out, 8) == 6 && out[0] == 5 && out[5] == 10 && r.m_position == 10;
    ok = ok && r.Read(out, 8) == 0 && r.m_resets == 0;
    r.m_position = 2; r.m_fail = true;
    ok = ok && r.Read(out, 3) == -1 && r.m_resets == 1 && r.m_position == 0 && r.m_segmentSize == 0;
    if (!ok) { std::printf("00c3a9e0_TEST FAIL\n"); return 1; }
    std::printf("00c3a9e0_TEST PASS\n");
    return 0;
}