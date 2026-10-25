#include <cstdio>
struct IDirectSoundBuffer {
    unsigned int play; int hr;
    int GetCurrentPosition(unsigned int* p, unsigned int* w) { *p = play; *w = play + 64; return hr; }
};
struct CDSoundStreamBuffer {
    IDirectSoundBuffer* m_pBuffer; int m_8; unsigned int m_cursor; unsigned int m_bufferBytes; unsigned int m_chunkBytes;
    bool IsChunkReady()
    {
        unsigned int play, write;
        if (m_pBuffer->GetCurrentPosition(&play, &write) != 0)
            return false;
        unsigned int played;
        if (m_cursor <= play)
            played = play - m_cursor;
        else
            played = m_bufferBytes - m_cursor + play;
        if (played < m_chunkBytes)
            return false;
        return true;
    }
};
#define CHECK(x) do { if (!(x)) { std::printf("00c57070_TEST FAIL\n"); return 1; } } while (0)
int main()
{
    IDirectSoundBuffer b; b.hr = 0;
    CDSoundStreamBuffer s; s.m_pBuffer = &b; s.m_8 = 0; s.m_bufferBytes = 1000; s.m_chunkBytes = 100; s.m_cursor = 200;
    b.play = 299; CHECK(!s.IsChunkReady());
    b.play = 300; CHECK(s.IsChunkReady());
    s.m_cursor = 950; b.play = 49; CHECK(!s.IsChunkReady());
    b.play = 50; CHECK(s.IsChunkReady());
    b.hr = (int)0x88780096; CHECK(!s.IsChunkReady());
    std::printf("00c57070_TEST PASS\n");
    return 0;
}