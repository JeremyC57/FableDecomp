#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = ""; static unsigned char g_buf[16]; static long g_seek = -1; static unsigned char* g_readTo = 0; static long g_readLen = -1; static long g_readFlags = -1;
struct CSourceStream_00994360 { virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void Read(unsigned char* b, long l, long f) { g_readTo = b; g_readLen = l; g_readFlags = f; std::strcat(g_seq, "R"); } virtual void v04() { } virtual void Seek(long p) { g_seek = p; std::strcat(g_seq, "S"); } };
struct CFileDataInputStream { virtual void v00() { } virtual void v01() { } virtual long GetPosition() { std::strcat(g_seq, "P"); return 30; } unsigned char _pad04[4]; unsigned long m_end; unsigned char _pad0c[0xc]; CSourceStream_00994360* m_pSource; unsigned char* m_pBuffer; unsigned long m_chunkSize;
    void InternalReadNextSourceChunk(const unsigned char*& chunk, long& length) { long position = GetPosition(); unsigned long remaining = m_end - position; if (m_chunkSize < remaining) remaining = m_chunkSize; length = remaining; m_pSource->Seek(position); m_pSource->Read(m_pBuffer, length, 0); chunk = m_pBuffer; } };
int main() {
    CSourceStream_00994360 src; CFileDataInputStream s; s.m_end = 100; s.m_pSource = &src; s.m_pBuffer = g_buf; s.m_chunkSize = 8; const unsigned char* chunk = 0; long len = 0; s.InternalReadNextSourceChunk(chunk, len);
    if (std::strcmp(g_seq, "PSR") || g_seek != 30 || len != 8 || g_readTo != g_buf || g_readLen != 8 || g_readFlags != 0 || chunk != g_buf) { std::printf("00994360_TEST FAIL\n"); return 1; }
    s.m_end = 33; s.InternalReadNextSourceChunk(chunk, len); if (len != 3 || g_readLen != 3) { std::printf("00994360_TEST FAIL\n"); return 1; }
    std::printf("00994360_TEST PASS\n"); return 0;
}
