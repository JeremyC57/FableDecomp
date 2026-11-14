// CTCInventoryAbilities::ConstructPCExpressionsList(NUISystem::CComponent* parent) -- the PC skills screen's expressions page: sort the hero's inventory into the four expression categories (social, renown, alignment, stealth) by each item's expression def, build the "PC_SUB_LIST" with its items container and bottom-left position dummy, wire one containment slot per quick-access item (observe-event prompts, the assign action, an empty inside-container or the assigned icon with its graphic, drag-off unassign, observe and drop-to-swap actions), add the "EXPRESSIONS TITLE" header (registered with the PC frame), initialise the list offsets and append the four category lists with their text-bank titles; returns the sub-list (null when the hero has no CTCInventory)
// Grade: DIFFER(2649v2652) -- code identical modulo colouring; what-if (agentC10, re-confirmed agentC11 on this text) gives retail with textBank prio 35 and every const -1 piece <= 40: B + const -1=40,_textBank=35 = D 13 (masked relocations only)
// Residue detail: ours textBank (prio 56 = tail-block K 8 x cost 7) takes edi before const 0 (48); retail const 0 edi, inventory piece ebx (0x005F5A3E..0x005F5C45), -1 tail piece ebx (0x005F60F8 / 0x005F6122), textBank memory [ebp+5Ch]. agentC11: same-TU helpers now byte-match retail (GetMiscThingsDef 0x005EE3B3, GetDef<CExpressionDef> 0x005F81BE, GetDef<CThingBaseDef> 0x0042AF3C); row unchanged
// Next: K of the tail block. Measured agentC11: K drops 8->7 only when the PeekComponent result is used with no inline layer (diagnostic raw cast); the vector-free "local" is the stealth vector's _M_start field itself (stays with every ~_Vector_base text). See NOTES.md row 1
// Leaf: ?ConstructPCExpressionsList@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005F59C7, 2652 bytes, `ret 4`. Biased frame (lea ebp,[esp-74h]): optimize("s",on) alone.
// Callees: CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D, CCharString 0x0099EBF0 / copy 0x0099EC30 / ~ 0x0099EAE0,
//   CTCInventoryAbilities::GetMiscThingsDef 0x005EE3B3 (hidden CDefPointer result), std::bitset<274>::test 0x00410DE0 +
//   CVectorMap::LowerBound 0x0040F020 (the inline GetTC on the parent thing), _Rb_tree::clear 0x00593235, CTCInventoryBase::GetAllItems
//   0x005BF482, CTCBase::GetDefinitionManager 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C / GetDef(long,
//   CDefPointer<CExpressionDef>*) 0x005F81BE, CDefClassBase::GetSubDef<CExpressionSubDef> 0x005DA240, vector<pair<long,long> >::
//   push_back 0x005947B6, CCountedPointer<CComponent>(T*) 0x00429C15 / ~ 0x004291DE, CCountedPointer<CAction>(T*) 0x0042AA29 / ~
//   0x00429160, operator new 0x00BFEA1A, CAction::CAction 0x0042BE50 / AddParam 0x0053FDE0 / AddParam(param) 0x0053FDB0,
//   list<CActionParam*>::push_back 0x0042B646, GFIntToCharString 0x0099F830, operator+(const char*, const CCharString&) 0x0099F690,
//   CTCInventory::GetQuickAccessItemGraphicIndex 0x00581FA4, CTCBase::PeekComponent 0x00686D60, CWideString 0x0099AED0 / = 0x0099B7D0
//   / ~ 0x0099B510, NGameText::CDataBank::GetTextBySymbol 0x009C95E0, CTCInventoryAbilities::ConstructPCExpressionCategoryList
//   0x005F45B5, free 0x00BFEA14 (the five vectors). Virtuals: CComponent FindChild 3, AddChild 59, AddAction 66, AddActionOnSelected
//   67, AddActionOnDropped 74, AddActionOnDroppedNowhere 75, SetName 83, AddPositionChild 124; CList AddNonScrollingChild 145,
//   InitialiseOffsets 156; CSprite SetGraphicIndex 145. The global 0x013B8C60 is the file's anonymous-namespace CCharString
//   TITLE_NAME ("EXPRESSIONS TITLE", its initializer landed as 0x0121AE9C).
// Shape: the expression sub-def and expression-def handles are declared before the sub-def lookup (both zeroed ahead of the
//   GetSubDef call; the def handle's release is laid out inside the found branch only); the category switch's default falls into
//   the social list; the slot loop's index is unsigned (`jb`) and names the slot from its 1-based number; the icon sprite's
//   graphic is set only when the slot is still assigned (the author re-tests the slot after the sprite lookup); the drop-to-swap
//   action takes its two SLONG parameters before the container parameter (single-argument AddParam).
// Layout: engine/CTCInventoryAbilities.h (retail branch) is `this`; CTCInventory through engine/CTCInventory.h's retail branch
//   (QuickAccessItems +0x148, ItemsMenuEntries +0x1fc, AssignedIcons +0x208); CExpressionSubDef::ExpressionDef +0x28,
//   CExpressionDef::ExpressionType +0x8c (engine/CExpressionDef.h).
#pragma optimize("s", on)
#define FABLE_RETAIL_CEXPRESSIONSUBDEF_STATGETTYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ITEMLISTS
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCINVENTORYABILITIES_EXPRESSIONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC
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

