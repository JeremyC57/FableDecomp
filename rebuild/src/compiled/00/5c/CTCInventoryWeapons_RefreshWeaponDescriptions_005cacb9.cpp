// CTCInventoryWeapons::RefreshWeaponDescriptions() -- re-texts the weapons screen: the augmentation entry's "Augment x<count>" title, then every weapon description of the weapons list is replaced by a fresh description container rebuilt against the damage of the active weapon of its kind (melee / ranged)
// Leaf: ?RefreshWeaponDescriptions@CTCInventoryWeapons@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CACB9, 1135 bytes, thiscall `leave ; ret`, frame 0x44; optimize("s",on).
// Identity: Ego_r.pdb / FableWin.pdb ?RefreshWeaponDescriptions@CTCInventoryWeapons@@QAEXXZ (Ego_r donor 0x006299F1, after
//   CloseInventory 0x005CAC8D); the weapons twin of CTCInventoryClothing::RefreshDescriptions 0x005B690B (landed).
// Callees: GetDef<CUIMiscThingsDef> 0x0042B312, list<CComponent*>() 0x0042ABAF / ~ 0x0042ABCA, CComponent::FindChild (slot 3),
//   FindAllChildren (slot 5), FindChildNonRecursive (slot 8), SetDeletion (62), GetParent (52), AddChildDynamically (60), SetName (83),
//   GetParentViewport (99), CViewport::AddFakeChild (142), CText::SetValue (144), CTCInventoryStats::GetXString 0x005CC772,
//   CManager::GetNumberAugmentations (slot 31), IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340, CTCWeapon::GetDamage 0x005DAAA0,
//   map<CComponent*,long>::operator[] 0x0042D1B7 / _Rb_tree::erase(const key&) 0x005CC31B, CDeletion() 0x0042BED4 / (const&) 0x0042CD84,
//   ConstructWeaponDescriptionContainer 0x005C90FC, SetWeaponMappingToMenuEntry 0x0042D405, CCountedPointer Reset 0x00429C6F /
//   ReleaseData 0x004291DE.
// Data: the TU's anonymous-namespace WEAPON_DESCRIPTION_STRING 0x013B8BAC and AUGMENTATIONS_MENU_ENTRY_NAME 0x013B8BC0 (Ego_r.pdb
//   initializer order NEWLINE, WEAPON_DESCRIPTION_STRING, AUG_DESCRIPTION_STRING, WEAPON_TITLE_STRING, .._M, .._R,
//   AUGMENTATIONS_MENU_ENTRY_NAME over 0x013B8BA8..0x013B8BC0); "TEXT_GUI_MENU_AUGMENT" 0x012544B0, "PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE"
//   0x01230B5C, "MENU_ENTRY_TEXT" 0x01230B80, "PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE" 0x01230528,
//   "UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE_ONE_OFF" 0x012304F8.
// Shape: the active-weapon damage blocks as written out in ConstructScreen 0x005C8459 (the melee weapon pointer initialised: its
//   word doubles as the not-found result, `and [ebp-28h],0`, 0x005CAE47); the description key is a named CComponent* local with
//   FABLE_RETAIL_STLP_MAP / _TREE_ERASE_KEY bodies visible (both still called); the viewport test is `if (GetParentViewport())`
//   around a second fetch tested again (`test eax,eax` then `cmp eax,ebx`, 0x005CB054..0x005CB064; the ternary form keeps a
//   null arm); the container is created in an if/else over IsLiveGUIMode() (two CreateComponent calls, as 0x005C90FC);
//   FABLE_RETAIL_CDELETION_BODIES schedules the Method store right before the copy constructor (0x005CAFA2).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_POINTEE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CDELETION_BODIES
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_DAMAGE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_LIVEGUI
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCWeapon.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPlayerGui.h"
#include "engine/CWeaponDef.h"
#include "engine/CThingShotDef.h"

CWideString FABLE_FASTCALL GFIntToWideString(long value);                       // 0x00405F00
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x004CC340

