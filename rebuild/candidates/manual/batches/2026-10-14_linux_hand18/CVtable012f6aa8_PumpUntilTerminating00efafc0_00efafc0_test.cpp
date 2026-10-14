#include <cstdio>
static int g_pumps = 0; static int g_polls = 0; static int g_limit = 3;
struct CGameScriptInterface_Calls { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { } virtual void Pump() { ++g_pumps; } };
struct CScriptBase_Retail { virtual void v00() { } unsigned char _pad[0x3c]; unsigned char IsActiveThreadTerminating() { ++g_polls; return g_pumps >= g_limit ? 1 : 0; } };
struct CVtable_012f6aa8 : CScriptBase_Retail {
    CGameScriptInterface_Calls* m_pInterface;
    void PumpUntilTerminating_00efafc0() { while (!IsActiveThreadTerminating()) m_pInterface->Pump(); }
};
int main()
{
    CGameScriptInterface_Calls gi; CVtable_012f6aa8 s; s.m_pInterface = &gi;
    if ((char*)&s.m_pInterface - (char*)&s != 0x40) { std::printf("00efafc0_TEST FAIL\n"); return 1; }
    s.PumpUntilTerminating_00efafc0(); if (g_pumps != 3 || g_polls != 4) { std::printf("00efafc0_TEST FAIL\n"); return 1; }
    g_pumps = 0; g_polls = 0; g_limit = 0; s.PumpUntilTerminating_00efafc0(); if (g_pumps != 0 || g_polls != 1) { std::printf("00efafc0_TEST FAIL\n"); return 1; }
    std::printf("00efafc0_TEST PASS\n"); return 0;
}
