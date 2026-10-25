// Behaviour test for 00c356f0: CSampleHeader::GetFormat -- descriptor filled from the header; unknown kinds fail with -1
#include <cstdio>
struct SAudioFormatDesc {
    void* m_pData; long m_reserved4; long m_reserved8;
    unsigned short m_channels; unsigned short m_kind; unsigned int m_sampleRate;
};
struct CSampleHeader {
    unsigned short m_channels; unsigned short m_sampleRate; unsigned char m_kind;
    long GetFormat(SAudioFormatDesc* out, void* data) const
    {
        out->m_pData = data; out->m_reserved4 = 0; out->m_reserved8 = 0;
        switch (m_kind) {
        case 0: out->m_kind = 0; break;
        case 1: out->m_kind = 1; break;
        case 2: out->m_kind = 2; break;
        default: return -1;
        }
        out->m_channels = m_channels;
        out->m_sampleRate = m_sampleRate;
        return 0;
    }
};
int main()
{
    int data = 0;
    CSampleHeader h; h.m_channels = 2; h.m_sampleRate = 44100; h.m_kind = 2;
    SAudioFormatDesc d; d.m_reserved4 = 7; d.m_reserved8 = 7; d.m_kind = 9; d.m_channels = 1; d.m_sampleRate = 1;
    if (h.GetFormat(&d, &data) != 0 || d.m_pData != &data || d.m_reserved4 != 0 || d.m_reserved8 != 0 || d.m_kind != 2 || d.m_channels != 2 || d.m_sampleRate != 44100) { std::printf("00c356f0_TEST FAIL\n"); return 1; }
    h.m_kind = 3; d.m_channels = 5;
    if (h.GetFormat(&d, 0) != -1 || d.m_pData != 0 || d.m_channels != 5) { std::printf("00c356f0_TEST FAIL\n"); return 1; }
    std::printf("00c356f0_TEST PASS\n");
    return 0;
}