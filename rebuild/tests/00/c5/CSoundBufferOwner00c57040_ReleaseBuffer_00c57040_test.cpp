#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[128] = "";
struct IBuffer { virtual unsigned long Release() { std::strcat(g_seq, "R"); return 0; } virtual long Stop() { std::strcat(g_seq, "S"); return 0; } };
struct COwner { unsigned char _pad[4]; IBuffer* m_pBuffer; unsigned char _pad08[8]; long m_playing;
    void ReleaseBuffer() { if (m_pBuffer) { m_pBuffer->Stop(); m_pBuffer->Release(); m_pBuffer = 0; } m_playing = 0; } };

int main()
{
    IBuffer buf; COwner o; o.m_pBuffer = &buf; o.m_playing = 5; o.ReleaseBuffer(); if (std::strcmp(g_seq, "SR") || o.m_pBuffer || o.m_playing) { std::printf("00c57040_TEST FAIL\n"); return 1; }
    o.m_playing = 5; o.ReleaseBuffer(); if (std::strcmp(g_seq, "SR") || o.m_playing) { std::printf("00c57040_TEST FAIL\n"); return 1; }
    std::printf("00c57040_TEST PASS\n");
    return 0;
}