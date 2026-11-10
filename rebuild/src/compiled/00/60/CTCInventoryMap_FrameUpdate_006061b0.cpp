// CTCInventoryMap::FrameUpdate() -- once the region event life has passed since the last event frame, refresh the region correspondences and the visited regions and stamp the frame; then, when a quest is waiting to be dropped, find its live quest card among the quest manager's active cards, fail the quest and deactivate it; the drop request is cleared either way
// Leaf: ?FrameUpdate@CTCInventoryMap@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006061B0, 322 bytes (+ int3 padding), plain `ret`. Vtable 0x01257A9C slot 9 (the CTCBase
//   FrameUpdate slot; the catalog's "vfunc_9"); Ego_r.pdb ?FrameUpdate@CTCInventoryMap@@UAEXXZ (0x00658200, right after
//   UpdateVisitedRegions 0x00657D20 as retail 0x006061B0 follows 0x00605CE0).
// Callees: CWorld::GetFrame 0x0049D870 (static, four reads), CTCInventoryMap::UpdateRegionsCorrespondances 0x005FC0C0 /
//   UpdateVisitedRegions 0x00605CE0, CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//   0x0040F020 (the inline GetTC<CTCQuestCard>, TCI 0x6C), CWideString(const wchar_t*) 0x0099B6B0 / ~CWideString 0x0099B510,
//   CQuestManager::SetQuestAsFailed 0x004B2280 / DeactivateQuest(const CCharString&, unsigned long) 0x004B43D0 on the singleton
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass 0x013B89FC.
// Layout (shared headers): CTCInventoryMap QuestToBeDropped +0x1A4, LastEventFrame +0x250, RegionEventLife +0x254; CQuestManager
//   ActiveQuestCards +0x5C; CThing DefGlobalIndex +0x8C, dead bit +0x91; CTCQuestCard ScriptQuestName +0x28.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_FAILQUEST
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_QUESTNAME
#define FABLE_RETAIL_CTCQUESTCARD_SCRIPTQUESTNAME
#define FABLE_RETAIL_CTCINVENTORYMAP_FRAMEUPDATE
#include "engine/CTCInventoryMap.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CQuestManager.h"
#include "engine/CTCQuestCard.h"

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)
{
    return thing.Get() && !thing->IsDead();
}

void CTCInventoryMap::FrameUpdate()
{
    if ((unsigned long)CWorld::GetFrame() >= LastEventFrame + RegionEventLife) {
        UpdateRegionsCorrespondances(LastEventFrame, CWorld::GetFrame());
        UpdateVisitedRegions(LastEventFrame, CWorld::GetFrame());
        LastEventFrame = CWorld::GetFrame();
    }
    if (QuestToBeDropped > 0) {
        const std::list<CIntelligentPointer<CThing> >& cards = CQuestManager::Get().GetActiveQuestCards();
        for (std::list<CIntelligentPointer<CThing> >::const_iterator it = cards.begin(); it != cards.end(); ++it) {
            const CIntelligentPointer<CThing>& quest_card = *it;
            if (IS_THING_ALIVE(quest_card) && (long)quest_card->GetDefGlobalIndex() == QuestToBeDropped) {
                CTCQuestCard* card;
                if (quest_card->GetTC(&card)) {
                    CQuestManager::Get().SetQuestAsFailed(card->GetScriptQuestName(), true, L"", true);
                    CQuestManager::Get().DeactivateQuest(card->GetScriptQuestName(), 0);
                }
                break;
            }
        }
        QuestToBeDropped = 0;
    }
}