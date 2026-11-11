// CTCInventoryWeapons::ConstructAugEntry(bool melee, std::vector<std::pair<long, long> >& augmentations, NUISystem::CViewport* viewport) -- builds the console "Augment x<total>" menu entry: its left-description text (ConstructAugmentationsLeftDescription, shown in the viewport), a UI_SUB_LIST with back / select / title children, a name text, and one action entry per owned augmentation item (name text, its inventory sprite, its details text in the viewport, and the add-child-augmentation / delete-all / augment actions); returns the entry
// Leaf: ?ConstructAugEntry@CTCInventoryWeapons@@QAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@_NAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCViewport@NUISystem@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C574E, 1987 bytes, thiscall `ret 10h` (hidden result), no EH frame; biased frame
//   (`lea ebp,[esp-68h] ; sub esp,0C0h`): optimize("s",on), as the landed screens of this TU.
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructAugEntry@CTCInventoryWeapons@@QAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@_NAAV?$vector@...@PAVCViewport@NUISystem@@@Z
//   (Ego_r donor 0x0062A4AE, after ConstructAugmentationsLeftDescription, the retail order); caller ConstructScreen 0x005C8459.
// Callees: CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D / CreateMenuEntryComponent 0x00428494,
//   ConstructAugmentationsLeftDescription 0x005C4F42, CTCInventoryStats::GetXString 0x005CC772, GFIntToWideString 0x00405F00,
//   CDataBank::GetTextBySymbol 0x009C95E0, CTCBase::GetDefinitionManager 0x006869D0, GetDef<CThingBaseDef> 0x0042AF3C,
//   GetSubDef<CAugmentationDef> 0x0042B248, CTCInventoryItem::GetDescription 0x005D95C0 / GetDetails 0x005D9630 (static __fastcall),
//   CCountedPointer<CComponent>(T*) 0x00429C15 / copy 0x00428928 / Reset 0x00429C6F / ReleaseData 0x004291DE,
//   CCountedPointer<CList>::ShareData 0x00429486 / ReleaseData 0x00429288, CCountedPointer<CText>::ShareData 0x00429460 / ReleaseData
//   0x0042925C, CAction() 0x0042BE50 + CCountedPointer<CAction>(T*) 0x0042AA29 / ReleaseData 0x00429160, operator new 0x00BFEA1A,
//   list<CActionParam*>::push_back 0x0042B646, list<long>::push_back 0x0042B687; component slots 59 AddChild, 65 GetType,
//   66 AddAction, 83 SetName, CViewport 142 AddFakeChild, CText 144 SetValue, CSprite 145 SetGraphicIndex, CList 145
//   AddNonScrollingChild, 152 AddScrollingChildAndSetOffsets, 156 InitialiseOffsets.
// Data: the TU's anonymous-namespace CCharStrings (Ego_r.pdb dynamic-initializer names, retail order and literals):
//   AUG_DESCRIPTION_STRING 0x013B8BB0 "Augmentation_Descriptions", AUGMENTATIONS_LEFT_DESCRIPTION_STRING_M / _R 0x013B8BB8 / 0x013B8BBC,
//   AUGMENTATIONS_MENU_ENTRY_NAME 0x013B8BC0; floats 0.6666667f 0x012544AC, 25.0f 0x0123003C; CActionParamULONG vtable 0x0122F844.
// Shape: the def lookups are the same-TU template bodies; FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (the release body visible: VC7.1
//   still calls it out of line, but ShareData's mod/ref summary then leaves the sub-list's counted pointer uncaptured, so its CList
//   pointer stays in edi from 0x005C598C through the loop as in retail, and the -1 length goes back to an immediate). The augment
//   action's own-entry parameter is the CActionParam base of the entry by reference (no null test, retail 0x005C5DCB `add eax,18h`);
//   the delete action's is the item entry pointer (null-guarded `neg/sbb/and`, 0x005C5D60).
// Name fix (agentNF7 2026-11-10): 0x0042AF3C is CDefinitionManager::GetDef<CThingBaseDef> (FableWin.pdb; NF5/NF6 STATUS): the out handle retyped CDefPointer<const CThingBaseDef> (shared engine/CThingBaseDef.h), so the template binds that instantiation.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_DETAILS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAUGMENTATIONDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CAugmentationDef.h"
#include "engine/CThingBaseDef.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00

namespace {                                                         // this TU's anonymous-namespace strings (Ego_r.pdb initializer names)
extern const CCharString AUGMENTATIONS_LEFT_DESCRIPTION_STRING_M;   // 0x013B8BB8 "Augmentations_Left_Description_M"
extern const CCharString AUGMENTATIONS_LEFT_DESCRIPTION_STRING_R;   // 0x013B8BBC "Augmentations_Left_Description_R"
extern const CCharString AUGMENTATIONS_MENU_ENTRY_NAME;             // 0x013B8BC0 "Augmentations_Menu_Entry_Name"
extern const CCharString AUG_DESCRIPTION_STRING;                    // 0x013B8BB0 "Augmentation_Descriptions"
}

