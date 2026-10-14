#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

struct CCodecState { long m_0, m_4, m_8, m_c, m_10, m_14, m_18, m_1c, m_20, m_24, m_28, m_2c, m_30, m_34, m_38, m_3c, m_40, m_44;
    void Reset() { std::memset(this, 0, sizeof(*this)); m_28 = 1; m_20 = 1; } };
int main()
{
    CCodecState s; std::memset(&s, 0xee, sizeof(s));
    s.Reset();
    bool ok = sizeof(CCodecState) == 0x48 && s.m_0 == 0 && s.m_1c == 0 && s.m_20 == 1 && s.m_24 == 0 && s.m_28 == 1 && s.m_2c == 0 && s.m_44 == 0;
    if (!ok) { std::printf("00cb1ac0_TEST FAIL\n"); return 1; }
    std::printf("00cb1ac0_TEST PASS\n"); return 0;
}
