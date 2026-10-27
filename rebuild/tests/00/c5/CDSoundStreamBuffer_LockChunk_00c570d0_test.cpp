// Test for 0x00C570D0 CDSoundStreamBuffer::LockChunk: Lock(cursor, chunk) and the first region, 0 on a failing HRESULT.
#include <cstdio>
struct CBuf { long hr; unsigned int off, bytes; unsigned char mem[64];
    long Lock(unsigned int o, unsigned int b, void** p1, unsigned int* b1, void** p2, unsigned int* b2, unsigned int) {
        off = o; bytes = b; *p1 = mem + o; *b1 = b; *p2 = 0; *b2 = 0; return hr; } };
struct CStream { CBuf* m_pBuffer; unsigned int m_cursor, m_bufferBytes, m_chunkBytes;
    void* LockChunk() { void* p1; unsigned int b1; void* p2; unsigned int b2;
        long hr = m_pBuffer->Lock(m_cursor, m_chunkBytes, &p1, &b1, &p2, &b2, 0); if (hr < 0) return 0; return p1; } };
int main()
{
    CBuf b; b.hr = 0; CStream s; s.m_pBuffer = &b; s.m_cursor = 16; s.m_bufferBytes = 64; s.m_chunkBytes = 16;
    bool ok = s.LockChunk() == b.mem + 16 && b.off == 16 && b.bytes == 16;
    b.hr = -1; ok = ok && s.LockChunk() == 0;
    if (!ok) { std::printf("00c570d0_TEST FAIL\n"); return 1; }
    std::printf("00c570d0_TEST PASS\n");
    return 0;
}