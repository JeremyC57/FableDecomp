// CTCInventoryWeapons::ConstructScreen(NUISystem::CComponent* parent) -- builds the console weapons screen: sorts the hero's items into melee / ranged weapons and augmentations, then the UI_LIST_WEAPONS list with the augmentation entry, a melee and a ranged menu entry (guide text, subtitle, ring picture) and, per non-empty category, a sub-list filled by ConstructWeaponItemEntries
// Leaf: ?ConstructScreen@CTCInventoryWeapons@@UAEPAVCComponent@NUISystem@@PAV23@@Z
// Shape notes (agentSCR, RELOCATION_MATCH): the def lookups GetDef<T> / GetSubDef<T> and the two builders ConstructAugEntry / ConstructWeaponItemEntries are visible in the TU (retail compiles them above ConstructScreen: Ego_r.pdb donor order), so the def pointers, vectors and counted pointers they get stay uncaptured (in-place `dec [ecx+4] ; jne` releases, the 0xE0 frame); the CCountedPointer(T*) ctor is the visible noinline COMDAT (FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR); the def releases use FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE (`damage = shot->HitThingDamage` loads before the release). The melee / ranged damage blocks are the bodies of GetActiveMeleeWeaponDamage 0x005C4769 / GetActiveProjectileWeaponDamage 0x005C3180, which retail expands here (both are called out of line only from 0x0042367A / 0x00423681); written out, as a call they stay calls.
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C8459, 3235 bytes, thiscall `ret 4`, no EH frame. Biased frame (`push ebp ; lea ebp,[esp-74h] ;
//   sub esp,0E0h`): optimize("s",on) alone (as the landed CTCInventoryAbilities::ConstructScreen 0x005F519F).
// Identity: slot 77 of the CTCInventoryWeapons vtable 0x01253FF4 (cell 0x01254128); Ego_r.pdb / FableWin.pdb
//   ?ConstructScreen@CTCInventoryWeapons@@UAEPAVCComponent@NUISystem@@PAV23@@Z; the console twin of ConstructScreenPC 0x005C9A95 (slot 87).
// Models: engine/CWeaponDef.h (new: Class +0x2C, Shot +0xA0), engine/CThingShotDef.h (HitThingDamage +0x54), engine/CTCInventoryWeapons.h (new).
// Callees: CGameDefinitionManager::Get 0x0044C6B0, GetGlobalIndexFromClassIndex 0x009AD390, GetDef<CUIMiscThingsDef> 0x0042B312,
//   list<CComponent*>::clear 0x0042A18F, GetAllItems 0x005BF482, CTCBase::GetDefinitionManager 0x006869D0, GetDef 0x0042AF3C,
//   GetSubDef<CWeaponDef> 0x0042B1A6, CTCWeapon::IsMeleeWeaponClass 0x005DA510, CTCInventoryItem::IsAugmentation 0x005D9870,
//   vector<pair<long,long> >::push_back 0x005947B6, CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D /
//   CreateMenuEntryComponent 0x00428494 / SetNumberAugmentations (slot 30), ConstructAugEntry 0x005C574E, map clear 0x004DB806,
//   CTCInventoryStats::GetXString 0x005CC772, GFIntToWideString 0x00405F00, CDeletion 0x0042BED4 / 0x0042CD84,
//   IS_THING_ALIVE 0x004CC340, GetPItem 0x00A01B50, CThing::PeekTC (expanded; const CVectorMap::LowerBound 0x004365B0), CTCWeapon::GetDamage 0x005DAAA0, GetPDefFromGlobalIndex 0x009AD9E0,
//   ConstructWeaponItemEntries 0x005C66B2.
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
// Same-TU stand-ins (MSVC build only) for the two builders retail's TU defines above ConstructScreen (Ego_r.pdb donor order:
// ConstructAugEntry, ConstructWeaponItemEntries, then ConstructScreen): VC7.1 reads their bodies and sees that the references
// they get are never stored, so the screen's locals stay uncaptured. Their real bodies are the rows 0x005C574E / 0x005C66B2.
void StandInBody();     // never defined, MSVC build only: an empty stand-in would be folded away with its calls
#pragma auto_inline(off)
CCountedPointer<NUISystem::CComponent> CTCInventoryWeapons::ConstructAugEntry(bool, std::vector<std::pair<long, long> >&, NUISystem::CViewport*)
{
    StandInBody();
    return CCountedPointer<NUISystem::CComponent>();
}
void CTCInventoryWeapons::ConstructWeaponItemEntries(std::vector<std::pair<long, long> >&, NUISystem::CList*, CCountedPointer<NUISystem::CComponent>&,
    NUISystem::CComponent*, NUISystem::CManager*, NUISystem::CViewport*, CDefPointer<const NUISystem::CUIMiscThingsDef>&, float,
    CCountedPointer<NUISystem::CComponent>&)
{
    StandInBody();
}
#pragma auto_inline(on)
#endif

