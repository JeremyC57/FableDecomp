// Behaviour test for CGameScriptInterface::IsSoundPlaying 0x0088F610: host model -- forwards the id to the sound manager's slot 15.
#include <cstdio>
class CASoundManager {
public:
    unsigned long Playing; unsigned long Last;
    CASoundManager() : Playing(5), Last(0) {}
    virtual ~CASoundManager() {}
    virtual bool IsSoundPlaying(unsigned long id) const { const_cast<CASoundManager*>(this)->Last = id; return id == Playing; }
};
static CASoundManager* PSoundManager = 0;
class CGameScriptInterface {
public:
    bool IsSoundPlaying(unsigned long soundId) const { return PSoundManager->IsSoundPlaying(soundId); }
};
int main()
{
    CASoundManager m; PSoundManager = &m; CGameScriptInterface gsi;
    bool ok = gsi.IsSoundPlaying(5) && m.Last == 5 && !gsi.IsSoundPlaying(6) && m.Last == 6;
    if (!ok) { std::printf("0088f610_TEST FAIL\n"); return 1; }
    std::printf("0088f610_TEST PASS\n");
    return 0;
}
