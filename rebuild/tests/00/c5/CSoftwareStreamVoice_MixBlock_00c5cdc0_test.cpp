// Behaviour test for 0x00C5CDC0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>

struct CAudioRingBuffer { unsigned char* m_pData; unsigned int m_capacity, m_frameBytes, m_channels, m_readPos;
    unsigned char* ReadPointer() const { return m_pData + m_readPos * m_frameBytes; } };
struct V { const short* last; unsigned int lastChannels; int mono, multi;
    void MixMono(const short* src) { last = src; mono++; }
    void Mix(const short* src, unsigned int channels) { last = src; lastChannels = channels; multi++; }
    void MixBlock(void* mixer, CAudioRingBuffer* ring, unsigned int channels)
    {
        if (channels == 1) MixMono((const short*)ring->ReadPointer());
        else Mix((const short*)ring->ReadPointer(), channels);
    } };
int main()
{
    static unsigned char mem[1024];
    CAudioRingBuffer r; r.m_pData = mem; r.m_frameBytes = 4; r.m_readPos = 10; r.m_capacity = 128; r.m_channels = 2;
    V v; v.mono = 0; v.multi = 0; v.last = 0; v.lastChannels = 0;
    v.MixBlock(0, &r, 1);
    if (v.mono != 1 || v.multi != 0 || (const unsigned char*)v.last != mem + 40) { std::printf("00c5cdc0_TEST FAIL\n"); return 1; }
    v.MixBlock(0, &r, 2);
    if (v.multi != 1 || v.lastChannels != 2 || (const unsigned char*)v.last != mem + 40) { std::printf("00c5cdc0_TEST FAIL\n"); return 1; }
    std::printf("00c5cdc0_TEST PASS\n");
    return 0;
}