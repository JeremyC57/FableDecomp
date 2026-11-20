// CQuestManager::PrepareQuestsWhenFinalQuestIsCompleted() -- make the world's map table show the quest cards when it is used
// Leaf: ?PrepareQuestsWhenFinalQuestIsCompleted@CQuestManager@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004AF270, 11 bytes: `mov eax,[ecx+7Ch] ; mov byte ptr [eax+0DAh],1 ; ret`.
// Naming (NF108, 2026-11-19, q4406): was the __fastcall stand-in DontPopulateNextLoadedRegion(CGameScriptInterface*) over local
//   models. Ego_r.pdb 0x004B45F0 + FableWin.pdb 0x01C535D0 ?PrepareQuestsWhenFinalQuestIsCompleted@CQuestManager@@QAEXXZ;
//   CGameScriptInterface::PrepareQuestsWhenFinalQuestIsCompleted 0x00892F30 loads the CQuestManager singleton (0x013B89FC) into ecx and
//   tail-jumps here. The body is FableWin's inline CWorld::SetMapTableToShowQuestCardsWhenUsed(bool) (0x01C535B0, just before the
//   caller) on CQuestManager::World (+0x7C) storing CWorld::MapTableShowQuestCardsOnUsed (+0xDA).
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_LOADGAMESTATE                        // the layout branch that names World (+0x7C)
#define FABLE_RETAIL_CQUESTMANAGER_PREPAREQUESTSWHENFINALQUESTISCOMPLETED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_SETMAPTABLETOSHOWQUESTCARDSWHENUSED
#include "engine/CWorld.h"
#include "engine/CQuestManager.h"

void CQuestManager::PrepareQuestsWhenFinalQuestIsCompleted()
{
    World->SetMapTableToShowQuestCardsWhenUsed(true);
}
