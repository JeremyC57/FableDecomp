// CTCInventoryAbilities::ConstructPCMagicList(NUISystem::CComponent* parent) -- PC skills screen, magic page: owned spells, assigned palette (received order padded with empty slots), per-category sub lists with titles, expression-item and spell quick-slot containers, one menu entry per owned spell with assign / swap / drag actions, the three category blocks
// Frame: the expression-block `icon->AddActionOnDroppedNowhere(action);` is written in both arms of the isSpell if / else (c2 cross-jumps them to one call; the IL reference makes the icon weigh 9 so the packer shares its word with the spell-block assign at [ebp+30h], 0x005F225D); FABLE_RETAIL_STLP_MAP (STLport map::operator[] body visible, still called out of line) lets the CategoriesLists key temporary share [ebp+58h] (0x005F34C2); counters declared surround, physical, attack (zero stores 0x005F1CA0).
// Leaf: ?ConstructPCMagicList@CTCInventoryAbilities@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: 0x005F1B10, 8383 bytes (`ret 4` at 0x005F3BCC)
// Same-TU bodies GetAbilityLevel 0x005EDE93 / GetAbilityGraphicIndex 0x005EDE7F (auto-inlined by c2 as in retail: 0x005F2E7C,
//   0x005F2F3B, 0x005F3430 load Abilities._M_start before the index and add 1 into a new register); `descriptionText` named
//   (retail loads description.Data into edi before the L"" temp, 0x005F3420); loop-1 bound `count` set in an if / else (per-arm
//   stores 0x005F1BED / 0x005F1BF2).
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif
#define FABLE_RETAIL_CTCINVENTORYABILITIES_PCMAGIC
#define FABLE_RETAIL_CTCINVENTORYABILITIES_MAGIC
#define FABLE_RETAIL_CTCINVENTORYABILITIES_EXPRESSIONS
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_FIND
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NOINLINE
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#include "stlport/fable_algorithm.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventory.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CThingObjectDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CScrollBar.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/EHeroAbility.h"
// CAbilityDef has no shared header (as CTCInventory::AssignThingPC 0x00584B86): RTTI .?AVCAbilityDef@@ : CSubDefClassBase,
// Ego_r.pdb member Ability +0x28.
class CAbilityDef : public CSubDefClassBase {
public:
    static CCharString FABLE_FASTCALL StatGetTypeName() { return CCharString("CAbilityDef", -1); }
    EHeroAbility Ability;                                            // +0x28
};
CCharString FABLE_FASTCALL GFIntToCharString(long value);            // 0x0099F830
CWideString FABLE_FASTCALL GFIntToWideString(long value);            // 0x00405F00
using NUISystem::CComponent;
using NUISystem::CAction;
using NUISystem::CActionParamSLONG;
using NUISystem::CActionParamULONG;

#pragma auto_inline(off)
// Same-TU callees, earlier in retail's source file (real bodies; landed rows 0x005EDE37 / 0x005EDFD9): out of line, but their
// register use is known to the caller (retail keeps edx live across both calls, 0x005F1CD0 / 0x005F3454).
CTCInventoryAbilities::EUISpellCategory CTCInventoryAbilities::GetCategory(EHeroAbility ability)
{
    switch (ability) {
    case 1: case 2: case 5: case 6: case 10:
        return SPELL_CATEGORY_SURROUND;
    case 3: case 9: case 11: case 12: case 16:
        return SPELL_CATEGORY_ATTACK;
    case 4: case 8: case 13: case 14: case 15: case 17:
        return SPELL_CATEGORY_PHYSICAL;
    default:
        return SPELL_CATEGORY_ATTACK;
    }
}
CDefPointer<const CHeroAbilityDef> CTCInventoryAbilities::GetAbilityPDef(EHeroAbility ability) const
{
    return Abilities[ability].second;
}
const CDefPointer<const NUISystem::CUIMiscThingsDef> CTCInventoryAbilities::GetMiscThingsDef() const
{
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CDefPointer<const NUISystem::CUIMiscThingsDef> def;
    CGameDefinitionManager::Get().GetDef(index, &def);
    return def;
}
#pragma auto_inline(on)
long CTCInventoryAbilities::GetAbilityLevel(EHeroAbility ability) const
{
    return Abilities[ability].first;
}
long CTCInventoryAbilities::GetAbilityGraphicIndex(EHeroAbility ability) const
{
    return Abilities[ability].second->IconGraphicIndex;
}

