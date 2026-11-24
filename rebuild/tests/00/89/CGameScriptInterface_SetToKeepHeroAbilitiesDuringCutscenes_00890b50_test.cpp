// NF335 (NF324 Q2): the row now defines the PDB member CGameScriptInterface::SetToKeepHeroAbilitiesDuringCutscenes
// (?SetToKeepHeroAbilitiesDuringCutscenes@CGameScriptInterface@@UBEX_N@Z), declared by the shared header's retail model
// (FABLE_RETAIL_NSCRIPT, the row's own switch). The test calls it qualified through a reference to that model bound to
// zero-initialised static storage: the model is never constructed, so the test emits no vftable and no copy of the row's symbol.
#define FABLE_RETAIL_NSCRIPT
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

static unsigned long g_gsiStorage[(sizeof(CGameScriptInterface) + sizeof(unsigned long) - 1) / sizeof(unsigned long)];

int main()
{
    CMainGameComponent inner;
    CGameScriptInterface& gsi = *(CGameScriptInterface*)g_gsiStorage;
    gsi.Component = &inner;
    inner.PPlayerManager = (void*)0x1234;

    g_obj[0x224] = 0;
    gsi.CGameScriptInterface::SetToKeepHeroAbilitiesDuringCutscenes(true);
    if (g_obj[0x224] != 1) { printf("FAIL true\n"); return 1; }

    gsi.CGameScriptInterface::SetToKeepHeroAbilitiesDuringCutscenes(false);
    if (g_obj[0x224] != 0) { printf("FAIL false\n"); return 1; }

    printf("00890b50_TEST PASS\n");
    return 0;
}
