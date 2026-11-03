// NScript::CGameflowScript::Init() -- reset gameflow and declare gossip categories.
// Leaf: ?Init@CGameflowScript@NScript@@UAEXXZ
// Extent: 16 bytes through 00ce6d00; supplied 2237-byte span includes the next function.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CENTITYSCRIPTBASE
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CGameflowScript.h"
#include "engine/CQ_SunnyvaleMasterData.h"
void NScript::CGameflowScript::Init()
{
    MasterScriptData.PostSavePosition = EGP_INTRO;
    CoreQuestWaiting = 0;
    DeclareGossipCategories();
}
