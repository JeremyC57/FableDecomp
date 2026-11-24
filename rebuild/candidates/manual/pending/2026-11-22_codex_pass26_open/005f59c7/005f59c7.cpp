// 005f59c7 CTCInventoryAbilities::ConstructPCExpressionsList
// Leaf: ?ConstructPCExpressionsList@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// Flags: none
#define FABLE_RETAIL_CDEFPOINTER_ORIGINAL_DIRECT_DEREF_PASS26
#define FABLE_RETAIL_INVENTORY_EXPRESSIONS_PASS26
#include "engine/GFDebugPDynamicCast.h"
#define FABLE_RETAIL_CEXPRESSIONSUBDEF_STATGETTYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFCLASSBASE_EXPRESSION_SPEED_INSTANCE_PASS26
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CVECTORMAP_COMPONENT_LOWERBOUND_OWNER_PASS26
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ITEMLISTS
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_ORIGINAL_SCOPES_PASS22
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
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_STLP_THING_BITSET_SPEED_OWNER_PASS26
// Inline-budget: the size-built expressions leaf expands HasTC but calls the checked bitset test.
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMULONG
#include "stlport/fable_thing_bitset.h"
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
#include "engine/CDefClassBase_GetSubDef.h"
#pragma optimize("s", on)

using NUISystem::CComponent;
using NUISystem::CList;
using NUISystem::CSprite;
using NUISystem::CAction;
using NUISystem::CActionParamSLONG;
using NUISystem::CActionParamULONG;

namespace { extern CCharString TITLE_NAME; }                  // 0x013B8C60 ("EXPRESSIONS TITLE")

// Same-TU bodies of the def lookups the row calls (retail instantiates / emits them in this TU and calls them out of line:
// CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C, GetDef<CExpressionDef>(long, CDefPointer<const CExpressionDef>*) 0x005F81BE,
// CDefClassBase::GetSubDef<CExpressionSubDef> 0x005DA240, CTCInventoryAbilities::GetMiscThingsDef 0x005EE3B3). With the
// bodies visible VC7.1 knows the handles' addresses do not escape, so their scope-end releases are the fused `dec [ecx+4] ;
// jne` with the null store dropped, as retail's are (see hdr/README.md, CDefPointer release shapes). Only the row is graded.
#pragma auto_inline(off)
template <class T> bool CDefinitionManager::GetDef(long global_index, CDefPointer<const T>* def) const
{
    if (global_index > 0) {
        CDefPointer<const CDefClassBase> pbase_def = GetPDefFromGlobalIndex(global_index);
        if (pbase_def.Get()) {
            def->SetDynamicCast(pbase_def);
            return true;
        }
    }
    return false;
}
// the UI misc-things def ("UI_MISC_THINGS_DEF", class index 1) the abilities screen reads its layout values from
const CDefPointer<const NUISystem::CUIMiscThingsDef> CTCInventoryAbilities::GetMiscThingsDef() const
{
    long misc_thing_def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CDefPointer<const NUISystem::CUIMiscThingsDef> pmiscthingsdef;
    CGameDefinitionManager::Get().GetDef(misc_thing_def_index, &pmiscthingsdef);
    return pmiscthingsdef;
}
#pragma auto_inline(on)