namespace {
extern const CCharString WEAPON_DESCRIPTION_STRING;                            // 0x013B8BAC
extern const CCharString AUGMENTATIONS_MENU_ENTRY_NAME;                        // 0x013B8BC0 "Augmentations_Menu_Entry_Name"
}

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

void CTCInventoryWeapons::RefreshWeaponDescriptions()
{
    CDefPointer<const NUISystem::CUIMiscThingsDef> miscDef;
    long miscIndex = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(miscIndex, &miscDef);
    std::list<NUISystem::CComponent*> descriptions;
    if (WeaponsList) {
        // the augmentation entry's text: "Augment" x <number of augmentations>
        NUISystem::CComponent* augmentationEntry = WeaponsList->FindChild(AUGMENTATIONS_MENU_ENTRY_NAME);
        if (augmentationEntry) {
            NUISystem::CComponent* entryText = augmentationEntry->FindChildNonRecursive("PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE")->FindChildNonRecursive("MENU_ENTRY_TEXT");
            CWideString title = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_AUGMENT") + CTCInventoryStats::GetXString();
            title += GFIntToWideString(NUISystem::CManager::GetInstance()->GetNumberAugmentations());
            static_cast<NUISystem::CText*>(entryText)->SetValue(title, true, true);
        }
        // every weapon description: rebuilt against the damage of the active weapon of its kind
        WeaponsList->FindAllChildren(WEAPON_DESCRIPTION_STRING, descriptions);
        float meleeDamage = 0.0f;
        float rangedDamage = 0.0f;
        if (IS_THING_ALIVE(PActiveMeleeWeapon)) {
            const CTCWeapon* weapon = 0;
            static_cast<CThing*>(PActiveMeleeWeapon.GetPItem())->PeekTC(&weapon);
            meleeDamage = weapon->GetDamage();
        }
        if (IS_THING_ALIVE(PActiveRangedWeapon)) {
            CDefPointer<const CWeaponDef> weaponDef;
            static_cast<CThing*>(PActiveRangedWeapon.GetPItem())->GetPBaseDef()->GetSubDef(&weaponDef);
            CDefPointer<const CThingShotDef> shot(GetDefinitionManager().GetPDefFromGlobalIndex(weaponDef->Shot));
            rangedDamage = shot->HitThingDamage;
        }
        std::list<NUISystem::CComponent*>::iterator it;
        std::list<NUISystem::CComponent*>::iterator end;
        for (it = descriptions.begin(), end = descriptions.end(); it != end; ++it) {
            NUISystem::CComponent* description = *it;
            long itemDefIndex = WeaponMenuItemMap[description];
            if (itemDefIndex > 0) {
                CDefPointer<const CDefClassBase> def;
                GetDefinitionManager().GetDef(itemDefIndex, &def);
                CDefPointer<const CWeaponDef> weaponDef;
                def->GetSubDef(&weaponDef);
                // the old description goes; a new container takes its place
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                description->SetDeletion(deletion);
                WeaponMenuItemMap.erase(description);
                CCountedPointer<NUISystem::CComponent> container;
                if (CTBaseSingletonNoDefaultAlloc<CPlayerGui>::Get().IsLiveGUIMode())
                    container.Reset(NUISystem::CManager::GetInstance()->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE", false));
                else
                    container.Reset(NUISystem::CManager::GetInstance()->CreateComponent("UI_TEXT_CONTAINER_DESCRIPTION_TEMPLATE_ONE_OFF", false));
                container->SetName(WEAPON_DESCRIPTION_STRING);
                if (description->GetParentViewport()) {
                    NUISystem::CViewport* viewport = static_cast<NUISystem::CViewport*>(description->GetParentViewport());
                    if (viewport)
                        viewport->AddFakeChild(container.Get());
                }
                ConstructWeaponDescriptionContainer(container.Get(), itemDefIndex, CTCWeapon::IsMeleeWeaponClass(weaponDef->Class) ? meleeDamage : rangedDamage);
                if (description->GetParent())
                    description->GetParent()->AddChildDynamically(container);
                SetWeaponMappingToMenuEntry(container.Get(), itemDefIndex);
            }
        }
    }
}