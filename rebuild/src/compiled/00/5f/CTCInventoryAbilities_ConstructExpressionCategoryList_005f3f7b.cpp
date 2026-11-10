// CTCInventoryAbilities::ConstructExpressionCategoryList(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long,long> >& items, NUISystem::CComponent* owner, const CCharString& ringPicture, const CCharString& guideSymbol) -- console expressions category: add a branch entry captioned "title x n" to the list with its menu-guide description (also shown in the owner's scrolling viewport), subtitle and ring picture; an empty category is made unavailable (its arrow deleted), otherwise it opens a sub list titled "<list title>: title" whose item rows ContructItemLists builds with a shared actions list
// Leaf: ?ConstructExpressionCategoryList@CTCInventoryAbilities@@AAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCComponent@4@ABVCCharString@@4@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F3F7B, 1438 bytes, thiscall `ret 18h`; frame-pointer size TU (optimize "s").
//   FableWin.pdb ?ConstructExpressionCategoryList@CTCInventoryAbilities@@AAEX.. (private); the catalog label
//   "ConstructPCExpressionCategoryList" is propagated from the PC twin 0x005F45B5. Identity: the console caller
//   GetExpressionsListUIComponent 0x005F4C8F calls this address four times (UI_* names); the PC ConstructPCExpressionsList
//   0x005F59C7 calls 0x005F45B5 (PC_* names); this body calls the console ContructItemLists 0x005F077F.
// Callees: CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494, CCountedPointer
//   <CComponent>(CComponent*) 0x00429C15 / ReleaseData 0x004291DE, CTCBase::PeekComponent 0x00686D60, CDataBank::GetTextBySymbol
//   0x009C95E0, CTCInventoryStats::GetXString 0x005CC772, GFIntToWideString 0x00405F00, CWideString copy 0x0099B720 / += 0x0099B8D0
//   / += (const wchar_t*) 0x0099B940 / ~ 0x0099B510, CCharString 0x0099EBF0 / ~ 0x0099EAE0 / != (const char*) 0x0099E960 /
//   operator+(const CCharString&, const char*) 0x0099F600, CDeletion 0x0042BED4 / 0x0042CD84 / 0x0042ABCA,
//   ContructItemLists 0x005F077F. The global 0x013B8C60 is the TU's anonymous-namespace CCharString TITLE_NAME.
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ITEMLISTS
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCINVENTORYABILITIES_SCREEN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CEXPRESSIONSUBDEF
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_EXPRESSIONDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_EXPRESSIONS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMULONG
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCInventory.h"
#include "engine/CThingBaseDef.h"
#include "engine/CExpressionSubDef.h"
#include "engine/CExpressionDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/fable_script_text.h"
#include "engine/CCountedPointer.h"


#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CTCInventoryStats.h"
CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00

namespace { extern CCharString TITLE_NAME; }                  // 0x013B8C60 ("EXPRESSIONS TITLE")

void CTCInventoryAbilities::ConstructExpressionCategoryList(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long, long> >& items,
                                                            NUISystem::CComponent* owner, const CCharString& ringPicture, const CCharString& guideSymbol)
{
    NUISystem::CManager* ui = NUISystem::CManager::GetInstance();
    NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(owner->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    CWideString caption(title);
    if (!items.empty()) {
        caption += CTCInventoryStats::GetXString();
        caption += GFIntToWideString(items.size());
    }
    CCountedPointer<NUISystem::CComponent> entry(ui->CreateMenuEntryComponent(caption, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f,
                                                                              CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
    list->AddScrollingChildAndSetOffsets(entry);
    CCountedPointer<NUISystem::CComponent> description(ui->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
    NUISystem::CText* descriptionText = static_cast<NUISystem::CText*>(description.Get());
    descriptionText->SetValue(PeekComponent().GetTextBank().GetTextBySymbol(guideSymbol), true, true);
    entry->AddChild(description);
    viewport->AddFakeChild(description.Get());
    CCountedPointer<NUISystem::CComponent> subtitle(ui->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    static_cast<NUISystem::CText*>(subtitle.Get())->SetValue(caption, true, true);
    entry->AddChild(subtitle);
    if (ringPicture != "") {
        CCountedPointer<NUISystem::CComponent> ring(ui->CreateComponent(ringPicture, false));
        entry->AddChild(ring);
    }
    {   // the category's sub list and its headers: their frame slots reuse the optional ring picture's (retail [ebp-3Ch])
        CCountedPointer<NUISystem::CComponent> subList(ui->CreateComponent("UI_SUB_LIST", false));
        NUISystem::CList* sub = static_cast<NUISystem::CList*>(subList.Get());
        CCountedPointer<NUISystem::CComponent> back(ui->CreateComponent("UI_BACK_FOR_LIST", false));
        CCountedPointer<NUISystem::CComponent> select(ui->CreateComponent("UI_SELECT_FOR_LIST", false));
        sub->AddNonScrollingChild(back);
        sub->AddNonScrollingChild(select);
        if (ringPicture != "") {
            CCountedPointer<NUISystem::CComponent> ringHeader(ui->CreateComponent(ringPicture + "_FOR_LIST", false));
            sub->AddNonScrollingChild(ringHeader);
        }
        if (items.empty()) {
            entry->MakeUnavailable();
            NUISystem::CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        } else {
            entry->AddChild(subList);
            sub->InitialiseOffsets(items.size());
            CCountedPointer<NUISystem::CComponent> listTitle(ui->CreateComponent("UI_EXPRESSIONS_TITLE", false));
            NUISystem::CText* listTitleText = static_cast<NUISystem::CText*>(listTitle.Get());
            CWideString titleText(listTitleText->GetValue());
            titleText += L": ";
            titleText += title;
            listTitleText->SetValue(titleText, true, true);
            listTitle->SetName(TITLE_NAME);
            sub->AddNonScrollingChild(listTitle);
            CCountedPointer<NUISystem::CComponent> actions(NUISystem::CManager::GetInstance()->CreateComponent("UI_LIST_ITEMS_ACTIONS", false));
            NUISystem::CList* actionsList = static_cast<NUISystem::CList*>(actions.Get());
            actionsList->SetDefaultChild(0L);
            CCountedPointer<NUISystem::CComponent> actionsBack(ui->CreateComponent("UI_BACK_FOR_LIST", false));
            CCountedPointer<NUISystem::CComponent> actionsSelect(ui->CreateComponent("UI_SELECT_FOR_LIST", false));
            actionsList->AddNonScrollingChild(actionsBack);
            actionsList->AddNonScrollingChild(actionsSelect);
            CCountedPointer<NUISystem::CComponent> actionsTitle(ui->CreateComponent("UI_EXPRESSIONS_TITLE", false));
            static_cast<NUISystem::CText*>(actionsTitle.Get())->SetValue(titleText, true, true);
            actionsTitle->SetName(TITLE_NAME);
            actionsList->AddNonScrollingChild(actionsTitle);
            ContructItemLists(items, sub, owner, actions);
        }
    }
}