CComponent* CTCInventoryAbilities::ConstructPCExpressionsList(CComponent* ptop_list)
{
    CTCInventory* ptc_inventory;
    NUISystem::CManager* pmanager = NUISystem::CManager::GetInstance();
    CComponent* pframe = ptop_list->FindChild("PC_UI_FRAME");
    CDefPointer<const NUISystem::CUIMiscThingsDef> pmiscthingsdef = GetMiscThingsDef();

    if (!GetParentThing().GetTC(&ptc_inventory))
        return 0;
    ptc_inventory->GetItemsMenuEntries().clear();
    ptc_inventory->GetAssignedIcons().clear();

    std::vector<std::pair<long, long> > items, expressions_catA, expressions_catB, expressions_catC, expressions_catD;
    ptc_inventory->GetAllItems(items);
    long tot_num_of_expressions = 0;
    for (long count = 0; count < items.size(); ++count) {
        CDefPointer<const CThingBaseDef> pthing_def;
        GetDefinitionManager().GetDef(items[count].first, &pthing_def);
        CDefPointer<const CExpressionSubDef> pexpression_sub_def;
        CDefPointer<const CExpressionDef> pexpression_def;
        if (pthing_def->GetSubDef(&pexpression_sub_def) && GetDefinitionManager().GetDef(pexpression_sub_def->ExpressionDef, &pexpression_def)) {
            EExpressionInventoryType exp_type = pexpression_def->ExpressionType;
            switch (exp_type) {
            case EXPRESSION_INVENTORY_SOCIAL:    expressions_catA.push_back(items[count]);    break;
            case EXPRESSION_INVENTORY_RENOWN:    expressions_catB.push_back(items[count]);    break;
            case EXPRESSION_INVENTORY_ALIGNMENT: expressions_catC.push_back(items[count]); break;
            case EXPRESSION_INVENTORY_STEALTH:   expressions_catD.push_back(items[count]);   break;
            default:                                  expressions_catA.push_back(items[count]);    break;
            }
            ++tot_num_of_expressions;
        }
    }

    CComponent* pexpressions_list_component = 0;
    CList* pexpressions_list = 0;
    pexpressions_list_component = pmanager->CreateComponent("PC_SUB_LIST", false);
    pexpressions_list = GFDebugPDynamicCast(pexpressions_list_component,(CList*)0);
    CCountedPointer<CComponent> pexpressions_container(pmanager->CreateComponent("PC_EXPRESSIONS_ITEMS_CONTAINMENT_CONTAINER", false));
    pexpressions_list->AddNonScrollingChild(pexpressions_container);
    CCountedPointer<CComponent> pdummy_bottom_left(pmanager->CreateComponent("PC_POSITION_DUMMY_BOTTOM_LEFT", false));
    pexpressions_list->AddNonScrollingChild(pdummy_bottom_left);
    pdummy_bottom_left->AddPositionChild(pexpressions_container.Get());

    std::vector<long>& assigned_things = ptc_inventory->GetQuickAccessItems();
    for(unsigned long count=0;count<assigned_things.size();++count) {
        CCharString name="PC_EXPRESSION_ITEM_CONTAINMENT_"+GFIntToCharString(count+1);
        CComponent* pexpression_containment=pexpressions_list->FindChild(name);
        if(pexpression_containment) {
            CCountedPointer<CAction> paction_observe(new CAction);
            paction_observe->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            paction_observe->AddParam(pexpression_containment,paction_observe);
            paction_observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
            pexpressions_list->AddActionOnSelected(paction_observe);
            CCountedPointer<CAction> paction_observe_dropped_event(new CAction);
            paction_observe_dropped_event->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            paction_observe_dropped_event->AddParam(pexpression_containment,paction_observe_dropped_event);
            paction_observe_dropped_event->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_COMPONENT_DROPPED));
            pexpressions_list->AddActionOnSelected(paction_observe_dropped_event);
            CCountedPointer<CAction> paction_assign_expression(new CAction);
            paction_assign_expression->ActionType=NUISystem::UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC;
            paction_assign_expression->AddParam(pexpression_containment,paction_assign_expression);
            paction_assign_expression->UIActionParams.push_back(new CActionParamULONG(count));
            pexpression_containment->AddAction(paction_assign_expression);
            if(assigned_things[count]==0) {
                CCountedPointer<CComponent> pchild(pmanager->CreateComponent("PC_EXPRESSION_ITEM_INSIDE_CONTAINER",false));
                pexpression_containment->AddChild(pchild);
            } else {
                CCountedPointer<CComponent> pchild(pmanager->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER",false));
                CComponent* psprite_in_child=pchild->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
                if(psprite_in_child) {
                    NUISystem::CSprite* psprite_in_child_real = GFDebugPDynamicCast(psprite_in_child,(NUISystem::CSprite*)0);
                    if (assigned_things[count]!=0) psprite_in_child_real->SetGraphicIndex(ptc_inventory->GetQuickAccessItemGraphicIndex(count));
                }
                CCountedPointer<CAction> paction_on_nowhere(new CAction);
                paction_on_nowhere->ActionType=NUISystem::UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC;
                paction_on_nowhere->AddParam(pexpression_containment,paction_on_nowhere);
                paction_on_nowhere->UIActionParams.push_back(new CActionParamULONG(count));
                pchild->AddActionOnDroppedNowhere(paction_on_nowhere);
                CCountedPointer<CAction> paction_draggable_observe(new CAction);
                paction_draggable_observe->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                paction_draggable_observe->AddParam(pchild.Get(),paction_draggable_observe);
                paction_draggable_observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                pexpressions_list->AddActionOnSelected(paction_draggable_observe);
                CCountedPointer<CAction> paction_set_expression_to_assign(new CAction);
                paction_set_expression_to_assign->ActionType=NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC;
                paction_set_expression_to_assign->UIActionParams.push_back(new CActionParamSLONG(assigned_things[count]));
                paction_set_expression_to_assign->UIActionParams.push_back(new CActionParamSLONG(count));
                paction_set_expression_to_assign->AddParam(pexpression_containment);
                pchild->AddActionOnDropped(paction_set_expression_to_assign);
                pexpression_containment->AddChild(pchild);
            }
        }
    }

    CCountedPointer<CComponent> ptitle(pmanager->CreateComponent("PC_TITLE_EXPRESSIONS", false));
    ptitle->SetName(TITLE_NAME);
    pexpressions_list->AddNonScrollingChild(ptitle);
    if (pframe) {
        pframe->AddPositionChild(ptitle.Get());
    }
    pexpressions_list->InitialiseOffsets(4);

    const NGameText::CDataBank& rTextBank = PeekComponent().GetTextBank();
    CWideString menuparam;
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_SOCIAL");
    ConstructPCExpressionCategoryList(menuparam, pexpressions_list, expressions_catA, ptop_list, "PC_RING_PIC_EXPR_SOCIAL", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_SOCIAL");
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN");
    ConstructPCExpressionCategoryList(menuparam, pexpressions_list, expressions_catB, ptop_list, "PC_RING_PIC_EXPR_RENOWN", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_RENOWN");
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_ALIGNMENT");
    ConstructPCExpressionCategoryList(menuparam, pexpressions_list, expressions_catC, ptop_list, "PC_RING_PIC_EXPR_ALIGNMENT", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_ALIGNMENT");
    menuparam = rTextBank.GetTextBySymbol("TEXT_GUI_MENU_STEALTH");
    ConstructPCExpressionCategoryList(menuparam, pexpressions_list, expressions_catD, ptop_list, "PC_RING_PIC_EXPR_STEALTH", "TEXT_GUI_MENUGUIDE_SKILLS_EXPRESSIONS_STEALTH");
    return pexpressions_list;
}

// Both donors place the original category builder after the caller. The visible body retains
// the native out-of-line call and preserves the scoped string and vector references.
#pragma auto_inline(off)
CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00

void CTCInventoryAbilities::ConstructPCExpressionCategoryList(CWideString& menuparam, NUISystem::CList* pexpressions_list, std::vector<std::pair<long, long> >& expressions,
                                                              NUISystem::CComponent* ptop_list, const CCharString& ring_pic_name, const CCharString& text_tag)
{
    NUISystem::CManager* pmanager = NUISystem::CManager::GetInstance();
    NUISystem::CViewport* pviewport_real = GFDebugPDynamicCast(ptop_list->FindChild("PC_DESCRIPTION_VIEWPORT"),(NUISystem::CViewport*)0);
    NUISystem::CComponent* pframe = ptop_list->FindChild("PC_UI_FRAME");
    if (pframe && pviewport_real)
        pframe->AddPositionChild(pviewport_real);
    CWideString menu_title(menuparam);
    if (!expressions.empty()) {
        menu_title += CTCInventoryStats::GetXString();
        menu_title += GFIntToWideString(expressions.size());
    }
    CCountedPointer<NUISystem::CComponent> pmenu_entry_cat(pmanager->CreateMenuEntryComponent(menu_title, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f,
                                                                              CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
    pexpressions_list->AddScrollingChildAndSetOffsets(pmenu_entry_cat);
    CCountedPointer<NUISystem::CComponent> pmenu_entry_description(pmanager->CreateComponent("PC_DESCRIPTION", false));
    NUISystem::CText* pmenu_entry_description_real = GFDebugPDynamicCast(pmenu_entry_description.Get(),(NUISystem::CText*)0);
    pmenu_entry_description_real->SetValue(PeekComponent().GetTextBank().GetTextBySymbol(text_tag), true, true);
    pmenu_entry_cat->AddChild(pmenu_entry_description);
    pviewport_real->AddFakeChild(pmenu_entry_description.Get());
    if (pframe)
        pframe->AddPositionChild(pmenu_entry_description.Get());
    CCountedPointer<NUISystem::CComponent> psubtitle(pmanager->CreateComponent("PC_SUBTITLE", false));
    NUISystem::CText* psubtitle_real = static_cast<NUISystem::CText*>(psubtitle.Get());
    psubtitle_real->SetValue(menu_title, true, true);
    pmenu_entry_cat->AddChild(psubtitle);
    if (pframe)
        pframe->AddPositionChild(psubtitle.Get());
    if (ring_pic_name != "") {
        CCountedPointer<NUISystem::CComponent> plist_pic_component(pmanager->CreateComponent(ring_pic_name, false));
        pmenu_entry_cat->AddChild(plist_pic_component);
        if (pframe)
            pframe->AddPositionChild(plist_pic_component.Get());
    }
    CCountedPointer<NUISystem::CComponent> plist_cat_component(pmanager->CreateComponent("PC_SUB_LIST", false));
    NUISystem::CList* plist_cat = static_cast<NUISystem::CList*>(plist_cat_component.Get());
    if (ring_pic_name != "") {
        CCountedPointer<NUISystem::CComponent> plist_pic_component(pmanager->CreateComponent(ring_pic_name + "_FOR_LIST", false));
        plist_cat->AddNonScrollingChild(plist_pic_component);
        if (pframe)
            pframe->AddPositionChild(plist_pic_component.Get());
    }
    if (expressions.empty()) {
        pmenu_entry_cat->MakeUnavailable();
    } else {
        pmenu_entry_cat->AddChild(plist_cat_component);
        plist_cat->InitialiseOffsets(expressions.size());
        CCountedPointer<NUISystem::CComponent> ptitle_for_cat(pmanager->CreateComponent("PC_TITLE_EXPRESSIONS", false));
        NUISystem::CText* ptext = static_cast<NUISystem::CText*>(ptitle_for_cat.Get());
        CWideString tit(ptext->GetValue());
        tit += L": ";
        tit += menuparam;
        ptext->SetValue(tit, true, true);
        ptitle_for_cat->SetName(TITLE_NAME);
        plist_cat->AddNonScrollingChild(ptitle_for_cat);
        if (pframe)
            pframe->AddPositionChild(ptitle_for_cat.Get());
        CCountedPointer<NUISystem::CComponent> pactions_list(NUISystem::CManager::GetInstance()->CreateComponent("PC_LIST_ITEMS_ACTIONS", false));
        NUISystem::CList* pactions_list_real = static_cast<NUISystem::CList*>(pactions_list.Get());
        CCountedPointer<NUISystem::CComponent> pactiontitle(pmanager->CreateComponent("PC_TITLE_EXPRESSIONS", false));
        ptext = static_cast<NUISystem::CText*>(pactiontitle.Get());
        ptext->SetValue(tit, true, true);
        pactiontitle->SetName(TITLE_NAME);
        pactions_list_real->AddNonScrollingChild(pactiontitle);
        if (pframe)
            pframe->AddPositionChild(pactiontitle.Get());
        ConstructPCItemLists(expressions, plist_cat, ptop_list, pactions_list);
    }
}
#pragma auto_inline(on)
