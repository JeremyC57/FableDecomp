// Behaviour test for 0x006ffa40 CReactionFrequencyTraits_LinearTime::TransferBinaryIn
#include <cstdio>
#include <cstring>

class CMemoryDataInputStream { public:
    unsigned long m_position;
    const unsigned char* m_pData;
    long m_remaining;
    int overflowCalls;
    float overflowValue;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++overflowCalls;
        std::memcpy(buffer, &overflowValue, size);
        m_position += size;
    }
    static bool IsValidPosition(long long position) { return position >= 0 && position <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(m_position + size);
        if (!IsValidPosition(newPosition))
            return;
        if (m_remaining >= size) {
            std::memcpy(buffer, m_pData, size);
            m_pData += size;
            m_remaining -= size;
            m_position += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

struct CReactionFrequencyTraits_LinearTime {
    float MinWait;
    float WaitRange;
    unsigned long FrameHappened;
    void TransferBinaryIn(CMemoryDataInputStream& stream)
    {
        float value;
        stream.Read(&value, sizeof(value));
        MinWait = value;
        stream.Read(&value, sizeof(value));
        WaitRange = value;
    }
};

int main()
{
    float src[2] = { 1.5f, 7.25f };
    CMemoryDataInputStream s;
    s.m_position = 10; s.m_pData = (const unsigned char*)src; s.m_remaining = 8; s.overflowCalls = 0; s.overflowValue = 0.0f;
    CReactionFrequencyTraits_LinearTime t; t.MinWait = 0.0f; t.WaitRange = 0.0f; t.FrameHappened = 77;
    t.TransferBinaryIn(s);
    float a = 1.5f, b = 7.25f;
    if (t.MinWait != a || t.WaitRange != b || s.m_position != 18 || s.m_remaining != 0 || s.overflowCalls != 0 || t.FrameHappened != 77) {
        std::printf("006ffa40_TEST FAIL\n"); return 1;
    }
    // second value comes from the overflow reader once the chunk runs out
    CMemoryDataInputStream s2;
    s2.m_position = 0; s2.m_pData = (const unsigned char*)src; s2.m_remaining = 4; s2.overflowCalls = 0; s2.overflowValue = 3.0f;
    t.TransferBinaryIn(s2);
    float c = 3.0f;
    if (t.MinWait != a || t.WaitRange != c || s2.overflowCalls != 1) { std::printf("006ffa40_TEST FAIL\n"); return 1; }
    std::printf("006ffa40_TEST PASS\n");
    return 0;
}