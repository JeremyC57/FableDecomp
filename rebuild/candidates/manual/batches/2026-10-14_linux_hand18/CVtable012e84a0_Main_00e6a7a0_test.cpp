#include <cstdio>
static int g_pumps = 0; static int g_polls = 0; static int g_limit = 3;
struct CGameScriptInterface_Calls { virtual void v000() { } virtual void v001() { } virtual void v002() { } virtual void v003() { } virtual void v004() { } virtual void v005() { } virtual void v006() { } virtual void Pump() { ++g_pumps; } };
struct CVtable_012e84a0 {
    virtual void v00() { } CGameScriptInterface_Calls* m_pInterface;
    unsigned char IsFinished() { ++g_polls; return g_pumps >= g_limit ? 1 : 0; }
    void Main() { while (!IsFinished()) m_pInterface->Pump(); }
};
int main()
{
    CGameScriptInterface_Calls gi; CVtable_012e84a0 a; a.m_pInterface = &gi;
    a.Main(); if (g_pumps != 3 || g_polls != 4) { std::printf("00e6a7a0_TEST FAIL\n"); return 1; }
    g_pumps = 0; g_polls = 0; g_limit = 0; a.Main(); if (g_pumps != 0 || g_polls != 1) { std::printf("00e6a7a0_TEST FAIL\n"); return 1; }
    std::printf("00e6a7a0_TEST PASS\n"); return 0;
}
