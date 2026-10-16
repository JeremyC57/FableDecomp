#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
static int g_overflow = 0;
struct CMemoryDataInputStream {
    unsigned long m_position; const unsigned char* m_pData; long m_remaining;
    void ReadWithSrcChunkOverflow(unsigned char*, long) { ++g_overflow; }
    void Read(void* buffer, long size)
    {
        i64 np = (i64)(unsigned long)(m_position + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (m_remaining >= size) { std::memcpy(buffer, m_pData, size); m_pData += size; m_remaining -= size; m_position += size; }
        else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};
struct Block { int Version; int Pos; int Crc;
    bool Load(CMemoryDataInputStream& s, long n) { if (n != 8) return false; s.Read(this, 8); return true; } };
int main()
{
    unsigned char data[8] = { 3, 0, 0, 0, 0x10, 0, 0, 0 };
    CMemoryDataInputStream s; s.m_position = 4; s.m_pData = data; s.m_remaining = 8;
    Block b; b.Version = 0; b.Pos = 0; b.Crc = 7;
    if (b.Load(s, 4)) { std::printf("00b6d6e0_TEST FAIL\n"); return 1; }
    if (!b.Load(s, 8) || b.Version != 3 || b.Pos != 16 || b.Crc != 7 || s.m_remaining != 0 || s.m_position != 12)
        { std::printf("00b6d6e0_TEST FAIL\n"); return 1; }
    if (!b.Load(s, 8) || g_overflow != 1) { std::printf("00b6d6e0_TEST FAIL\n"); return 1; }
    s.m_position = 0x7ffffffc; if (!b.Load(s, 8) || g_overflow != 1) { std::printf("00b6d6e0_TEST FAIL\n"); return 1; }
    std::printf("00b6d6e0_TEST PASS\n");
    return 0;
}