// Same-TU bodies of the def lookups retail's TU holds (out of line in retail: GetDef<CThingBaseDef> 0x0042AF3C, GetSubDef<CAugmentationDef>
// 0x0042B248): VC7.1 sees that they only write through the out pointer, so the CDefPointer locals stay unaliased.
#pragma auto_inline(off)
template <class T>
bool CDefClassBase::GetSubDef(CDefPointer<const T>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(T::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
template <class T>
bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
#pragma auto_inline(on)

CCountedPointer<NUISystem::CComponent> CTCInventoryWeapons::ConstructAugEntry(bool melee, std::vector<std::pair<long, long> >& augmentations, NUISystem::CViewport* viewport)
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> subListComponent;
    CCountedPointer<NUISystem::CList> subList;
    CWideString leftDescription;
    ConstructAugmentationsLeftDescription(leftDescription, augmentations, melee);
    CCountedPointer<NUISystem::CComponent> description(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
    description->SetName(melee ? AUGMENTATIONS_LEFT_DESCRIPTION_STRING_M : AUGMENTATIONS_LEFT_DESCRIPTION_STRING_R);
    viewport->AddFakeChild(description.Get());
    static_cast<NUISystem::CText*>(description.Get())->SetValue(leftDescription, true, true);
    // "Augment x<total>"
    CWideString title = bank.GetTextBySymbol("TEXT_GUI_MENU_AUGMENT");
    title += CTCInventoryStats::GetXString();
    long total = 0;
    for (unsigned long i = 0; i < augmentations.size(); ++i)
        total += augmentations[i].second;
    title += GFIntToWideString(total);
    CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(title, 25.0f, 0.6666667f,
        "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
    entry->SetName(AUGMENTATIONS_MENU_ENTRY_NAME);
    subListComponent.Reset(manager->CreateComponent("UI_SUB_LIST", false));
    subList.SetStaticCast(subListComponent);
    CCountedPointer<NUISystem::CComponent> back(manager->CreateComponent("UI_BACK_FOR_LIST", false));
    CCountedPointer<NUISystem::CComponent> select(manager->CreateComponent("UI_SELECT_FOR_LIST", false));
    subList->AddNonScrollingChild(back);
    subList->AddNonScrollingChild(select);
    CCountedPointer<NUISystem::CComponent> augTitle(manager->CreateComponent("UI_TITLE_AUGMENTATIONS_NEW", false));
    augTitle->SetName("Aug Title");
    subList->AddNonScrollingChild(augTitle);
    entry->AddChild(subListComponent);
    entry->AddChild(description);
    CCountedPointer<NUISystem::CComponent> name(manager->CreateComponent("UI_TEXT_TRADE_NAME_TEMPLATE_NEW_ONE_OFF", false));
    static_cast<NUISystem::CText*>(name.Get())->SetValue(title, true, true);
    entry->AddChild(name);
    subList->InitialiseOffsets(total);
    // one entry per owned augmentation item
    for (unsigned long i = 0; i < augmentations.size(); ++i) {
        for (long j = 0; j < augmentations[i].second; ++j) {
            CWideString itemName;
            CDefPointer<const CThingBaseDef> def;
            GetDefinitionManager().GetDef(augmentations[i].first, &def);
            CDefPointer<const CAugmentationDef> augmentationDef;
            def->GetSubDef(&augmentationDef);
            itemName = CTCInventoryItem::GetDescription(augmentations[i].first);
            CCountedPointer<NUISystem::CComponent> itemEntry(manager->CreateMenuEntryComponent(itemName, 0.0f, 1.0f,
                "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
            CCountedPointer<NUISystem::CComponent> itemNameComponent(manager->CreateComponent("UI_TEXT_AUGMENTATION_NAME_TEMPLATE_NEW", false));
            CCountedPointer<NUISystem::CText> itemNameText;
            itemNameText.SetStaticCast(itemNameComponent);
            itemNameText->SetValue(itemName, true, true);
            itemEntry->AddChild(itemNameComponent);
            CCountedPointer<NUISystem::CComponent> sprite(manager->CreateComponent("UI_AUGMENTATION_SPRITE_TEMPLATE", false));
            if (sprite->GetType() == NUISystem::UI_TYPE_SPRITE)
                static_cast<NUISystem::CSprite*>(sprite.Get())->SetGraphicIndex(augmentationDef->InventoryGraphic);
            itemEntry->AddChild(sprite);
            CWideString details = CTCInventoryItem::GetDetails(augmentations[i].first);
            CCountedPointer<NUISystem::CComponent> itemDescription(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
            CCountedPointer<NUISystem::CText> itemDescriptionText;
            itemDescription->SetName(AUG_DESCRIPTION_STRING);
            viewport->AddFakeChild(itemDescription.Get());
            itemDescriptionText.SetStaticCast(itemDescription);
            itemDescriptionText->SetValue(details, true, true);
            itemEntry->AddChild(itemDescription);
            CCountedPointer<NUISystem::CAction> showAction(new NUISystem::CAction);
            showAction->ActionType = NUISystem::UI_ACTION_TYPE_ADD_CHILD_AUGMENTATION;
            showAction->UIActionParams.push_back(new NUISystem::CActionParamULONG(augmentationDef->InventoryGraphic));
            itemEntry->AddAction(showAction);
            CCountedPointer<NUISystem::CAction> deleteAction(new NUISystem::CAction);
            deleteAction->ActionType = NUISystem::UI_ACTION_TYPE_DELETE_ALL;
            deleteAction->UIActionParams.push_back(itemEntry.Get());
            itemEntry->AddAction(deleteAction);
            CCountedPointer<NUISystem::CAction> augmentAction(new NUISystem::CAction);
            augmentAction->ActionType = NUISystem::UI_ACTION_TYPE_AUGMENT;
            augmentAction->ThingActionParams.push_back(augmentations[i].first);
            augmentAction->UIActionParams.push_back(&static_cast<NUISystem::CActionParam&>(*entry));
            itemEntry->AddAction(augmentAction);
            subList->AddScrollingChildAndSetOffsets(itemEntry);
        }
    }
    return entry;
}