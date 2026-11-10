// CTCInventoryWeapons::ConstructAugEntryPC(bool melee, std::vector<std::pair<long, long> >& augmentations, NUISystem::CViewport* viewport, NUISystem::CComponent* parent) -- the PC twin of ConstructAugEntry: the "Augment x<total>" PC menu entry with its left-description text, a PC_SUB_LIST with a title, a subtitle, and one entry per owned augmentation item (subtitle, icon, details text, add-child-augmentation / delete-all / augment actions); every child it adds is also registered as a position child of the given parent, which the add-child action carries as its parameter
// Leaf: ?ConstructAugEntryPC@CTCInventoryWeapons@@QAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@_NAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCViewport@NUISystem@@PAVCComponent@6@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C5F11, 1953 bytes, thiscall `ret 14h` (hidden result), no EH frame; biased frame
//   (`lea ebp,[esp-64h] ; sub esp,0B4h`): optimize("s",on), as the landed screens of this TU.
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructAugEntryPC@CTCInventoryWeapons@@QAE?AV?$CCountedPointer@VCComponent@NUISystem@@@@_NAAV?$vector@...@PAVCViewport@NUISystem@@PAVCComponent@6@@Z
//   (Ego_r donor 0x0062AC7B, after ConstructAugEntry, the retail order); caller ConstructScreenPC 0x005C9A95.
// Callees: as ConstructAugEntry 0x005C574E, plus CAction::AddParam(CActionParam*, CCountedPointer<CAction>&) 0x0053FDE0 and the
//   parent's CComponent slot 124 AddPositionChild (+0x1F0, under a null test).
// Data: AUG_DESCRIPTION_STRING 0x013B8BB0, AUGMENTATIONS_LEFT_DESCRIPTION_STRING_M / _R 0x013B8BB8 / 0x013B8BBC,
//   AUGMENTATIONS_MENU_ENTRY_NAME 0x013B8BC0 (the TU's anonymous-namespace CCharStrings, Ego_r.pdb initializer names).
// Shape: the console body with the PC templates; FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA (visible release) as ConstructAugEntry;
//   the parent's CActionParam conversion (null-guarded) is loop-invariant and VC7.1 hoists it to the inner loop's preheader
//   (retail 0x005C6226), and the entry pointer stays in edi through the header part.
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

CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00

namespace {                                                         // this TU's anonymous-namespace strings (Ego_r.pdb initializer names)
extern const CCharString AUGMENTATIONS_LEFT_DESCRIPTION_STRING_M;   // 0x013B8BB8 "Augmentations_Left_Description_M"
extern const CCharString AUGMENTATIONS_LEFT_DESCRIPTION_STRING_R;   // 0x013B8BBC "Augmentations_Left_Description_R"
extern const CCharString AUGMENTATIONS_MENU_ENTRY_NAME;             // 0x013B8BC0 "Augmentations_Menu_Entry_Name"
extern const CCharString AUG_DESCRIPTION_STRING;                    // 0x013B8BB0 "Augmentation_Descriptions"
}

// Same-TU bodies of the def lookups retail's TU holds (out of line in retail: GetDef<CDefClassBase> 0x0042AF3C, GetSubDef<CAugmentationDef>
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

CCountedPointer<NUISystem::CComponent> CTCInventoryWeapons::ConstructAugEntryPC(bool melee, std::vector<std::pair<long, long> >& augmentations, NUISystem::CViewport* viewport, NUISystem::CComponent* parent)
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CCountedPointer<NUISystem::CComponent> subListComponent;
    CCountedPointer<NUISystem::CList> subList;
    CWideString leftDescription;
    ConstructAugmentationsLeftDescription(leftDescription, augmentations, melee);
    CCountedPointer<NUISystem::CComponent> description(manager->CreateComponent("PC_DESCRIPTION", false));
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
    CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(title, 0.0f, 1.0f,
        "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
    entry->SetName(AUGMENTATIONS_MENU_ENTRY_NAME);
    subListComponent.Reset(manager->CreateComponent("PC_SUB_LIST", false));
    subList.SetStaticCast(subListComponent);
    CCountedPointer<NUISystem::CComponent> augTitle(manager->CreateComponent("PC_TITLE_AUGMENTATIONS", false));
    augTitle->SetName("Aug Title");
    subList->AddNonScrollingChild(augTitle);
    if (parent)
        parent->AddPositionChild(augTitle.Get());
    entry->AddChild(subListComponent);
    entry->AddChild(description);
    if (parent)
        parent->AddPositionChild(description.Get());
    CCountedPointer<NUISystem::CComponent> name(manager->CreateComponent("PC_SUBTITLE", false));
    static_cast<NUISystem::CText*>(name.Get())->SetValue(title, true, true);
    entry->AddChild(name);
    if (parent)
        parent->AddPositionChild(name.Get());
    subList->InitialiseOffsets(total);
    // one entry per owned augmentation item
    for (unsigned long i = 0; i < augmentations.size(); ++i) {
        for (long j = 0; j < augmentations[i].second; ++j) {
            CWideString itemName;
            CDefPointer<const CDefClassBase> def;
            GetDefinitionManager().GetDef(augmentations[i].first, &def);
            CDefPointer<const CAugmentationDef> augmentationDef;
            def->GetSubDef(&augmentationDef);
            itemName = CTCInventoryItem::GetDescription(augmentations[i].first);
            CCountedPointer<NUISystem::CComponent> itemEntry(manager->CreateMenuEntryComponent(itemName, 0.0f, 1.0f,
                "PC_MENU_ENTRY_TEMPLATE", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
            CCountedPointer<NUISystem::CComponent> itemNameComponent(manager->CreateComponent("PC_SUBTITLE", false));
            CCountedPointer<NUISystem::CText> itemNameText;
            itemNameText.SetStaticCast(itemNameComponent);
            itemNameText->SetValue(itemName, true, true);
            itemEntry->AddChild(itemNameComponent);
            if (parent)
                parent->AddPositionChild(itemNameComponent.Get());
            CCountedPointer<NUISystem::CComponent> sprite(manager->CreateComponent("PC_ICON", false));
            if (sprite->GetType() == NUISystem::UI_TYPE_SPRITE)
                static_cast<NUISystem::CSprite*>(sprite.Get())->SetGraphicIndex(augmentationDef->InventoryGraphic);
            itemEntry->AddChild(sprite);
            if (parent)
                parent->AddPositionChild(sprite.Get());
            CWideString details = CTCInventoryItem::GetDetails(augmentations[i].first);
            CCountedPointer<NUISystem::CComponent> itemDescription(manager->CreateComponent("PC_DESCRIPTION", false));
            CCountedPointer<NUISystem::CText> itemDescriptionText;
            itemDescription->SetName(AUG_DESCRIPTION_STRING);
            viewport->AddFakeChild(itemDescription.Get());
            itemDescriptionText.SetStaticCast(itemDescription);
            itemDescriptionText->SetValue(details, true, true);
            itemEntry->AddChild(itemDescription);
            if (parent)
                parent->AddPositionChild(itemDescription.Get());
            CCountedPointer<NUISystem::CAction> showAction(new NUISystem::CAction);
            showAction->ActionType = NUISystem::UI_ACTION_TYPE_ADD_CHILD_AUGMENTATION;
            showAction->UIActionParams.push_back(new NUISystem::CActionParamULONG(augmentationDef->InventoryGraphic));
            showAction->AddParam(parent, showAction);
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