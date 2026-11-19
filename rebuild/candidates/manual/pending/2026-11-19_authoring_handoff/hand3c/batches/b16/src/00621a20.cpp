// CTCInventoryQuests::InitialiseQuestCardList() -- rebuild the quest screen's categories: clear the category list, collect the quest cards (the quest manager's active cards on the inventory screen, else the building's hero-centre cards when the screen is enabled), count them, add one category per card that has a quest-card component (its def's inventory category, associated with the card thing) and select the first category
// Leaf: ?InitialiseQuestCardList@CTCInventoryQuests@@QAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00621A20, length 600 bytes, plain `ret`; no EH frame (the CTCInventoryQuests TU, as
//   OpenInventory 0x00622A00).
// Shape: FableWin.pdb ?InitialiseQuestCardList@CTCInventoryQuests@@QAEXXZ (FableWin 0x01F7D6C0), statement for statement, with the
//   Ego_d.pdb local names (cardList, quest_cards, it, pbuilding_thing, pTCHeroCentre, idx, pQuestCard, ptc_quest_card, category_def_index,
//   pCat). The debug build's GetTC ASSERT is not in retail.
// Callees: ResetCategoryList (inline: Categories.clear(), __copy 0x00625820 + _Destroy 0x005C2A8E); NHeroInformationScreens::CBase
//   IsInventoryScreen (slot 3, through the +0x130 base); CQuestManager::Get (PClass 0x013B89FC) / GetActiveQuestCards (inline, +0x5C);
//   CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10; vector<CIntelligentPointer<CThing> >
//   _M_insert_overflow 0x004AC940; CTCInventoryBase::GetIsEnabled (slot 75) / GetBuildingThing 0x005BC4C4 / AddCategory 0x005C024A /
//   GetCategoryWithIndex 0x005BCE3C; CThing::GetTC<CTCHeroCentre> (expanded, LowerBound 0x0040F020); CTCHeroCentre::GetQuestCards 0x0077DBC0;
//   CThing::PeekTC<const CTCQuestCard> (expanded, the const LowerBound inline); NInventory::CCategory::SetAsSelected 0x005BC934; free 0x00BFEA14.
// TU opt-ins: FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL (the /Gr __destroy_aux convention, as the same-TU DrawActiveTestQuests 0x0061AD60:
//   ecx = first, edx = last, tag pushed, 0x00621A4E..0x00621A51) and FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND (PeekTC's const
//   LowerBound expanded in place, 0x00621BA4..0x00621BD3, while GetTC's non-const LowerBound stays the call 0x0040F020).
// Same-TU helper: __copy<NInventory::CCategory*> grades RELOCATION_MATCH at its own address 0x00625820. The range destroy the row
//   calls, 0x005C2A8E, is the copy the linker kept from the NInventory base TU (27 bytes, ~CCategory called out of line); this TU's
//   COMDAT copy (~CCategory expanded) is discarded, as it is in retail.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CBASEOBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_CONST_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_DESTROY_AUX_FASTCALL
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_ACCESSORS
#define FABLE_RETAIL_CTCQUESTCARD_STATGETINTERFACETYPE
#define FABLE_RETAIL_CTCHEROCENTRE_GETQUESTCARDS
#define FABLE_RETAIL_INVENTORY_ORIGINAL_ALL_ITEMS_CHAIN_PASS22
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_CATEGORYWITHINDEX
#define FABLE_RETAIL_CTCINVENTORYBASE_ADDCATEGORY
#define FABLE_RETAIL_CCATEGORY_SETASSELECTED
#define FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
#define FABLE_RETAIL_CTCINVENTORYQUESTS_OPENINVENTORY
#define FABLE_RETAIL_CTCINVENTORYQUESTS_RESETCATEGORYLIST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CQuestCardDef.h"
#include "engine/CTCQuestCard.h"
#include "engine/CTCHeroCentre.h"
#include "engine/CQuestManager.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CTCInventoryQuests.h"

void CTCInventoryQuests::InitialiseQuestCardList()
{
    ResetCategoryList();
    std::vector<CIntelligentPointer<CThing> > cardList;
    if (IsInventoryScreen()) {
        const std::list<CIntelligentPointer<CThing> >& quest_cards = CQuestManager::Get().GetActiveQuestCards();
        for (std::list<CIntelligentPointer<CThing> >::const_iterator it = quest_cards.begin(); it != quest_cards.end(); ++it)
            cardList.push_back(CIntelligentPointer<CThing>(it->Get()));
    } else if (GetIsEnabled()) {
        CThing* pbuilding_thing = GetBuildingThing();
        if (pbuilding_thing != NULL) {
            CTCHeroCentre* pTCHeroCentre = NULL;
            pbuilding_thing->GetTC(TCI_HERO_CENTRE, &pTCHeroCentre);   // FableWin GetTC<CTCHeroCentre>; the generated CTCHeroCentre.h has no StatGetInterfaceType (OpenInventory 0x00622A00 spells it the same way)
            pTCHeroCentre->GetQuestCards(cardList);
        }
    }
    QuestCardCount = cardList.size();
    for (long idx = 0; idx < QuestCardCount; ++idx) {
        CThing* pQuestCard = cardList[idx].Get();
        const CTCQuestCard* ptc_quest_card;
        if (pQuestCard->PeekTC(&ptc_quest_card)) {
            long category_def_index = ptc_quest_card->GetPDef()->InventoryCategory;
            AddCategory(this, category_def_index, pQuestCard);
        }
    }
    if (QuestCardCount > 0) {
        NInventory::CCategory* pCat = GetCategoryWithIndex(0);
        pCat->SetAsSelected(true);
    }
}
