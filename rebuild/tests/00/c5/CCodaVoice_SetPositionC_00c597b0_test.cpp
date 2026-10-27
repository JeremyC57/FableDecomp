// Behaviour test for 0x00C597B0 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct SVoiceFlags { unsigned int m_bit0 : 1; unsigned int m_bits1to4 : 4; unsigned int m_bit5 : 1; unsigned int m_rest : 26; };
struct SVoiceRequests { unsigned int m_bits0to6 : 7; unsigned int m_bit7 : 1; unsigned int m_bit8 : 1; unsigned int m_bit9 : 1; unsigned int m_rest : 22; };

struct V { i64 m_positionC; SVoiceFlags m_flags; SVoiceRequests m_requests;
    unsigned char GetFlag5() const { return (unsigned char)m_flags.m_bit5; }
    void SetPositionC(i64 position) { if (!GetFlag5()) { m_requests.m_bit9 = 1; m_positionC = position; } } };
int main()
{
    V v; std::memset(&v, 0, sizeof(v));
    v.SetPositionC(((i64)3 << 32) | 9);
    if (v.m_positionC != (((i64)3 << 32) | 9) || !v.m_requests.m_bit9) { std::printf("00c597b0_TEST FAIL\n"); return 1; }
    v.m_flags.m_bit5 = 1; v.m_requests.m_bit9 = 0;
    v.SetPositionC(1);
    if (v.m_positionC != (((i64)3 << 32) | 9) || v.m_requests.m_bit9) { std::printf("00c597b0_TEST FAIL\n"); return 1; }
    std::printf("00c597b0_TEST PASS\n");
    return 0;
}