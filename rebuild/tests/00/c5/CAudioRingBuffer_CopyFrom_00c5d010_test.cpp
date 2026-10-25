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
    CAudioRingBuffer dst, src; Setup(dst, g_a, 16, 3); Setup(src, g_b, 8, 2);
    for (int i = 0; i < 48; ++i) g_a[i] = 0;
    for (int i = 0; i < 16; ++i) g_b[i] = (short)(100 + i);
    src.m_readPos = 2; dst.m_writePos = 10;
    dst.CopyFrom(&src, 3);                            // channels 0 and 1, frames 2..4 -> 10..12
    int ok = g_a[10] == 102 && g_a[12] == 104 && g_a[13] == 0 && g_a[9] == 0
          && g_a[16 + 10] == 110 && g_a[16 + 12] == 112 && g_a[32 + 10] == 0;
    if (!ok) { std::printf("00c5d010_TEST FAIL\n"); return 1; }
    std::printf("00c5d010_TEST PASS\n");
    return 0;
}