// CTCInventoryQuests::ProcessQuestActivateAction() -- take the selected quest card: when the screen is enabled and has cards, activate the selected category's card with the quest manager (as a feat card when it has a feat index, else as a quest card), drop it from the building's hero centre and rebuild the card list
// Leaf: ?ProcessQuestActivateAction@CTCInventoryQuests@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006223C0, length 242 bytes (+ int3 padding), bool in al.
// Identity: Ego_r.pdb + FableWin.pdb ?ProcessQuestActivateAction@CTCInventoryQuests@@UAE_NXZ, the override of NInventory::CTCInventoryBase
//   slot 70 (engine/NInventory_CTCInventoryBase.h VTABLE: 70 ProcessQuestActivateAction, base 0x004E7D45 `xor al,al`); vtable 0x012585B4
//   slot 70 = 0x006223C0. Ego_d.pdb locals: pCardThing, ptc_quest_card, pbuilding, pTCHeroCentre in nested blocks. The catalog's
//   "vfunc_70" was a placeholder.
// Callees: slot 75 GetIsEnabled() const (+0x12C), CTCInventoryBase::GetSelectedCategoryIndex @ 0x005BC4B2 / GetCategoryWithIndex @
//   0x005BCE3C / GetBuildingThing @ 0x005BC4C4, NInventory::CCategory::GetCategoryThingAssociation @ 0x005BD1AE, the component-map
//   LowerBound const @ 0x004365B0 / non-const @ 0x0040F020, CQuestManager::ActivateFeatCard @ 0x004B1590 / ActivateQuestCard @ 0x004B4AA0
//   on CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass (0x013B89FC), CTCHeroCentre::RemoveQuestCard @ 0x0077D400,
//   CTCInventoryQuests::InitialiseQuestCardList @ 0x00621A20.
// Shape: the component pointers are left unset when the thing lacks the component (retail reads the lookup-key word): PeekTC for the
//   quest card (TCI_QUEST_CARD 0x6C, const lookup), GetTC for the hero centre (TCI_HERO_CENTRE 0x6A, non-const lookup).
// NF87: moved off the row-local CThing / CVectorMap / CQuestManager / CTBaseSingletonNoDefaultAlloc / CTCQuestCard / CTCHeroCentre /
//   CCategory / CTCInventoryQuests (placeholder vtable) models onto the shared headers (overlay opt-ins CTCINVENTORYQUESTS_PROCESSQUESTACTIVATEACTION,
//   CTCINVENTORYBASE_CATEGORYWITHINDEX, NINVENTORY_CCATEGORY_ACCESSORS, CQUESTMANAGER_ACTIVATEQUESTCARDS, CTCHEROCENTRE_REMOVEQUESTCARD).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDCATEGORYINDEX
#define FABLE_RETAIL_CTCINVENTORYBASE_CATEGORYWITHINDEX
#define FABLE_RETAIL_NINVENTORY_CCATEGORY_ACCESSORS
#define FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
#define FABLE_RETAIL_CTCINVENTORYQUESTS_OPENINVENTORY
#define FABLE_RETAIL_CTCINVENTORYQUESTS_PROCESSQUESTACTIVATEACTION
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCHEROCENTRE_REMOVEQUESTCARD
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_ACTIVATEQUESTCARDS
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CQuestCardDef.h"
#include "engine/CTCQuestCard.h"
#include "engine/CTCHeroCentre.h"
#include "engine/CQuestManager.h"
#include "engine/CTCInventoryQuests.h"

bool CTCInventoryQuests::ProcessQuestActivateAction()
{
    if (GetIsEnabled() && QuestCardCount > 0) {
        CThing* pCardThing = GetCategoryWithIndex(GetSelectedCategoryIndex())->GetCategoryThingAssociation();
        if (pCardThing) {
            const CTCQuestCard* ptc_quest_card;
            pCardThing->PeekTC(TCI_QUEST_CARD, &ptc_quest_card);
            if (ptc_quest_card) {
                if (ptc_quest_card->FeatIndex > -1) {
                    if (!CQuestManager::Get().ActivateFeatCard(pCardThing))
                        return false;
                } else if (!CQuestManager::Get().ActivateQuestCard(pCardThing, false)) {
                    return false;
                }
                CThing* pbuilding = GetBuildingThing();
                if (pbuilding) {
                    CTCHeroCentre* pTCHeroCentre;
                    pbuilding->GetTC(TCI_HERO_CENTRE, &pTCHeroCentre);
                    pTCHeroCentre->RemoveQuestCard(pCardThing);
                }
                InitialiseQuestCardList();
                return true;
            }
        }
    }
    return false;
}
