#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
static int g_termCalls = 0, g_termAt = 0, g_yields = 0; static unsigned char g_condition = 0;
class CGameScriptInterface { public: virtual void v000() { } virtual void YieldFrame() { ++g_yields; } virtual unsigned char IsDonationConditionMet() { return g_condition; } };
struct CV_TempleOfLightScript {
    virtual void v00() { }
    unsigned char _pad04[0x3c]; CGameScriptInterface* m_pInterface; unsigned char _pad44[0x11]; unsigned char m_donationNoted;
    unsigned char IsActiveThreadTerminating() { ++g_termCalls; return g_termCalls >= g_termAt; }
    void WatchForDonationConditions()
    {
        while (!IsActiveThreadTerminating()) {
            if (m_pInterface->IsDonationConditionMet()) { if (!m_donationNoted) { if (IsActiveThreadTerminating()) return; m_donationNoted = 1; } }
            m_pInterface->YieldFrame();
        }
    }
};
int main()
{
    CGameScriptInterface iface; CV_TempleOfLightScript s; s.m_pInterface = &iface; s.m_donationNoted = 0;
    g_condition = 0; g_termAt = 3; s.WatchForDonationConditions();               // two frames without the condition
    if (g_yields != 2 || s.m_donationNoted != 0 || g_termCalls != 3) { std::printf("00edbc30_TEST FAIL\n"); return 1; }
    g_termCalls = 0; g_yields = 0; g_condition = 1; g_termAt = 4; s.WatchForDonationConditions();   // noted on the first frame, then one more frame
    if (s.m_donationNoted != 1 || g_yields != 2 || g_termCalls != 4) { std::printf("00edbc30_TEST FAIL\n"); return 1; }
    g_termCalls = 0; g_yields = 0; s.m_donationNoted = 0; g_termAt = 2; s.WatchForDonationConditions();   // the inner test terminates before noting
    if (s.m_donationNoted != 0 || g_yields != 0 || g_termCalls != 2) { std::printf("00edbc30_TEST FAIL\n"); return 1; }
    std::printf("00edbc30_TEST PASS\n"); return 0;
}