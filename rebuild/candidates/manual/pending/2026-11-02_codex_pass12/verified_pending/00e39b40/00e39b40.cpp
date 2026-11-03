// NScript::CV_BordelloScript::Main() -- register the seven entities and four original background threads.
// Leaf: ?Main@CV_BordelloScript@NScript@@UAEXXZ
#define FABLE_RETAIL_CSCRIPTBASE_PRIVATE
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_PERSONAL_SCRIPT
#define FABLE_RETAIL_BANDIT_CUTSCENE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BORDELLO
#include "engine/CCharString.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CRGBColour.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CV_BordelloScript.h"
#include "engine/CEntityScriptBinding.h"
#include "engine/CSpawnedFunc.h"
#include "engine/TRetVoid.h"
void NScript::CV_BordelloScript::Main()
{
    TRetVoid _Fail_Val;
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BordelloScript>("Madame",this,&CMadame::Alloc,1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BordelloScript>("BordelloLady",this,&CBordelloLady::Alloc,1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BordelloScript>("BordelloGuard",this,&CBordelloGuard::Alloc,1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BordelloScript>("Magicman",this,&CMagicman::Alloc,1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BordelloScript>("BordelloClient",this,&CBordelloClient::Alloc,1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BordelloScript>("M_HeroDownstairs",this,&CM_HeroDownstairs::Alloc,1));
    AddEntityScriptBinding(new CEntityScriptBinding<CV_BordelloScript>("BordelloEntrance",this,&CBordelloEntrance::Alloc,0));
    PostAddScriptedEntities();Game.PostAddScriptedEntities();
    AddSpawnedFunction(new CSpawnedFunc<CV_BordelloScript>("WatchForBordelloStatus",0,this,&CV_BordelloScript::WatchForBordelloStatus),CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BordelloScript>("WatchForDeedStatus",0,this,&CV_BordelloScript::WatchForDeedStatus),CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BordelloScript>("CreateCustomers",0,this,&CV_BordelloScript::CreateCustomers),CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CV_BordelloScript>("AdjustTavernPrices",0,this,&CV_BordelloScript::AdjustTavernPrices),CCharString(""));
}
