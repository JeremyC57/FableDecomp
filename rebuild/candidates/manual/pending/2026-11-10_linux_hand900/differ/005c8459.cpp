// CTCInventoryWeapons::ConstructScreen(NUISystem::CComponent* parent) -- builds the console weapons screen: sorts the hero's items into melee / ranged weapons and augmentations, then the UI_LIST_WEAPONS list with the augmentation entry, a melee and a ranged menu entry (guide text, subtitle, ring picture) and, per non-empty category, a sub-list filled by ConstructWeaponItemEntries
// Grade: DIFFER(3299v3235) -- masked-frame instruction share 0.865 (1088 v 1066 instructions). Residues: (1) every CDefPointer release in the item loop and the final miscDef release comes out as `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],edi` plus a kept `Object = 0` store where retail has `dec dword ptr [ecx+4] ; jne` (engine/CDefPointer.h Release shape in this TU); (2) the frame is 0xE4 v 0xE0 and the slot map differs; (3) the second title SetValue(CCharString) loads the vtable before building the string; (4) `augmentations.size()` test folds to `test ecx,mask` where retail keeps `sar eax,3 ; je`.
// Leaf: ?ConstructScreen@CTCInventoryWeapons@@UAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C8459, 3235 bytes, thiscall `ret 4`, no EH frame. Biased frame (`push ebp ; lea ebp,[esp-74h] ;
//   sub esp,0E0h`): optimize("s",on) alone (as the landed CTCInventoryAbilities::ConstructScreen 0x005F519F).
// Identity: slot 77 of the CTCInventoryWeapons vtable 0x01253FF4 (cell 0x01254128); Ego_r.pdb / FableWin.pdb
//   ?ConstructScreen@CTCInventoryWeapons@@UAEPAVCComponent@NUISystem@@PAV23@@Z; the console twin of ConstructScreenPC 0x005C9A95 (slot 87).
// Callees: CGameDefinitionManager::Get 0x0044C6B0, GetGlobalIndexFromClassIndex 0x009AD390, GetDef<CUIMiscThingsDef> 0x0042B312,
//   list<CComponent*>::clear 0x0042A18F, GetAllItems 0x005BF482, CTCBase::GetDefinitionManager 0x006869D0, GetDef 0x0042AF3C,
//   GetSubDef<CWeaponDef> 0x0042B1A6, CTCWeapon::IsMeleeWeaponClass 0x005DA510, CTCInventoryItem::IsAugmentation 0x005D9870,
//   vector<pair<long,long> >::push_back 0x005947B6, CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D /
//   CreateMenuEntryComponent 0x00428494 / SetNumberAugmentations (slot 30), ConstructAugEntry 0x005C574E, map clear 0x004DB806,
//   CTCInventoryStats::GetXString 0x005CC772, GFIntToWideString 0x00405F00, CDeletion 0x0042BED4 / 0x0042CD84,
//   IS_THING_ALIVE 0x004CC340, GetPItem 0x00A01B50, CTCWeapon::GetDamage 0x005DAAA0, GetPDefFromGlobalIndex 0x009AD9E0,
//   ConstructWeaponItemEntries 0x005C66B2.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
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

// The weapon def as these screens read it (no shared header yet): Ego_r.pdb CWeaponDef Class +0x2c; +0xa0 is the def index of the
// projectile the weapon fires (its def's +0x54 is the damage the ranged menu shows). Unproven member names: see NOTES.md.
class CWeaponDef : public CDefClassBase {
public:
    long Type;                                                                  // +0x28 EWeaponType
    EWeaponClass Class;                                                         // +0x2c
    unsigned char _pad30[0xa0 - 0x30];
    long Shot;                                                                  // +0xa0 (unproven name)
};
class CUnprovenShotDef_005C8459 : public CDefClassBase {
public:
    unsigned char _pad25[0x54 - 0x25];
    float Damage;                                                               // +0x54 (unproven name)
};

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
    if (augmentations.size())
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
    description = manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF", false);
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
        meleeTitleText->SetValue(CCharString("TEXT_GUI_MENU_MELEE"), true, true);
    CCountedPointer<NUISystem::CComponent> meleeItemTitle(manager->CreateComponent("UI_TITLE_WEAPONS_NEW", false));
    if (!meleeItemTitle.IsNull())
        static_cast<NUISystem::CText*>(meleeItemTitle.Get())->SetValue(CCharString("TEXT_GUI_MENU_MELEE"), true, true);
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
            CTCWeapon* weapon;
            static_cast<CThing*>(PActiveMeleeWeapon.GetPItem())->GetTC(&weapon);
            damage = weapon->GetDamage();
        }
        ConstructWeaponItemEntries(meleeItems, subList, augmentationEntry, parent, manager, viewport, miscDef, damage, meleeItemTitle);
    }

    CCountedPointer<NUISystem::CComponent> rangedTitle(manager->CreateComponent("UI_TITLE_WEAPONS_NEW", false));
    NUISystem::CText* rangedTitleText = static_cast<NUISystem::CText*>(rangedTitle.Get());
    if (rangedTitleText)
        rangedTitleText->SetValue(CCharString("TEXT_GUI_MENU_RANGED"), true, true);
    CCountedPointer<NUISystem::CComponent> rangedItemTitle(manager->CreateComponent("UI_TITLE_WEAPONS_NEW", false));
    if (!rangedItemTitle.IsNull())
        static_cast<NUISystem::CText*>(rangedItemTitle.Get())->SetValue(CCharString("TEXT_GUI_MENU_RANGED"), true, true);

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
            CDefPointer<const CUnprovenShotDef_005C8459> shot(GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot));
            damage = shot->Damage;
        }
        subList->InitialiseOffsets(rangedItems.size());
        ConstructWeaponItemEntries(rangedItems, subList, augmentationEntry, parent, manager, viewport, miscDef, damage, rangedItemTitle);
    }
    WeaponsList = list;
    return list;
}
