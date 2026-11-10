// Behaviour test for CGameScriptInterface::EnableSounds 0x0088F620: host model of the row's body over a recording sound manager.
#include <cstdio>
class CASoundManager {
public:
    int Stops, AtmosOn, AtmosOff;
    CASoundManager() : Stops(0), AtmosOn(0), AtmosOff(0) {}
    virtual ~CASoundManager() {}
    virtual void StopAllSounds() { ++Stops; }
    void EnableAtmos(bool enable) { if (enable) ++AtmosOn; else ++AtmosOff; }
};
static CASoundManager* PSoundManager = 0;
static unsigned char g_soundsEnabledFlag = 7;
class CGameScriptInterface {
public:
    void EnableSounds(bool enable) const
    {
        CASoundManager* mgr = PSoundManager;
        if (mgr == 0)
            return;
        if (enable) { mgr->EnableAtmos(true); g_soundsEnabledFlag = 1; return; }
        mgr->StopAllSounds();
        PSoundManager->EnableAtmos(false);
        g_soundsEnabledFlag = 0;
    }
};
int main()
{
    CGameScriptInterface gsi; CASoundManager m;
    gsi.EnableSounds(true);
    bool ok = g_soundsEnabledFlag == 7;
    PSoundManager = &m;
    gsi.EnableSounds(true);
    ok = ok && m.AtmosOn == 1 && m.Stops == 0 && g_soundsEnabledFlag == 1;
    gsi.EnableSounds(false);
    ok = ok && m.AtmosOff == 1 && m.Stops == 1 && g_soundsEnabledFlag == 0;
    if (!ok) { std::printf("0088f620_TEST FAIL\n"); return 1; }
    std::printf("0088f620_TEST PASS\n");
    return 0;
}