using NUISystem::CComponent;
using NUISystem::CList;
using NUISystem::CSprite;
using NUISystem::CAction;
using NUISystem::CActionParamSLONG;
using NUISystem::CActionParamULONG;

namespace { extern CCharString TITLE_NAME; }                  // 0x013B8C60 ("EXPRESSIONS TITLE")

// Same-TU bodies of the def lookups the row calls (retail instantiates / emits them in this TU and calls them out of line:
// CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C, GetDef(long, CDefPointer<CExpressionDef>*) 0x005F81BE,
// CDefClassBase::GetSubDef<CExpressionSubDef> 0x005DA240, CTCInventoryAbilities::GetMiscThingsDef 0x005EE3B3). With the
// bodies visible VC7.1 knows the handles' addresses do not escape, so their scope-end releases are the fused `dec [ecx+4] ;
// jne` with the null store dropped, as retail's are (see hdr/README.md, CDefPointer release shapes). Only the row is graded.
#pragma auto_inline(off)
template <class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index <= 0)
        return false;
    CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
    if (!def.IsNull()) {
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
// the UI misc-things def ("UI_MISC_THINGS_DEF", class index 1) the abilities screen reads its layout values from
const CDefPointer<const NUISystem::CUIMiscThingsDef> CTCInventoryAbilities::GetMiscThingsDef() const
{
    long index;
    {
        CCharString name("UI_MISC_THINGS_DEF");
        index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex(name, 1);
    }
    {
        CDefPointer<const NUISystem::CUIMiscThingsDef> def;
        CGameDefinitionManager::Get().GetDef(index, &def);
        return def;
    }
}
// The PC category builder 0x005F45B5 (landed row CTCInventoryAbilities_ConstructPCExpressionCategoryList_005f45b5.cpp), defined
// above this function in retail's TU: VC7.1 reads its body and sees the references it gets are never stored.
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
#pragma auto_inline(on)

CComponent* CTCInventoryAbilities::ConstructPCExpressionsList(CComponent* parent)
{
    CTCInventory* inventory;
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CComponent* frame = parent->FindChild("PC_UI_FRAME");
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscThingsDef = GetMiscThingsDef();

    // the expressions live in the hero's item inventory component
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
            }
        }
    }

    // the page: a sub-list holding the quick-access items container and the bottom-left position dummy
    CList* subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
    CCountedPointer<CComponent> itemsContainer(manager->CreateComponent("PC_EXPRESSIONS_ITEMS_CONTAINMENT_CONTAINER", false));
    subList->AddNonScrollingChild(itemsContainer);
    CCountedPointer<CComponent> positionDummy(manager->CreateComponent("PC_POSITION_DUMMY_BOTTOM_LEFT", false));
    subList->AddNonScrollingChild(positionDummy);
    positionDummy->AddPositionChild(itemsContainer.Data);

    // one containment slot per quick-access item, named from its 1-based number
    for(unsigned long n=0;n<inventory->QuickAccessItems.size();++n) {
        CCharString name="PC_EXPRESSION_ITEM_CONTAINMENT_"+GFIntToCharString(n+1);
        CComponent* slot=subList->FindChild(name);
        if(slot) {
            CCountedPointer<CAction> observe(new CAction);
            observe->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observe->AddParam(slot,observe);
            observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
            subList->AddActionOnSelected(observe);
            CCountedPointer<CAction> observeDrop(new CAction);
            observeDrop->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observeDrop->AddParam(slot,observeDrop);
            observeDrop->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_COMPONENT_DROPPED));
            subList->AddActionOnSelected(observeDrop);
            CCountedPointer<CAction> assign(new CAction);
            assign->ActionType=NUISystem::UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC;
            assign->AddParam(slot,assign);
            assign->UIActionParams.push_back(new CActionParamULONG(n));
            slot->AddAction(assign);
            if(inventory->QuickAccessItems[n]==0) {
                CCountedPointer<CComponent> inside(manager->CreateComponent("PC_EXPRESSION_ITEM_INSIDE_CONTAINER",false));
                slot->AddChild(inside);
            } else {
                CCountedPointer<CComponent> icon(manager->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER",false));
                CComponent* sprite=icon->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
                if(sprite && inventory->QuickAccessItems[n]!=0)
                    static_cast<NUISystem::CSprite*>(sprite)->SetGraphicIndex(inventory->GetQuickAccessItemGraphicIndex(n));
                CCountedPointer<CAction> action(new CAction);
                action->ActionType=NUISystem::UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC;
                action->AddParam(slot,action);
                action->UIActionParams.push_back(new CActionParamULONG(n));
                icon->AddActionOnDroppedNowhere(action);
                CCountedPointer<CAction> follow(new CAction);
                follow->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                follow->AddParam(icon.Get(),follow);
                follow->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                subList->AddActionOnSelected(follow);
                CCountedPointer<CAction> swap(new CAction);
                swap->ActionType=NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC;
                swap->UIActionParams.push_back(new CActionParamSLONG(inventory->QuickAccessItems[n]));
                swap->UIActionParams.push_back(new CActionParamSLONG(n));
                swap->AddParam(slot);
                icon->AddActionOnDropped(swap);
                slot->AddChild(icon);
            }
        }
    }

    // the page title, registered with the PC frame so it is positioned with it
    CCountedPointer<CComponent> title(manager->CreateComponent("PC_TITLE_EXPRESSIONS", false));
    title->SetName(TITLE_NAME);
    subList->AddNonScrollingChild(title);
    if (frame)
        frame->AddPositionChild(title.Data);
    subList->InitialiseOffsets(4);

    // the four category lists, titled from the text bank
    const NGameText::CDataBank& textBank = PeekComponent().GetTextBank();
    CWideString categoryTitle;
    categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_SOCIAL");
    ConstructPCExpressionCategoryList(categoryTitle, subList, socialItems, parent, "PC_RING_PIC_EXPR_SOCIAL", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_SOCIAL");
    categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN");
    ConstructPCExpressionCategoryList(categoryTitle, subList, renownItems, parent, "PC_RING_PIC_EXPR_RENOWN", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_RENOWN");
    categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_ALIGNMENT");
    ConstructPCExpressionCategoryList(categoryTitle, subList, alignmentItems, parent, "PC_RING_PIC_EXPR_ALIGNMENT", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_ALIGNMENT");
    categoryTitle = textBank.GetTextBySymbol("TEXT_GUI_MENU_STEALTH");
    ConstructPCExpressionCategoryList(categoryTitle, subList, stealthItems, parent, "PC_RING_PIC_EXPR_STEALTH", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_STEALTH");
    return subList;
}
