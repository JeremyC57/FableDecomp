// Behaviour test for 0x00C59770 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct SVoiceFlags { unsigned int m_bit0 : 1; unsigned int m_bits1to4 : 4; unsigned int m_bit5 : 1; unsigned int m_rest : 26; };
struct SVoiceRequests { unsigned int m_bits0to6 : 7; unsigned int m_bit7 : 1; unsigned int m_bit8 : 1; unsigned int m_bit9 : 1; unsigned int m_rest : 22; };

struct V { i64 m_endPosition; SVoiceFlags m_flags; SVoiceRequests m_requests;
    int CheckPosition(i64 position)
    {
        m_flags.m_bit0 = 1;
        if (position >= m_endPosition) { m_requests.m_bit8 = 0; return 2; }
        return 0x10;
    } };
int main()
{
    V v; std::memset(&v, 0, sizeof(v)); v.m_endPosition = ((i64)1 << 32) + 5; v.m_requests.m_bit8 = 1;
    if (v.CheckPosition(7) != 0x10 || !v.m_flags.m_bit0 || !v.m_requests.m_bit8) { std::printf("00c59770_TEST FAIL\n"); return 1; }
    if (v.CheckPosition(((i64)1 << 32) + 4) != 0x10) { std::printf("00c59770_TEST FAIL\n"); return 1; }
    if (v.CheckPosition(((i64)1 << 32) + 5) != 2 || v.m_requests.m_bit8) { std::printf("00c59770_TEST FAIL\n"); return 1; }
    if (v.CheckPosition(-1) != 0x10) { std::printf("00c59770_TEST FAIL\n"); return 1; }
    std::printf("00c59770_TEST PASS\n");
    return 0;
}