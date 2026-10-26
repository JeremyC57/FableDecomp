// Behaviour test for 0x00e9fec0: NScript::CV_KnotholeGladeGatesScript::InitialiseVariables clears the thirteen flags
// +0x48..+0x54, BalvKillCount +0x58 and FirstBalvDead +0x68, and nothing else (TheGate +0x5c, the base, the padding).
#include <cstdio>
#include <cstring>
struct Gates { unsigned char _base[0x48]; bool f[13]; int BalvKillCount; unsigned char TheGate[0xc]; bool FirstBalvDead;
    void InitialiseVariables() { for (int i = 0; i < 8; ++i) f[i] = false; f[9] = false; f[11] = false; f[12] = false; f[8] = false; f[10] = false; BalvKillCount = 0; FirstBalvDead = false; } };
int main() {
    Gates g; std::memset(&g, 0xff, sizeof g); g.InitialiseVariables(); unsigned char* p = (unsigned char*)&g;
    bool ok = sizeof(Gates) == 0x6c && (unsigned char*)&g.BalvKillCount - p == 0x58 && (unsigned char*)&g.FirstBalvDead - p == 0x68;
    for (int i = 0x48; i <= 0x54; ++i) if (p[i]) ok = false;
    for (int i = 0x5c; i < 0x68; ++i) if (p[i] != 0xff) ok = false;
    if (g.BalvKillCount || g.FirstBalvDead || p[0x47] != 0xff || p[0x55] != 0xff) ok = false;
    if (!ok) { std::printf("00e9fec0_TEST FAIL\n"); return 1; }
    std::printf("00e9fec0_TEST PASS\n"); return 0;
}
