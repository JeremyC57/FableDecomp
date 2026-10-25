#include <cstdio>
#include <cstring>
struct CAudioRingBuffer {
    unsigned char* m_pData;
    unsigned int m_capacity, m_frameBytes, m_channels, m_readPos, m_writePos, m_queued;
    void CopyChannel(unsigned int dstChannel, unsigned int srcChannel)
    {
        unsigned int stride = m_frameBytes * m_capacity;
        std::memcpy(m_pData + dstChannel * stride, m_pData + srcChannel * stride, stride);
    }
    void ClearChannel(unsigned int channel, unsigned int frames)
    {
        std::memset(m_pData + (m_capacity * channel + m_writePos) * m_frameBytes, 0, m_frameBytes * frames);
    }
    void CopyFrom(const CAudioRingBuffer* src, unsigned int frames)
    {
        const unsigned char* from = src->m_pData + src->m_readPos * src->m_frameBytes;
        unsigned char* to = m_pData + m_writePos * m_frameBytes;
        unsigned int fromStride = src->m_capacity * m_frameBytes;
        unsigned int toStride = m_capacity * m_frameBytes;
        unsigned int bytes = m_frameBytes * frames;
        for (unsigned int c = 0; c < src->m_channels; ++c) {
            std::memcpy(to, from, bytes);
            from += fromStride;
            to += toStride;
        }
    }
};
static short g_a[3 * 16], g_b[2 * 8];
static void Setup(CAudioRingBuffer& r, short* data, unsigned int cap, unsigned int ch)
{ std::memset(&r, 0, sizeof(r)); r.m_pData = (unsigned char*)data; r.m_capacity = cap; r.m_frameBytes = 2; r.m_channels = ch; }
int main()
{
    CAudioRingBuffer r; Setup(r, g_a, 16, 3);
    for (int i = 0; i < 48; ++i) g_a[i] = (short)i;
    r.CopyChannel(0, 2);
    int ok = g_a[0] == 32 && g_a[15] == 47 && g_a[16] == 16 && g_a[32] == 32;
    if (!ok) { std::printf("00c5cf90_TEST FAIL\n"); return 1; }
    std::printf("00c5cf90_TEST PASS\n");
    return 0;
}