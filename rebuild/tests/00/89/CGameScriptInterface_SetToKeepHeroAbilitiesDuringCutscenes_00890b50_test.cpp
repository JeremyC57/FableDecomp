#include "engine/CGameScriptInterface.h"
#include <cstdio>

class CMainGameComponent { public:
    char pad[0x1c];
    void* PPlayerManager;
};

static unsigned char g_obj[0x300];

class CPlayer;
class CPlayerManager { public: CPlayer& GetMainPlayer(); };   // 0x00449970, called by the row
CPlayer& CPlayerManager::GetMainPlayer()
{
    // return our fake object
    return *(CPlayer*)g_obj;
}

void __fastcall SetToKeepHeroAbilitiesDuringCutscenes(CGameScriptInterface* self, int edx, bool keep);

int main()
{
    CMainGameComponent inner;
    CGameScriptInterface gsi;
    gsi.Component = &inner;
    inner.PPlayerManager = (void*)0x1234;

    g_obj[0x224] = 0;
    SetToKeepHeroAbilitiesDuringCutscenes(&gsi, 0, true);
    if (g_obj[0x224] != 1) { printf("FAIL true\n"); return 1; }

    SetToKeepHeroAbilitiesDuringCutscenes(&gsi, 0, false);
    if (g_obj[0x224] != 0) { printf("FAIL false\n"); return 1; }

    printf("00890b50_TEST PASS\n");
    return 0;
}