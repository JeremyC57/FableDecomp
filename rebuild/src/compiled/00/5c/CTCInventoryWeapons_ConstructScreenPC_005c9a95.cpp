// CTCInventoryWeapons::ConstructScreenPC(NUISystem::CComponent* parent) -- builds the PC weapons screen: registers the screen and the hero with the UI manager, sorts the hero's items into melee / ranged weapons and augmentations, then fills the PC_SUB_LIST with the augmentation entry, the title, a melee and a ranged menu entry (guide text, subtitle, ring picture), all placed in the parent's PC_UI_FRAME, and per non-empty category a sub-list filled by ConstructWeaponItemEntriesPC
// Leaf: ?ConstructScreenPC@CTCInventoryWeapons@@UAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C9A95, thiscall `ret 4`, no EH frame, biased frame (`push ebp ; lea ebp,[esp-74h] ; sub esp,0C8h`).
// Identity: slot 87 of the CTCInventoryWeapons vtable 0x01253FF4; Ego_r.pdb / FableWin.pdb ?ConstructScreenPC@CTCInventoryWeapons@@UAEPAVCComponent@NUISystem@@PAV23@@Z;
//   the PC twin of ConstructScreen 0x005C8459 (same TU, same shape devices; see that row's notes).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_DAMAGE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_ISAUGMENTATION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x004CC340

#include "engine/CWeaponDef.h"
#include "engine/CThingShotDef.h"

// Same-TU bodies of the def lookups retail's TU holds (the header member templates; out of line in retail: GetDef<CUIMiscThingsDef>
// 0x0042B312, GetDef<CDefClassBase> 0x0042AF3C, GetSubDef<CWeaponDef> 0x0042B1A6). VC7.1 sees that they only write through the
// out pointer, so the CDefPointer locals whose addresses they take stay unaliased (retail's `dec dword ptr [ecx+4] ; jne` releases).
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

#if defined(_MSC_VER)
// Same-TU stand-ins (MSVC build only) for the two PC builders retail's TU defines above ConstructScreenPC (Ego_r.pdb donor order:
// ConstructAugEntryPC, ConstructWeaponItemEntriesPC, then ConstructScreenPC): VC7.1 reads their bodies and sees that the references
// they get are never stored, so the screen's locals stay uncaptured. Their real bodies are the rows 0x005C5F11 / 0x005C8176.
void StandInBody();     // never defined, MSVC build only: an empty stand-in would be folded away with its calls
#pragma auto_inline(off)
CCountedPointer<NUISystem::CComponent> CTCInventoryWeapons::ConstructAugEntryPC(bool, std::vector<std::pair<long, long> >&, NUISystem::CViewport*,
    NUISystem::CComponent*)
{
    StandInBody();
    return CCountedPointer<NUISystem::CComponent>();
}
void CTCInventoryWeapons::ConstructWeaponItemEntriesPC(std::vector<std::pair<long, long> >&, NUISystem::CList*, CCountedPointer<NUISystem::CComponent>&,
    NUISystem::CComponent*, NUISystem::CManager*, NUISystem::CViewport*, CDefPointer<const NUISystem::CUIMiscThingsDef>&, float,
    CCountedPointer<NUISystem::CComponent>&)
{
    StandInBody();
}
#pragma auto_inline(on)
#endif

