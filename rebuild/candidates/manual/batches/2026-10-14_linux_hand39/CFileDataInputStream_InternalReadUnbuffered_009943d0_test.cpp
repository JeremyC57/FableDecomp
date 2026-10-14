#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = ""; static unsigned char g_buf[16]; static long g_seek = -1; static unsigned char* g_readTo = 0; static long g_readLen = -1; static long g_readFlags = -1;
struct CSourceStream_00994360 { virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void Read(unsigned char* b, long l, long f) { g_readTo = b; g_readLen = l; g_readFlags = f; std::strcat(g_seq, "R"); } virtual void v04() { } virtual void Seek(long p) { g_seek = p; std::strcat(g_seq, "S"); } };
struct CFileDataInputStream { virtual void v00() { } virtual void v01() { } virtual long GetPosition() { std::strcat(g_seq, "P"); return 30; } unsigned char _pad04[0x14]; CSourceStream_00994360* m_pSource;
    void InternalReadUnbuffered(unsigned char* buffer, long length) { long position = GetPosition(); m_pSource->Seek(position); m_pSource->Read(buffer, length, 0); } };
int main() {
    CSourceStream_00994360 src; CFileDataInputStream s; s.m_pSource = &src; s.InternalReadUnbuffered(g_buf, 5);
    if (std::strcmp(g_seq, "PSR") || g_seek != 30 || g_readTo != g_buf || g_readLen != 5 || g_readFlags != 0) { std::printf("009943d0_TEST FAIL\n"); return 1; }
    std::printf("009943d0_TEST PASS\n"); return 0;
}
