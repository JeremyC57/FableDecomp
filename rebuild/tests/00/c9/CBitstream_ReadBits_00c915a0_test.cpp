// Behaviour test for 00c915a0: body copied from src/00c915a0.cpp (CBitstream::ReadBits), the bit-mask table supplied.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct CBitstream {
    long m_bytePosition;
    long m_bitOffset;
    long m_unknown08;
    const unsigned char* m_pData;
    long m_size;
};
unsigned long s_BitMasks[33];
unsigned long __fastcall ReadBits_00c915a0(CBitstream* stream, long count)
{
    unsigned long mask = s_BitMasks[count];
    long offset = stream->m_bitOffset;
    count += offset;
    unsigned long value;
    if (stream->m_bytePosition + 4 >= stream->m_size) {
        value = 0xffffffff;
        if (count + stream->m_bytePosition * 8 > stream->m_size * 8)
            goto advance;
    }
    {
        const unsigned char* p = stream->m_pData;
        value = p[0] >> stream->m_bitOffset;
        if (count > 8) {
            value |= p[1] << (8 - offset);
            if (count > 16) {
                value |= p[2] << (16 - offset);
                if (count > 24) {
                    value |= p[3] << (24 - offset);
                    if (count > 32 && offset != 0)
                        value |= p[4] << (32 - offset);
                }
            }
        }
        value &= mask;
    }
advance:
    stream->m_pData += count / 8;
    stream->m_bytePosition += count / 8;
    stream->m_bitOffset = count & 7;
    return value;
}
static unsigned char g_buf[8] = { 0xB5, 0x6C, 0x3A, 0xF1, 0x9E, 0x27, 0x80, 0x01 };
static void reset(CBitstream& s, long bytePos, long bitOff, long size)
{
    s.m_bytePosition = bytePos; s.m_bitOffset = bitOff; s.m_unknown08 = 0; s.m_pData = g_buf + bytePos; s.m_size = size;
}
int main()
{
    for (int i = 0; i < 33; ++i) s_BitMasks[i] = (i == 32) ? 0xffffffffUL : ((1UL << i) - 1);
    CBitstream s;
    // 1. 3 bits from offset 0: low bits of 0xB5 (101) = 5; advance stays in byte 0
    reset(s, 0, 0, 8);
    CHECK(ReadBits_00c915a0(&s, 3) == 5); CHECK(s.m_bytePosition == 0); CHECK(s.m_bitOffset == 3); CHECK(s.m_pData == g_buf);
    // 2. next 8 bits straddle bytes 0/1: (0xB5 >> 3) | (0x6C << 5) & 0xff = 0x16 | 0x80 = 0x96
    CHECK(ReadBits_00c915a0(&s, 8) == 0x96); CHECK(s.m_bytePosition == 1); CHECK(s.m_bitOffset == 3); CHECK(s.m_pData == g_buf + 1);
    // 3. 32 bits straddling five bytes from offset 3 at byte 1
    unsigned long expect = (0x6CUL >> 3) | (0x3AUL << 5) | (0xF1UL << 13) | (0x9EUL << 21) | (0x27UL << 29);
    CHECK(ReadBits_00c915a0(&s, 32) == expect); CHECK(s.m_bytePosition == 5); CHECK(s.m_bitOffset == 3);
    // 4. 16 bits exactly aligned (offset 0): bytes 2,3 -> 0xF13A; the fifth-byte read is skipped (offset 0)
    reset(s, 2, 0, 8); CHECK(ReadBits_00c915a0(&s, 16) == 0xF13A); CHECK(s.m_bytePosition == 4); CHECK(s.m_bitOffset == 0);
    // 5. 32 aligned bits near the end: pos 4, size 8 -> pos+4 >= size but bits fit (64 <= 64): read happens
    reset(s, 4, 0, 8); CHECK(ReadBits_00c915a0(&s, 32) == 0x0180279EUL); CHECK(s.m_bytePosition == 8);
    // 6. past the end: pos 7 offset 4, 8 bits -> 68 > 64: -1, but the stream still advances
    reset(s, 7, 4, 8); CHECK(ReadBits_00c915a0(&s, 8) == 0xffffffffUL); CHECK(s.m_bytePosition == 8); CHECK(s.m_bitOffset == 4); CHECK(s.m_pData == g_buf + 8);
    // 7. 1 bit with 24 bits (count 24 > 16, <= 24 arm): offset 0 at byte 0: 0x3A6CB5
    reset(s, 0, 0, 8); CHECK(ReadBits_00c915a0(&s, 24) == 0x3A6CB5UL); CHECK(s.m_bytePosition == 3);
    // 8. 0 bits: nothing read, mask 0
    reset(s, 0, 5, 8); CHECK(ReadBits_00c915a0(&s, 0) == 0); CHECK(s.m_bitOffset == 5);
    if (fails == 0) printf("00c915a0_TEST PASS\n");
    else printf("00c915a0_TEST FAIL (%d)\n", fails);
    return fails;
}