NUISystem::CComponent* CTCInventoryWeapons::ConstructScreenPC(NUISystem::CComponent* parent)
{
    NUISystem::CManager::GetInstance()->SetWeaponsInventory(this);
    NUISystem::CManager::GetInstance()->SetPlayerThing(ParentThing);
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);
    PAssignedRangedIcon = 0;
    PAssignedMeleeIcon = 0;
    PAssignedRangedMenuEntry = 0;
    PAssignedMeleeMenuEntry = 0;
    PRangedList = 0;
    PMeleeList = 0;
    AvailableWeaponsEntries.clear();

    // sort the owned items into melee weapons, ranged weapons and augmentations
    std::vector<std::pair<long, long> > items;
    std::vector<std::pair<long, long> > meleeItems;
    std::vector<std::pair<long, long> > rangedItems;
    std::vector<std::pair<long, long> > augmentations;
    GetAllItems(items);
    long augmentationCount = 0;
    for (unsigned long i = 0; i < items.size(); ++i) {
        CDefPointer<const CDefClassBase> def;
        GetDefinitionManager().GetDef(items[i].first, &def);
        CDefPointer<const CWeaponDef> weaponDef;
        if (def->GetSubDef(&weaponDef)) {
            if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class))
                meleeItems.push_back(items[i]);
            else
                rangedItems.push_back(items[i]);
        } else if (CTCInventoryItem::IsAugmentation(items[i].first)) {
            augmentations.push_back(items[i]);
            augmentationCount += items[i].second;
        }
    }

    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    NUISystem::CComponent* frame = parent->FindChild("PC_UI_FRAME");
    manager->SetNumberAugmentations(augmentationCount);
    CCountedPointer<NUISystem::CComponent> augmentationEntry;
    NUISystem::CList* list = static_cast<NUISystem::CList*>(manager->CreateComponent("PC_SUB_LIST", false));
    NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(parent->FindChild("PC_DESCRIPTION_VIEWPORT"));
    frame->AddPositionChild(viewport);
    if (augmentations.size() > 0)
        augmentationEntry = ConstructAugEntryPC(true, augmentations, viewport, frame);
    CCountedPointer<NUISystem::CComponent> title(manager->CreateComponent("PC_TITLE_WEAPONS", false));
    list->AddNonScrollingChild(title);
    frame->AddPositionChild(title.Get());
    WeaponMenuItemMap.clear();

    // melee entry
    CWideString meleeText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(miscDef->WeaponCatMeleeString.ToCharString());
    if (!meleeItems.empty()) {
        meleeText += CTCInventoryStats::GetXString();
        meleeText += GFIntToWideString(meleeItems.size());
    }
    CCountedPointer<NUISystem::CComponent> meleeEntry(manager->CreateMenuEntryComponent(meleeText, 0.0f, 1.0f,
        "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
    list->AddScrollingChildAndSetOffsets(meleeEntry);
    CCountedPointer<NUISystem::CComponent> description(manager->CreateComponent("PC_DESCRIPTION", false));
    NUISystem::CText* descriptionText = static_cast<NUISystem::CText*>(description.Get());
    const NGameText::CDataBank& guideBank = CMainGameComponent::Get()->GetTextBank();
    descriptionText->SetValue(guideBank.GetTextBySymbol("TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_PC"), true, true);
    meleeEntry->AddChild(description);
    viewport->AddFakeChild(description.Get());
    frame->AddPositionChild(description.Get());
    CCountedPointer<NUISystem::CComponent> meleeSubtitle(manager->CreateComponent("PC_SUBTITLE", false));
    static_cast<NUISystem::CText*>(meleeSubtitle.Get())->SetValue(meleeText, true, true);
    meleeEntry->AddChild(meleeSubtitle);
    frame->AddPositionChild(meleeSubtitle.Get());

    // ranged entry
    CWideString rangedText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(miscDef->WeaponCatRangedString.ToCharString());
    if (!rangedItems.empty()) {
        rangedText += CTCInventoryStats::GetXString();
        rangedText += GFIntToWideString(rangedItems.size());
    }
    CCountedPointer<NUISystem::CComponent> rangedEntry(manager->CreateMenuEntryComponent(rangedText, 0.0f, 1.0f,
        "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
    list->AddScrollingChildAndSetOffsets(rangedEntry);
    description.Reset(manager->CreateComponent("PC_DESCRIPTION", false));
    descriptionText = static_cast<NUISystem::CText*>(description.Get());
    descriptionText->SetValue(guideBank.GetTextBySymbol("TEXT_GUI_MENUGUIDE_WEAPONS_RANGED_PC"), true, true);
    rangedEntry->AddChild(description);
    viewport->AddFakeChild(description.Get());
    frame->AddPositionChild(description.Get());
    CCountedPointer<NUISystem::CComponent> rangedSubtitle(manager->CreateComponent("PC_SUBTITLE", false));
    static_cast<NUISystem::CText*>(rangedSubtitle.Get())->SetValue(rangedText, true, true);
    rangedEntry->AddChild(rangedSubtitle);
    frame->AddPositionChild(rangedSubtitle.Get());

    CCountedPointer<NUISystem::CComponent> meleeTitle(manager->CreateComponent("PC_TITLE", false));
    NUISystem::CText* meleeTitleText = static_cast<NUISystem::CText*>(meleeTitle.Get());
    if (meleeTitleText)
        meleeTitleText->SetValue("TEXT_GUI_MENU_MELEE", true, true);
    frame->AddPositionChild(meleeTitle.Get());
    CCountedPointer<NUISystem::CComponent> meleeItemTitle(manager->CreateComponent("PC_TITLE", false));
    if (!meleeItemTitle.IsNull())
        static_cast<NUISystem::CText*>(meleeItemTitle.Get())->SetValue("TEXT_GUI_MENU_MELEE", true, true);
    frame->AddPositionChild(meleeItemTitle.Get());
    CCountedPointer<NUISystem::CComponent> meleeRing(manager->CreateComponent("PC_RING_PIC_MELEE", false));
    meleeEntry->AddChild(meleeRing);
    frame->AddPositionChild(meleeRing.Get());
    CCountedPointer<NUISystem::CComponent> rangedRing(manager->CreateComponent("PC_RING_PIC_RANGED", false));
    rangedEntry->AddChild(rangedRing);
    frame->AddPositionChild(rangedRing.Get());

    // melee weapons: an unavailable entry, or the melee sub-list
    if (meleeItems.empty()) {
        meleeEntry->MakeUnavailable();
    } else {
        CCountedPointer<NUISystem::CComponent> meleeList(manager->CreateComponent("PC_SUB_LIST", false));
        NUISystem::CList* subList = static_cast<NUISystem::CList*>(meleeList.Get());
        meleeEntry->AddChild(meleeList);
        subList->AddNonScrollingChild(meleeTitle);
        subList->InitialiseOffsets(meleeItems.size());
        float damage = 0.0f;
        if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
            const CTCWeapon* weapon;
            static_cast<CThing*>(PActiveMeleeWeapon.GetPItem())->PeekTC(&weapon);
            damage = weapon->GetDamage();
        }
        ConstructWeaponItemEntriesPC(meleeItems, subList, augmentationEntry, parent, manager, viewport, miscDef, damage, meleeItemTitle);
    }

    CCountedPointer<NUISystem::CComponent> rangedTitle(manager->CreateComponent("PC_TITLE", false));
    NUISystem::CText* rangedTitleText = static_cast<NUISystem::CText*>(rangedTitle.Get());
    if (rangedTitleText)
        rangedTitleText->SetValue("TEXT_GUI_MENU_RANGED", true, true);
    frame->AddPositionChild(rangedTitle.Get());
    CCountedPointer<NUISystem::CComponent> rangedItemTitle(manager->CreateComponent("PC_TITLE", false));
    NUISystem::CText* rangedItemTitleText = static_cast<NUISystem::CText*>(rangedItemTitle.Get());
    if (rangedItemTitleText)
        rangedItemTitleText->SetValue("TEXT_GUI_MENU_RANGED", true, true);
    frame->AddPositionChild(rangedItemTitle.Get());

    // ranged weapons
    if (rangedItems.empty()) {
        rangedEntry->MakeUnavailable();
    } else {
        CCountedPointer<NUISystem::CComponent> rangedList(manager->CreateComponent("PC_SUB_LIST", false));
        NUISystem::CList* subList = static_cast<NUISystem::CList*>(rangedList.Get());
        rangedEntry->AddChild(rangedList);
        subList->AddNonScrollingChild(rangedTitle);
        subList->InitialiseOffsets(rangedItems.size());
        float damage = 0.0f;
        if (IS_THING_ALIVE(PActiveRangedWeapon)) {
            CDefPointer<const CWeaponDef> weaponDef;
            static_cast<CThing*>(PActiveRangedWeapon.GetPItem())->GetPBaseDef()->GetSubDef(&weaponDef);
            CDefPointer<const CThingShotDef> shot(GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot));
            damage = shot->HitThingDamage;
        }
        ConstructWeaponItemEntriesPC(rangedItems, subList, augmentationEntry, parent, manager, viewport, miscDef, damage, rangedItemTitle);
    }
    WeaponsList = list;
    return list;
}