NUISystem::CComponent* CTCInventoryWeapons::ConstructScreen(NUISystem::CComponent* parent)
{
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
    manager->SetNumberAugmentations(augmentationCount);
    CCountedPointer<NUISystem::CComponent> augmentationEntry;
    NUISystem::CComponent* list = manager->CreateComponent("UI_LIST_WEAPONS", false);
    NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(list->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    if (augmentations.size() > 0)
        augmentationEntry = ConstructAugEntry(true, augmentations, viewport);
    CCountedPointer<NUISystem::CComponent> title(manager->CreateComponent("UI_TITLE_WEAPONS_NEW", false));
    title->SetName("Weapon Title");
    WeaponMenuItemMap.clear();
    NUISystem::CList* weaponsList = static_cast<NUISystem::CList*>(list);

    // melee entry
    CWideString meleeText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(miscDef->WeaponCatMeleeString.ToCharString());
    if (!meleeItems.empty()) {
        meleeText += CTCInventoryStats::GetXString();
        meleeText += GFIntToWideString(meleeItems.size());
    }
    CCountedPointer<NUISystem::CComponent> meleeEntry(manager->CreateMenuEntryComponent(meleeText, 0.0f, 1.0f,
        "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
    weaponsList->AddScrollingChild(meleeEntry);
    CCountedPointer<NUISystem::CComponent> description(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
    NUISystem::CText* descriptionText = static_cast<NUISystem::CText*>(description.Get());
    const NGameText::CDataBank& guideBank = CMainGameComponent::Get()->GetTextBank();
    descriptionText->SetValue(guideBank.GetTextBySymbol("TEXT_GUI_MENUGUIDE_WEAPONS_MELEE"), true, true);
    meleeEntry->AddChild(description);
    viewport->AddFakeChild(description.Get());
    CCountedPointer<NUISystem::CComponent> meleeSubtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    static_cast<NUISystem::CText*>(meleeSubtitle.Get())->SetValue(meleeText, true, true);
    meleeEntry->AddChild(meleeSubtitle);

    // ranged entry
    CWideString rangedText = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(miscDef->WeaponCatRangedString.ToCharString());
    if (!rangedItems.empty()) {
        rangedText += CTCInventoryStats::GetXString();
        rangedText += GFIntToWideString(rangedItems.size());
    }
    CCountedPointer<NUISystem::CComponent> rangedEntry(manager->CreateMenuEntryComponent(rangedText, 25.0f, 0.6666667f,
        "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
    weaponsList->AddScrollingChild(rangedEntry);
    description.Reset(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false));
    descriptionText = static_cast<NUISystem::CText*>(description.Get());
    descriptionText->SetValue(guideBank.GetTextBySymbol("TEXT_GUI_MENUGUIDE_WEAPONS_RANGED"), true, true);
    rangedEntry->AddChild(description);
    viewport->AddFakeChild(description.Get());
    CCountedPointer<NUISystem::CComponent> rangedSubtitle(manager->CreateComponent("UI_ONE_OFF_SUBTITLE", false));
    static_cast<NUISystem::CText*>(rangedSubtitle.Get())->SetValue(rangedText, true, true);
    rangedEntry->AddChild(rangedSubtitle);

    CCountedPointer<NUISystem::CComponent> meleeTitle(manager->CreateComponent("UI_TITLE_WEAPONS_NEW", false));
    NUISystem::CText* meleeTitleText = static_cast<NUISystem::CText*>(meleeTitle.Get());
    if (meleeTitleText)
        meleeTitleText->SetValue("TEXT_GUI_MENU_MELEE", true, true);
    CCountedPointer<NUISystem::CComponent> meleeItemTitle(manager->CreateComponent("UI_TITLE_WEAPONS_NEW", false));
    if (!meleeItemTitle.IsNull())
        static_cast<NUISystem::CText*>(meleeItemTitle.Get())->SetValue("TEXT_GUI_MENU_MELEE", true, true);
    title->SetName("Weapon Title");
    CCountedPointer<NUISystem::CComponent> back(manager->CreateComponent("UI_BACK_FOR_LIST", false));
    CCountedPointer<NUISystem::CComponent> select(manager->CreateComponent("UI_SELECT_FOR_LIST", false));
    CCountedPointer<NUISystem::CComponent> meleeRing(manager->CreateComponent("UI_RING_PIC_MELEE", false));
    meleeEntry->AddChild(meleeRing);
    CCountedPointer<NUISystem::CComponent> rangedRing(manager->CreateComponent("UI_RING_PIC_RANGED", false));
    rangedEntry->AddChild(rangedRing);

    // melee weapons: an unavailable entry without its arrow, or the melee sub-list
    if (meleeItems.empty()) {
        NUISystem::CComponent* arrow = meleeEntry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(deletion);
        }
        meleeEntry->MakeUnavailable();
    } else {
        CCountedPointer<NUISystem::CComponent> meleeList(manager->CreateComponent("UI_SUB_LIST", false));
        NUISystem::CList* subList = static_cast<NUISystem::CList*>(meleeList.Get());
        subList->AddNonScrollingChild(back);
        subList->AddNonScrollingChild(select);
        meleeEntry->AddChild(meleeList);
        subList->AddNonScrollingChild(meleeTitle);
        subList->InitialiseOffsets(meleeItems.size());
        float damage = 0.0f;
        if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
            const CTCWeapon* weapon;
            static_cast<CThing*>(PActiveMeleeWeapon.GetPItem())->PeekTC(&weapon);
            damage = weapon->GetDamage();
        }
        ConstructWeaponItemEntries(meleeItems, subList, augmentationEntry, parent, manager, viewport, miscDef, damage, meleeItemTitle);
    }

    CCountedPointer<NUISystem::CComponent> rangedTitle(manager->CreateComponent("UI_TITLE_WEAPONS_NEW", false));
    NUISystem::CText* rangedTitleText = static_cast<NUISystem::CText*>(rangedTitle.Get());
    if (rangedTitleText)
        rangedTitleText->SetValue("TEXT_GUI_MENU_RANGED", true, true);
    CCountedPointer<NUISystem::CComponent> rangedItemTitle(manager->CreateComponent("UI_TITLE_WEAPONS_NEW", false));
    NUISystem::CText* rangedItemTitleText = static_cast<NUISystem::CText*>(rangedItemTitle.Get());
    if (rangedItemTitleText)
        rangedItemTitleText->SetValue("TEXT_GUI_MENU_RANGED", true, true);

    // ranged weapons
    if (rangedItems.empty()) {
        NUISystem::CComponent* arrow = rangedEntry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(deletion);
        }
        rangedEntry->MakeUnavailable();
    } else {
        CCountedPointer<NUISystem::CComponent> rangedList(manager->CreateComponent("UI_SUB_LIST", false));
        NUISystem::CList* subList = static_cast<NUISystem::CList*>(rangedList.Get());
        subList->AddNonScrollingChild(back);
        subList->AddNonScrollingChild(select);
        rangedEntry->AddChild(rangedList);
        subList->AddNonScrollingChild(rangedTitle);
        float damage = 0.0f;
        if (IS_THING_ALIVE(PActiveRangedWeapon)) {
            CDefPointer<const CWeaponDef> weaponDef;
            static_cast<CThing*>(PActiveRangedWeapon.GetPItem())->GetPBaseDef()->GetSubDef(&weaponDef);
            CDefPointer<const CThingShotDef> shot(GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot));
            damage = shot->HitThingDamage;
        }
        subList->InitialiseOffsets(rangedItems.size());
        ConstructWeaponItemEntries(rangedItems, subList, augmentationEntry, parent, manager, viewport, miscDef, damage, rangedItemTitle);
    }
    WeaponsList = list;
    return list;
}