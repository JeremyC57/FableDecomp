// CTCInventoryAbilities::ConstructPCExpressionCategoryList(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long,long> >& items, NUISystem::CComponent* parent, const CCharString& ringPicture, const CCharString& guideSymbol) -- PC expressions category: add a branch entry captioned "title x n" to the list with its menu-guide description (shown in the description viewport), subtitle and ring picture, each registered with the PC frame; an empty category is made unavailable, otherwise it opens a sub list titled "<list title>: title" whose item rows ConstructPCItemLists builds with a shared actions list
// Leaf: ?ConstructPCExpressionCategoryList@CTCInventoryAbilities@@QAEXAAVCWideString@@PAVCList@NUISystem@@AAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCComponent@4@ABVCCharString@@4@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F45B5, 1233 bytes, thiscall `ret 18h`; frame-pointer size TU (optimize "s").
//   FableWin.pdb ?ConstructPCExpressionCategoryList@CTCInventoryAbilities@@QAEX..; its caller ConstructPCExpressionsList
//   0x005F59C7 calls it four times (PC_* names) and it calls ConstructPCItemLists 0x005F1526 (the console twin is
//   ConstructExpressionCategoryList 0x005F3F7B).
// Callees: as the console twin 0x005F3F7B, plus CComponent slot 124 AddPositionChild (the PC frame registration).
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ITEMLISTS
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCINVENTORYABILITIES_EXPRESSIONS
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

void CTCInventoryAbilities::ConstructPCExpressionCategoryList(CWideString& title, NUISystem::CList* list, std::vector<std::pair<long, long> >& items,
                                                              NUISystem::CComponent* parent, const CCharString& ringPicture, const CCharString& guideSymbol)
{
    NUISystem::CManager* ui = NUISystem::CManager::GetInstance();
    NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(parent->FindChild("PC_DESCRIPTION_VIEWPORT"));
    NUISystem::CComponent* frame = parent->FindChild("PC_UI_FRAME");
    if (frame && viewport)
        frame->AddPositionChild(viewport);
    CWideString caption(title);
    if (!items.empty()) {
        caption += CTCInventoryStats::GetXString();
        caption += GFIntToWideString(items.size());
    }
    CCountedPointer<NUISystem::CComponent> entry(ui->CreateMenuEntryComponent(caption, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f,
                                                                              CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
    list->AddScrollingChildAndSetOffsets(entry);
    CCountedPointer<NUISystem::CComponent> description(ui->CreateComponent("PC_DESCRIPTION", false));
    NUISystem::CText* descriptionText = static_cast<NUISystem::CText*>(description.Get());
    descriptionText->SetValue(PeekComponent().GetTextBank().GetTextBySymbol(guideSymbol), true, true);
    entry->AddChild(description);
    viewport->AddFakeChild(description.Get());
    if (frame)
        frame->AddPositionChild(description.Get());
    CCountedPointer<NUISystem::CComponent> subtitle(ui->CreateComponent("PC_SUBTITLE", false));
    static_cast<NUISystem::CText*>(subtitle.Get())->SetValue(caption, true, true);
    entry->AddChild(subtitle);
    if (frame)
        frame->AddPositionChild(subtitle.Get());
    if (ringPicture != "") {
        CCountedPointer<NUISystem::CComponent> ring(ui->CreateComponent(ringPicture, false));
        entry->AddChild(ring);
        if (frame)
            frame->AddPositionChild(ring.Get());
    }
    {   // the category's sub list and its headers
        CCountedPointer<NUISystem::CComponent> subList(ui->CreateComponent("PC_SUB_LIST", false));
        NUISystem::CList* sub = static_cast<NUISystem::CList*>(subList.Get());
        if (ringPicture != "") {
            CCountedPointer<NUISystem::CComponent> ringHeader(ui->CreateComponent(ringPicture + "_FOR_LIST", false));
            sub->AddNonScrollingChild(ringHeader);
            if (frame)
                frame->AddPositionChild(ringHeader.Get());
        }
        if (items.empty()) {
            entry->MakeUnavailable();
        } else {
            entry->AddChild(subList);
            sub->InitialiseOffsets(items.size());
            CCountedPointer<NUISystem::CComponent> listTitle(ui->CreateComponent("PC_TITLE_EXPRESSIONS", false));
            NUISystem::CText* listTitleText = static_cast<NUISystem::CText*>(listTitle.Get());
            CWideString titleText(listTitleText->GetValue());
            titleText += L": ";
            titleText += title;
            listTitleText->SetValue(titleText, true, true);
            listTitle->SetName(TITLE_NAME);
            sub->AddNonScrollingChild(listTitle);
            if (frame)
                frame->AddPositionChild(listTitle.Get());
            CCountedPointer<NUISystem::CComponent> actions(NUISystem::CManager::GetInstance()->CreateComponent("PC_LIST_ITEMS_ACTIONS", false));
            NUISystem::CList* actionsList = static_cast<NUISystem::CList*>(actions.Get());
            CCountedPointer<NUISystem::CComponent> actionsTitle(ui->CreateComponent("PC_TITLE_EXPRESSIONS", false));
            static_cast<NUISystem::CText*>(actionsTitle.Get())->SetValue(titleText, true, true);
            actionsTitle->SetName(TITLE_NAME);
            actionsList->AddNonScrollingChild(actionsTitle);
            if (frame)
                frame->AddPositionChild(actionsTitle.Get());
            ConstructPCItemLists(items, sub, parent, actions);
        }
    }
}