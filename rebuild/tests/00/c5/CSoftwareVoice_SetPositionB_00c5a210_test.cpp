// Behaviour test for 0x00C5A210 (standalone copy of the model and logic)
#include <cstdio>
#include <cstring>
#ifdef _MSC_VER
typedef __int64 i64;
#else
typedef long long i64;
#endif
struct SVoiceFlags { unsigned int m_bit0 : 1; unsigned int m_bits1to4 : 4; unsigned int m_bit5 : 1; unsigned int m_rest : 26; };
struct SVoiceRequests { unsigned int m_bits0to6 : 7; unsigned int m_bit7 : 1; unsigned int m_bit8 : 1; unsigned int m_bit9 : 1; unsigned int m_rest : 22; };

struct V { unsigned int m_positionBLow, m_positionBHigh; SVoiceRequests m_requests;
    bool SetPositionB(unsigned int low, unsigned int high)
    {
        if (low == 0 && high == 0) { m_requests.m_bit7 = 0; m_requests.m_bit8 = 1; return true; }
        m_requests.m_bit7 = 1; m_positionBLow = low; m_positionBHigh = high; return false;
    } };
int main()
{
    V v; std::memset(&v, 0, sizeof(v));
    if (v.SetPositionB(5, 6) || !v.m_requests.m_bit7 || v.m_positionBLow != 5 || v.m_positionBHigh != 6) { std::printf("00c5a210_TEST FAIL\n"); return 1; }
    if (v.SetPositionB(0, 1) || v.m_positionBHigh != 1) { std::printf("00c5a210_TEST FAIL\n"); return 1; }
    if (!v.SetPositionB(0, 0) || v.m_requests.m_bit7 || !v.m_requests.m_bit8 || v.m_positionBLow != 0) { std::printf("00c5a210_TEST FAIL\n"); return 1; }
    std::printf("00c5a210_TEST PASS\n");
    return 0;
}