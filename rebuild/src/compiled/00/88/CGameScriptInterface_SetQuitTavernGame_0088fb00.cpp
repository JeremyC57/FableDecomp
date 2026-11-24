// Leaf: ?SetQuitTavernGame@CGameScriptInterface@@UAEX_N@Z
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern unsigned char g_quitTavernGame;

void CGameScriptInterface::SetQuitTavernGame(bool quit)
{
    g_quitTavernGame = (unsigned char)quit;
}
// Flatten route (flatten_pilot v4, NF324): CGameScriptInterface_SetQuitTavernGame -> CGameScriptInterface::SetQuitTavernGame, the RTTI-placed PDB member; class model: engine/CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT.