CComponent* CTCInventoryAbilities::ConstructPCMagicList(CComponent* parent)
{
    NUISystem::CManager* ui=NUISystem::CManager::GetInstance();
    CComponent* frame=parent->FindChild("PC_UI_FRAME");
    const NGameText::CDataBank& bank=PeekComponent().GetTextBank();
    std::list<EHeroAbility> owned;
    const CDefPointer<const NUISystem::CUIMiscThingsDef> misc=GetMiscThingsDef();
    for(long n=1;n<20;++n)
        if(Abilities[n].first>=0) owned.push_back(static_cast<EHeroAbility>(n));
    AssignedAbilities.clear();
    AssignedAbilitiesSave.clear();
    unsigned long count;
    if(ReceivedAbilityOrder.size()<=misc->TotalSpellsInPalettes) count=ReceivedAbilityOrder.size();
    else count=misc->TotalSpellsInPalettes;
    for(unsigned long n=0;n<count;++n) {
        AssignedAbilities.push_back(ReceivedAbilityOrder[n]);
        AssignedAbilitiesSave.push_back(ReceivedAbilityOrder[n]);
    }
    unsigned long assigned=AssignedAbilities.size();
    if(assigned<misc->TotalSpellsInPalettes) {
        for(unsigned long n=0;n<misc->TotalSpellsInPalettes-assigned;++n) {
            EHeroAbility empty=static_cast<EHeroAbility>(0);
            EHeroAbility savedEmpty=static_cast<EHeroAbility>(0);
            AssignedAbilities.push_back(empty);
            AssignedAbilitiesSave.push_back(savedEmpty);
        }
    }
    CategoriesLists.clear();
    unsigned long surroundCount=0,physicalCount=0,attackCount=0;
    for(std::list<EHeroAbility>::iterator it=owned.begin();it!=owned.end();++it) {
        EUISpellCategory category=GetCategory(*it);
        if(category==SPELL_CATEGORY_SURROUND) ++surroundCount;
        if(category==SPELL_CATEGORY_PHYSICAL) ++physicalCount;
        if(category==SPELL_CATEGORY_ATTACK) ++attackCount;
    }
    if(surroundCount) {
        NUISystem::CList* list=static_cast<NUISystem::CList*>(ui->CreateComponent("PC_SUB_LIST",false));
        list->InitialiseOffsets(surroundCount);
        CCountedPointer<CComponent> title(ui->CreateComponent("PC_TITLE_SURROUND",false));
        list->AddNonScrollingChild(title);
        if(frame) frame->AddPositionChild(title.Get());
        CategoriesLists[SPELL_CATEGORY_SURROUND]=list;
    }
    if(attackCount) {
        NUISystem::CList* list=static_cast<NUISystem::CList*>(ui->CreateComponent("PC_SUB_LIST",false));
        list->InitialiseOffsets(attackCount);
        CCountedPointer<CComponent> title(ui->CreateComponent("PC_TITLE_ATTACK",false));
        list->AddNonScrollingChild(title);
        if(frame) frame->AddPositionChild(title.Get());
        CategoriesLists[SPELL_CATEGORY_ATTACK]=list;
    }
    if(physicalCount) {
        NUISystem::CList* list=static_cast<NUISystem::CList*>(ui->CreateComponent("PC_SUB_LIST",false));
        list->InitialiseOffsets(physicalCount);
        CCountedPointer<CComponent> title(ui->CreateComponent("PC_TITLE_PHYSICAL",false));
        list->AddNonScrollingChild(title);
        if(frame) frame->AddPositionChild(title.Get());
        CategoriesLists[SPELL_CATEGORY_PHYSICAL]=list;
    }
    NUISystem::CList* root=static_cast<NUISystem::CList*>(ui->CreateComponent("PC_MAGIC_CATEGORIES",false));
    CCountedPointer<CComponent> bottomRight(ui->CreateComponent("PC_POSITION_DUMMY_BOTTOM_RIGHT",false));
    root->AddNonScrollingChild(bottomRight);
    bottomRight->AddPositionChild(root->FindChild("PC_SPELL_CONTAINMENT_CONTAINER"));
    CCountedPointer<CComponent> bottomLeft(ui->CreateComponent("PC_POSITION_DUMMY_BOTTOM_LEFT",false));
    root->AddNonScrollingChild(bottomLeft);
    bottomLeft->AddPositionChild(root->FindChild("PC_EXPRESSIONS_ITEMS_CONTAINMENT_CONTAINER"));
    // expression item quick slots
    CTCInventory* inventory;
    ParentThing->GetTC(&inventory);
    for(unsigned long n=0;n<inventory->QuickAccessItems.size();++n) {
        CCharString name="PC_EXPRESSION_ITEM_CONTAINMENT_"+GFIntToCharString(n+1);
        CComponent* slot=root->FindChild(name);
        if(slot) {
            CCountedPointer<CAction> observe(new CAction);
            observe->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observe->AddParam(slot,observe);
            observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
            root->AddActionOnSelected(observe);
            CCountedPointer<CAction> observeDrop(new CAction);
            observeDrop->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observeDrop->AddParam(slot,observeDrop);
            observeDrop->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_COMPONENT_DROPPED));
            root->AddActionOnSelected(observeDrop);
            CCountedPointer<CAction> assign(new CAction);
            assign->ActionType=NUISystem::UI_ACTION_TYPE_ASSIGN_SPELL_TO_ITEM_SLOT_PC;
            assign->AddParam(slot,assign);
            assign->UIActionParams.push_back(new CActionParamULONG(n));
            slot->AddAction(assign);
            if(inventory->QuickAccessItems[n]==0) {
                CCountedPointer<CComponent> inside(ui->CreateComponent("PC_EXPRESSION_ITEM_INSIDE_CONTAINER",false));
                slot->AddChild(inside);
            } else {
                CCountedPointer<CComponent> icon(ui->CreateComponent("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_INSIDE_CONTAINER",false));
                CComponent* sprite=icon->FindChild("PC_EXPRESSION_ITEM_ASSIGNMENT_ICON_SPRITE");
                if(sprite && inventory->QuickAccessItems[n]!=0)
                    static_cast<NUISystem::CSprite*>(sprite)->SetGraphicIndex(inventory->GetQuickAccessItemGraphicIndex(n));
                CCountedPointer<CAction> action(new CAction);
                action->ActionType=NUISystem::UI_ACTION_TYPE_UNASSIGN_EXPRESSION_ITEM_PC;
                action->AddParam(slot,action);
                action->UIActionParams.push_back(new CActionParamULONG(n));
                icon->AddActionOnDroppedNowhere(action);
                CDefPointer<const CThingObjectDef> def;
                bool isSpell=false;
                PeekComponent().PeekDefinitionManager().GetDef(inventory->QuickAccessItems[n],&def);
                CDefPointer<const CAbilityDef> ability;
                if(!def.IsNull() && def->GetSubDef(&ability)) {
                    isSpell=true;
                    action.Reset(new CAction);
                    action->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC;
                    action->UIActionParams.push_back(new CActionParamSLONG(0));
                    icon->AddActionOnDroppedNowhere(action);
                } else {
                    action.Reset(new CAction);
                    action->ActionType=NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_PC;
                    action->UIActionParams.push_back(new CActionParamSLONG(0));
                    icon->AddActionOnDroppedNowhere(action);
                }
                CCountedPointer<CAction> follow(new CAction);
                follow->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                follow->AddParam(icon.Get(),follow);
                follow->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                root->AddActionOnSelected(follow);
                CCountedPointer<CAction> swap(new CAction);
                if(isSpell) {
                    swap->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_IN_ITEMS_SWAPPING_PC;
                    swap->UIActionParams.push_back(new CActionParamSLONG(ability->Ability));
                    swap->UIActionParams.push_back(new CActionParamSLONG(n));
                    swap->AddParam(slot);
                } else {
                    swap->ActionType=NUISystem::UI_ACTION_TYPE_SET_EXPRESSION_ITEM_TO_ASSIGN_SWAPPING_PC;
                    swap->UIActionParams.push_back(new CActionParamSLONG(inventory->QuickAccessItems[n]));
                    swap->UIActionParams.push_back(new CActionParamSLONG(n));
                    swap->AddParam(slot);
                }
                icon->AddActionOnDropped(swap);
                slot->AddChild(icon);
            }
        }
    }
    // spell quick slots
    for(unsigned long n=0;n<misc->TotalSpellsInPalettes;++n) {
        CCharString name="PC_SPELL_CONTAINMENT_"+GFIntToCharString(n+1);
        CComponent* slot=root->FindChild(name);
        if(slot) {
            CCountedPointer<CAction> observe(new CAction);
            observe->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observe->AddParam(slot,observe);
            observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
            root->AddActionOnSelected(observe);
            CCountedPointer<CAction> observeDrop(new CAction);
            observeDrop->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
            observeDrop->AddParam(slot,observeDrop);
            observeDrop->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_COMPONENT_DROPPED));
            root->AddActionOnSelected(observeDrop);
            CCountedPointer<CAction> assign(new CAction);
            assign->ActionType=NUISystem::UI_ACTION_TYPE_ASSIGN_SPELL_PC;
            assign->AddParam(slot,assign);
            assign->UIActionParams.push_back(new CActionParamULONG(n));
            slot->AddAction(assign);
            if(n<ReceivedAbilityOrder.size()) {
                if(ReceivedAbilityOrder[n]==0) {
                    CCountedPointer<CComponent> inside(ui->CreateComponent("PC_SPELL_INSIDE_CONTAINER",false));
                    slot->AddChild(inside);
                } else {
                    CCountedPointer<CComponent> icon(ui->CreateComponent("PC_SPELL_ASSIGNMENT_ICON_INSIDE_CONTAINER",false));
                    CComponent* sprite=icon->FindChild("PC_SPELL_ASSIGNMENT_ICON_SPRITE");
                    if(sprite)
                        static_cast<NUISystem::CSprite*>(sprite)->SetGraphicIndex(Abilities[ReceivedAbilityOrder[n]].second->IconGraphicIndex);
                    CCountedPointer<CAction> action(new CAction);
                    action->ActionType=NUISystem::UI_ACTION_TYPE_UNASSIGN_SPELL_PC;
                    action->AddParam(slot,action);
                    action->UIActionParams.push_back(new CActionParamULONG(n));
                    icon->AddActionOnDroppedNowhere(action);
                    action.Reset(new CAction);
                    action->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC;
                    action->UIActionParams.push_back(new CActionParamSLONG(0));
                    icon->AddActionOnDroppedNowhere(action);
                    CCountedPointer<CAction> follow(new CAction);
                    follow->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
                    follow->AddParam(icon.Get(),follow);
                    follow->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
                    root->AddActionOnSelected(follow);
                    CCountedPointer<CAction> swap(new CAction);
                    swap->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_SWAPPING_PC;
                    swap->UIActionParams.push_back(new CActionParamSLONG(ReceivedAbilityOrder[n]));
                    swap->UIActionParams.push_back(new CActionParamSLONG(n));
                    swap->AddParam(slot);
                    icon->AddActionOnDropped(swap);
                    slot->AddChild(icon);
                }
            }
        }
    }
    NUISystem::CViewport* viewport=static_cast<NUISystem::CViewport*>(parent->FindChild("PC_DESCRIPTION_VIEWPORT"));
    if(viewport) {
        CComponent* description=root->FindChild("PC_DESCRIPTION_ATTACK_SPELLS");
        if(description) viewport->AddFakeChild(description);
        description=root->FindChild("PC_DESCRIPTION_PHYSICAL_SPELLS");
        if(description) viewport->AddFakeChild(description);
        description=root->FindChild("PC_DESCRIPTION_SURROUND_SPELLS");
        if(description) viewport->AddFakeChild(description);
    }
    if(frame) {
        CComponent* child=root->FindChild("PC_TITLE_MAGIC_CATEGORIES");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_RING_PIC_MAGIC_ATTACK");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_SUBTITLE_ATTACK_SPELLS");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_DESCRIPTION_ATTACK_SPELLS");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_RING_PIC_MAGIC_SURROUND");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_SUBTITLE_SURROUND_SPELLS");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_DESCRIPTION_SURROUND_SPELLS");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_RING_PIC_MAGIC_PHYSICAL");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_SUBTITLE_PHYSICAL_SPELLS");
        if(child) frame->AddPositionChild(child);
        child=root->FindChild("PC_DESCRIPTION_PHYSICAL_SPELLS");
        if(child) frame->AddPositionChild(child);
        if(viewport) frame->AddPositionChild(viewport);
    }
    UnassignedIcons.clear();
    AbilitiesMenuEntries.clear();
    for(std::list<EHeroAbility>::iterator it=owned.begin();it!=owned.end();++it) {
        CWideString caption,displayName;
        CTCInventoryExperience* experience;
        if(ParentThing->GetTC(static_cast<ETCInterfaceType>(22),&experience)) {
            caption=displayName=experience->GetExperienceAbilityName(*it);
            caption+=misc->SpaceSeparator;
            caption+=GFIntToWideString(GetAbilityLevel(*it)+1);
            displayName+=CWideString(L" - ")+bank.GetTextBySymbol("TEXT_GUI_MENU_LEVEL")+CWideString(L" ");
            displayName+=GFIntToWideString(GetAbilityLevel(*it)+1);
        }
        CCountedPointer<CComponent> entry(ui->CreateMenuEntryComponent(caption,0.0f,1.0f,"PC_MENU_ENTRY_TEMPLATE",0.0f,CCharString(""),0.0f,CCharString("PC_BUTTON_BASTARD")));
        CCountedPointer<CComponent> icon(ui->CreateComponent("PC_SPELL_ASSIGNMENT_ICON",false));
        CComponent* sprite=icon->FindChild("PC_SPELL_ASSIGNMENT_ICON_SPRITE");
        CCountedPointer<CComponent> faded(ui->CreateComponent("PC_SPELL_ICON_FADED",false));
        NUISystem::CSprite* fadedSprite=static_cast<NUISystem::CSprite*>(faded.Get());
        CCountedPointer<CAction> assign(new CAction);
        assign->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC;
        assign->UIActionParams.push_back(new CActionParamSLONG(*it));
        icon->AddActionOnDropped(assign);
        CCountedPointer<CAction> reset(new CAction);
        reset->ActionType=NUISystem::UI_ACTION_TYPE_SET_SPELL_TO_ASSIGN_PC;
        reset->UIActionParams.push_back(new CActionParamSLONG(0));
        icon->AddActionOnDroppedNowhere(reset);
        NUISystem::CDraggable* draggable=static_cast<NUISystem::CDraggable*>(icon.Get());
        if(draggable) draggable->SetSnapBackIfDroppedNowhere(false);
        static_cast<NUISystem::CSprite*>(sprite)->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);
        fadedSprite->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);
        entry->AddChild(icon);
        entry->AddChild(faded);
        AbilitiesMenuEntries[*it]=entry.Get();
        if(std::find(AssignedAbilities.begin(),AssignedAbilities.end(),*it)==AssignedAbilities.end()) {
            CCountedPointer<CComponent> unassigned(ui->CreateComponent("PC_UNASSIGNED_SPELL_ICON",false));
            entry->AddChild(unassigned);
            UnassignedIcons[*it]=unassigned.Get();
        }
        CCountedPointer<NUISystem::CMenuEntry> menuEntry;
        menuEntry.SetStaticCast(entry);
        CCountedPointer<CComponent> name(ui->CreateComponent("PC_SUBTITLE",false));
        CCountedPointer<NUISystem::CText> text;
        text.SetStaticCast(name);
        text->SetValue(displayName,true,true);
        entry->AddChild(name);
        if(frame) frame->AddPositionChild(name.Get());
        CCountedPointer<CComponent> abilityIcon(ui->CreateComponent("PC_ICON",false));
        if(abilityIcon->GetType()==static_cast<NUISystem::EType>(0))
            static_cast<NUISystem::CSprite*>(abilityIcon.Get())->SetGraphicIndex(Abilities[*it].second->IconGraphicIndex);
        entry->AddChild(abilityIcon);
        if(frame) frame->AddPositionChild(abilityIcon.Get());
        CCountedPointer<CComponent> ring;
        switch(GetCategory(*it)) {
        case SPELL_CATEGORY_PHYSICAL: ring.Reset(ui->CreateComponent("PC_RING_PIC_MAGIC_PHYSICAL",false)); break;
        case SPELL_CATEGORY_SURROUND: ring.Reset(ui->CreateComponent("PC_RING_PIC_MAGIC_SURROUND",false)); break;
        case SPELL_CATEGORY_ATTACK: ring.Reset(ui->CreateComponent("PC_RING_PIC_MAGIC_ATTACK",false)); break;
        }
        if(!ring.IsNull()) {
            entry->AddChild(ring);
            if(frame) frame->AddPositionChild(ring.Get());
        }
        CCountedPointer<CComponent> description(ui->CreateComponent("PC_DESCRIPTION",false));
        NUISystem::CText* descriptionText=static_cast<NUISystem::CText*>(description.Get());
        descriptionText->SetValue(GetAbilityPDef(*it)->LevDescription[GetAbilityLevel(*it)],L"",true);
        entry->AddChild(description);
        if(viewport) viewport->AddFakeChild(description.Get());
        if(frame) frame->AddPositionChild(description.Get());
        NUISystem::CList* list=static_cast<NUISystem::CList*>(CategoriesLists[GetCategory(*it)]);
        list->AddScrollingChildAndSetOffsets(entry);
        CCountedPointer<CAction> observe(new CAction);
        observe->ActionType=NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observe->AddParam(icon.Get(),observe);
        observe->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observe);
        CCountedPointer<CAction> ignore(new CAction);
        ignore->ActionType=NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignore->AddParam(icon.Get(),ignore);
        ignore->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnInactive(ignore);
    }
    {
        std::map<EUISpellCategory,CComponent*>::iterator found=CategoriesLists.find(SPELL_CATEGORY_ATTACK);
        if(found!=CategoriesLists.end()) {
            CCountedPointer<CComponent> children(found->second);
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_ATTACK,root);
            entry->AddChild(children);
            NUISystem::CText* title=static_cast<NUISystem::CText*>(entry->FindChild("UI_MENU_ENTRY_ATTACK_SPELLS_TEXT"));
            if(title) title->SetValue(title->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(attackCount),true,true);
            NUISystem::CText* subtitle=static_cast<NUISystem::CText*>(entry->FindChild("PC_SUBTITLE_ATTACK_SPELLS"));
            if(subtitle) subtitle->SetValue(subtitle->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(attackCount),true,true);
        } else {
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_ATTACK,root);
            entry->MakeUnavailable();
            CComponent* arrow=entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if(arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method=NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    {
        std::map<EUISpellCategory,CComponent*>::iterator found=CategoriesLists.find(SPELL_CATEGORY_SURROUND);
        if(found!=CategoriesLists.end()) {
            CCountedPointer<CComponent> children(found->second);
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_SURROUND,root);
            entry->AddChild(children);
            NUISystem::CText* title=static_cast<NUISystem::CText*>(entry->FindChild("UI_MENU_ENTRY_SURROUND_SPELLS_TEXT"));
            if(title) title->SetValue(title->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(surroundCount),true,true);
            NUISystem::CText* subtitle=static_cast<NUISystem::CText*>(entry->FindChild("PC_SUBTITLE_SURROUND_SPELLS"));
            if(subtitle) subtitle->SetValue(subtitle->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(surroundCount),true,true);
        } else {
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_SURROUND,root);
            entry->MakeUnavailable();
            CComponent* arrow=entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if(arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method=NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    {
        std::map<EUISpellCategory,CComponent*>::iterator found=CategoriesLists.find(SPELL_CATEGORY_PHYSICAL);
        if(found!=CategoriesLists.end()) {
            CCountedPointer<CComponent> children(found->second);
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_PHYSICAL,root);
            entry->AddChild(children);
            NUISystem::CText* title=static_cast<NUISystem::CText*>(entry->FindChild("UI_MENU_ENTRY_PHYSICAL_SPELLS_TEXT"));
            if(title) title->SetValue(title->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(physicalCount),true,true);
            NUISystem::CText* subtitle=static_cast<NUISystem::CText*>(entry->FindChild("PC_SUBTITLE_PHYSICAL_SPELLS"));
            if(subtitle) subtitle->SetValue(subtitle->GetValue()+CTCInventoryStats::GetXString()+GFIntToWideString(physicalCount),true,true);
        } else {
            CComponent* entry=GetMenuEntryFromCategory(SPELL_CATEGORY_PHYSICAL,root);
            entry->MakeUnavailable();
            CComponent* arrow=entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if(arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method=NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    return root;
}
