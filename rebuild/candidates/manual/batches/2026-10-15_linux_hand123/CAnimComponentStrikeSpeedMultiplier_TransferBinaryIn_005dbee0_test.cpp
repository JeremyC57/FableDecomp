// Behaviour test for CAnimComponentStrikeSpeedMultiplier::TransferBinaryIn (0x005DBEE0): one float
// read through the inlined stream Read (fast chunk path and overflow path) into the member.
#include <cstdio>
#include <cstring>

struct CDataInputStream {
    unsigned int m_position;
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
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(unsigned int)(m_position + size);
        if (newPosition < 0 || newPosition > 0x7fffffff)
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

struct CAnimComponentStrikeSpeedMultiplier {
    float AnimationSpeedMultiplier;
    void TransferBinaryIn(CDataInputStream& in)
    {
        float value;
        in.Read(&value, sizeof(value));
        AnimationSpeedMultiplier = value;
    }
};

int main()
{
    float f = 1.5f;
    unsigned char buf[8];
    std::memcpy(buf, &f, 4);
    CDataInputStream in = { 0, buf, 8, 0, 0.0f };
    CAnimComponentStrikeSpeedMultiplier c;
    c.AnimationSpeedMultiplier = 0.0f;
    c.TransferBinaryIn(in);
    if (c.AnimationSpeedMultiplier != 1.5f || in.m_position != 4 || in.m_remaining != 4 || in.m_pData != buf + 4) {
        std::printf("005dbee0_TEST FAIL\n"); return 1;
    }
    CDataInputStream in2 = { 0, buf, 2, 0, 2.25f };
    c.TransferBinaryIn(in2);
    if (c.AnimationSpeedMultiplier != 2.25f || in2.overflowCalls != 1) {
        std::printf("005dbee0_TEST FAIL\n"); return 1;
    }
    std::printf("005dbee0_TEST PASS\n");
    return 0;
}
