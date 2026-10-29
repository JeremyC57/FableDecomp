#include <cstdio>
struct TutMgr { char pad0[0xc]; char flagC; char pad1[0x17-0xd]; char flag17; char pad2[0xd7-0x18]; char flagD7; };
class CUserProfileManager : public TutMgr { public: static CUserProfileManager& __fastcall Get(); };   // 0x0040D2A0 ?Get@CUserProfileManager@@SIAAV1@XZ
static TutMgr g;
CUserProfileManager& __fastcall CUserProfileManager::Get() { return *(CUserProfileManager*)(&g); }

char __fastcall CGameScriptInterface_IsTutorialSystemEnabled(void* self)
{
    TutMgr* p = &CUserProfileManager::Get();
    if (p->flagC)
        return p->flagD7;
    return p->flag17;
}

int main()
{
    g.flagC = 1; g.flagD7 = 7; g.flag17 = 0;
    if (CGameScriptInterface_IsTutorialSystemEnabled(0) != 7) { std::printf("FAIL a\n"); return 1; }
    g.flagC = 0; g.flag17 = 0;
    if (CGameScriptInterface_IsTutorialSystemEnabled(0) != 0) { std::printf("FAIL b\n"); return 1; }
    g.flagC = 0; g.flag17 = 5;
    if (CGameScriptInterface_IsTutorialSystemEnabled(0) != 5) { std::printf("FAIL c\n"); return 1; }
    std::printf("CGameScriptInterface_0088f3e0_TEST PASS\n");
    return 0;
}