#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#endif

struct CChannelState { long m_0; unsigned char m_block[0x160]; long m_164; long m_168, m_16c, m_170; long m_174;
    void Clear() { std::memset(m_block, 0, sizeof(m_block)); m_164 = 0; m_174 = 0; } };
int main()
{
    CChannelState s; std::memset(&s, 0xee, sizeof(s));
    s.Clear();
    bool ok = sizeof(s) == 0x178 && s.m_0 == (long)0xeeeeeeee && s.m_block[0] == 0 && s.m_block[0x15f] == 0 && s.m_164 == 0 && s.m_168 == (long)0xeeeeeeee && s.m_170 == (long)0xeeeeeeee && s.m_174 == 0;
    if (!ok) { std::printf("00c592d0_TEST FAIL\n"); return 1; }
    std::printf("00c592d0_TEST PASS\n"); return 0;
}