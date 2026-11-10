// Behaviour test for CGameScriptInterface::StopSound 0x0088F660: host model -- forwards the sample id to the sound manager's slot 14.
#include <cstdio>
class CASoundManager {
public:
    unsigned long Calls; unsigned long Last;
    CASoundManager() : Calls(0), Last(0) {}
    virtual ~CASoundManager() {}
    virtual void StopSound(unsigned long id) { ++Calls; Last = id; }
};
static CASoundManager* PSoundManager = 0;
class CGameScriptInterface {
public:
    void StopSound(unsigned long soundId) const { PSoundManager->StopSound(soundId); }
};
int main()
{
    CASoundManager m; PSoundManager = &m; CGameScriptInterface gsi;
    gsi.StopSound(7);
    bool ok = m.Calls == 1 && m.Last == 7;
    gsi.StopSound(0);
    ok = ok && m.Calls == 2 && m.Last == 0;
    if (!ok) { std::printf("0088f660_TEST FAIL\n"); return 1; }
    std::printf("0088f660_TEST PASS\n");
    return 0;
}
