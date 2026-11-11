// CTCInventoryAbilities::GetExpressionsListUIComponent(long& count, NUISystem::CComponent* owner) -- console expressions screen: (null without the hero inventory) reset the inventory's menu-entry maps, sort every owned expression item into the social / renown / alignment / stealth lists by its expression def's type (counting them), then build the expressions sub list (back / select / title headers, four entries) with one category branch per list
// Leaf: ?GetExpressionsListUIComponent@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@AAJPAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F4C8F, 1296 bytes, thiscall `ret 8`; frame-pointer size TU (optimize "s").
//   FableWin.pdb ?GetExpressionsListUIComponent@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@AAJPAV23@@Z.
// Callees: as the PC twin ConstructPCExpressionsList 0x005F59C7 (GetTC through bitset test 0x00410DE0 + LowerBound 0x0040F020,
//   map clear 0x00593235, GetAllItems 0x005BF482, GetDef<CThingBaseDef> 0x0042AF3C, GetSubDef<CExpressionSubDef> 0x005DA240,
//   GetDef(long, CDefPointer<CExpressionDef>*) 0x005F81BE, vector<pair<long,long> >::push_back 0x005947B6), plus the console
//   category builder ConstructExpressionCategoryList 0x005F3F7B (x4).
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


namespace { extern CCharString TITLE_NAME; }                  // 0x013B8C60 ("EXPRESSIONS TITLE")

// Same-TU bodies of the def lookups the row calls (retail instantiates / emits them in this TU and calls them out of line:
// CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C, GetDef<CExpressionDef> 0x005F81BE (FableWin.pdb template),
// CDefClassBase::GetSubDef<CExpressionSubDef> 0x005DA240, CTCInventoryAbilities::GetMiscThingsDef 0x005EE3B3). With the
// bodies visible VC7.1 knows the handles' addresses do not escape, so their scope-end releases are the fused `dec [ecx+4] ;
// jne` with the null store dropped, as retail's are (see hdr/README.md, CDefPointer release shapes). Only the row is graded.
#pragma auto_inline(off)
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (def.IsNull())
        return false;
    out->SetDynamicCast(def);
    return true;
}
template <> bool CDefClassBase::GetSubDef<CExpressionSubDef>(CDefPointer<const CExpressionSubDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CExpressionSubDef"));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
// The console category builder 0x005F3F7B, defined just above this function in retail's TU (its landed row is
// CTCInventoryAbilities_ConstructExpressionCategoryList_005f3f7b.cpp); VC7.1 reads its body and sees that the references it gets
// are never stored. Only GetExpressionsListUIComponent is graded here.
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
#pragma auto_inline(on)

NUISystem::CComponent* CTCInventoryAbilities::GetExpressionsListUIComponent(long& count, NUISystem::CComponent* owner)
{
    NUISystem::CManager* ui = NUISystem::CManager::GetInstance();
    CTCInventory* inventory;
    if (!ParentThing->GetTC(&inventory))
        return 0;
    inventory->ItemsMenuEntries.clear();
    inventory->AssignedIcons.clear();

    // sort the carried items into the four expression categories by their expression def's type
    std::vector<std::pair<long, long> > allItems;
    std::vector<std::pair<long, long> > socialItems;
    std::vector<std::pair<long, long> > renownItems;
    std::vector<std::pair<long, long> > alignmentItems;
    std::vector<std::pair<long, long> > stealthItems;
    inventory->GetAllItems(allItems);
    count = 0;
    for (unsigned long i = 0; i < allItems.size(); ++i) {
        CDefPointer<const CThingBaseDef> itemDef;
        GetDefinitionManager().GetDef(allItems[i].first, &itemDef);
        CDefPointer<const CExpressionSubDef> expressionSubDef;
        CDefPointer<const CExpressionDef> expressionDef;
        if (itemDef->GetSubDef(&expressionSubDef)) {
            if (GetDefinitionManager().GetDef(expressionSubDef->ExpressionDef, &expressionDef)) {
                switch (expressionDef->ExpressionType) {
                case EXPRESSION_INVENTORY_TYPE_SOCIAL:    socialItems.push_back(allItems[i]);    break;
                case EXPRESSION_INVENTORY_TYPE_RENOWN:    renownItems.push_back(allItems[i]);    break;
                case EXPRESSION_INVENTORY_TYPE_ALIGNMENT: alignmentItems.push_back(allItems[i]); break;
                case EXPRESSION_INVENTORY_TYPE_STEALTH:   stealthItems.push_back(allItems[i]);   break;
                default:                                  socialItems.push_back(allItems[i]);    break;
                }
                ++count;
            }
        }
    }

    // the sub list: back / select / title headers and the four category entries
    NUISystem::CList* subList = static_cast<NUISystem::CList*>(ui->CreateComponent("UI_SUB_LIST", false));
    {   // the headers and the category title live in this block only: their frame slots reuse the sorting loop's
        // (retail select [ebp-0Ch] over the item def, title [ebp-14h] over the index, the title text in count's slot [ebp+8])
        CCountedPointer<NUISystem::CComponent> back(ui->CreateComponent("UI_BACK_FOR_LIST", false));
        CCountedPointer<NUISystem::CComponent> select(ui->CreateComponent("UI_SELECT_FOR_LIST", false));
        subList->AddNonScrollingChild(back);
        subList->AddNonScrollingChild(select);
        CCountedPointer<NUISystem::CComponent> title(ui->CreateComponent("UI_EXPRESSIONS_TITLE", false));
        title->SetName(TITLE_NAME);
        subList->AddNonScrollingChild(title);
        subList->InitialiseOffsets(4);

        const NGameText::CDataBank& textBank = PeekComponent().GetTextBank();
        CWideString categoryTitle;
        categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_SOCIAL");
        ConstructExpressionCategoryList(categoryTitle, subList, socialItems, owner, "UI_RING_PIC_EXPR_SOCIAL", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_SOCIAL");
        categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN");
        ConstructExpressionCategoryList(categoryTitle, subList, renownItems, owner, "UI_RING_PIC_EXPR_RENOWN", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_RENOWN");
        categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_ALIGNMENT");
        ConstructExpressionCategoryList(categoryTitle, subList, alignmentItems, owner, "UI_RING_PIC_EXPR_ALIGNMENT", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_ALIGNMENT");
        categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_STEALTH");
        ConstructExpressionCategoryList(categoryTitle, subList, stealthItems, owner, "UI_RING_PIC_EXPR_STEALTH", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_STEALTH");
    }
    return subList;
}
