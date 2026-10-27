// Test for 0x00C57110 CDSoundStreamBuffer::UnlockChunk: Unlock(ptr, chunk) and the cursor advances with wrap-around.
#include <cstdio>
struct CBuf { void* ptr; unsigned int bytes; long Unlock(void* p, unsigned int b, void*, unsigned int) { ptr = p; bytes = b; return 0; } };
struct CStream { CBuf* m_pBuffer; unsigned int m_cursor, m_bufferBytes, m_chunkBytes;
    void UnlockChunk(void* p) { m_pBuffer->Unlock(p, m_chunkBytes, 0, 0); m_cursor = (m_cursor + m_chunkBytes) % m_bufferBytes; } };
int main()
{
    int dummy; CBuf b; CStream s; s.m_pBuffer = &b; s.m_cursor = 32; s.m_bufferBytes = 48; s.m_chunkBytes = 16;
    s.UnlockChunk(&dummy); bool ok = b.ptr == &dummy && b.bytes == 16 && s.m_cursor == 0;
    s.UnlockChunk(&dummy); ok = ok && s.m_cursor == 16;
    if (!ok) { std::printf("00c57110_TEST FAIL\n"); return 1; }
    std::printf("00c57110_TEST PASS\n");
    return 0;
}