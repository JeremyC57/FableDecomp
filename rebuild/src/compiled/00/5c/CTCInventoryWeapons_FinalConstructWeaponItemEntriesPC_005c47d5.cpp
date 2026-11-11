// CTCInventoryWeapons::FinalConstructWeaponItemEntriesPC(std::vector<std::pair<long, long> >& items, NUISystem::CList* list, CCountedPointer<NUISystem::CComponent>& augmentationEntry, NUISystem::CComponent* parent, NUISystem::CManager* manager, NUISystem::CViewport* viewport, CDefPointer<const NUISystem::CUIMiscThingsDef>& miscDef, float damage, CCountedPointer<NUISystem::CComponent>& title, CCountedPointer<NUISystem::CComponent>& wieldEntry, unsigned long offset) -- PC twin of FinalConstructWeaponItemEntries: per weapon item it also rebuilds the wield entry for the weapon's kind and adds the parent's PC_UI_FRAME to the mesh, description and name actions
// Leaf: ?FinalConstructWeaponItemEntriesPC@CTCInventoryWeapons@@AAEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCList@NUISystem@@AAV?$CCountedPointer@VCComponent@NUISystem@@@@PAVCComponent@5@PAVCManager@5@PAVCViewport@5@AAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@M22K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C47D5, 1901 bytes, thiscall `ret 2Ch`, no EH frame; biased frame (`lea ebp,[esp-4Ch] ; sub esp,0BCh`):
//   optimize("s",on), as the console twin 0x005C3E91.
// Identity: Ego_r.pdb / FableWin.pdb ?FinalConstructWeaponItemEntriesPC@CTCInventoryWeapons@@AAE... (private; Ego_r donor 0x0062814E,
//   FableWin 0x023956BE); caller: the landed ConstructWeaponItemEntriesPC 0x005C8176 (two calls, the second with the entry offset).
// Callees: as 0x005C3E91 (RefreshActiveWeaponData 0x005C30D3, the expanded PeekTC<CTCHero>, CreateMenuEntryComponent 0x00428494 /
//   CreateComponent 0x0041DB1D, IS_THING_ALIVE 0x004CC340, GetPItem 0x00A01B50, CTCInventoryItem::GetEngineGraphic(long) 0x005D96A0,
//   the CAction / CActionParam* builders, CAction::AddParam(CActionParam*, CCountedPointer<CAction>&) 0x0053FDE0) plus
//   CTCBase::GetDefinitionManager 0x006869D0, GetDef<CThingBaseDef> 0x0042AF3C, GetSubDef<CWeaponDef> 0x0042B1A6,
//   CTCWeapon::IsMeleeWeaponClass 0x005DA510, ConstructWieldEntryPC 0x005C3444, CCountedPointer<CComponent>::operator= 0x00429C58;
//   virtual CComponent slot 3 FindChild(CCharString) +0x0C, and the CList / CSprite / CMenuEntry slots of 0x005C3E91.
// Data: "PC_UI_FRAME" 0x0124EFA8, "PC_MENU_ENTRY_BRANCH_TEMPLATE" 0x01236EE4, L"PC_MODEL" 0x0125012C, L"INVALID NAME" 0x0123029C.
// Shape: the def lookups are the same-TU template bodies; the wield entry is rebuilt per item in an if/else of two calls (retail pushes the
//   shared arguments once, then `push 1` / `push edi`); the parent's PC_UI_FRAME is looked up once before the loop (for the wield entry)
//   and again per item (the frame parameter of the mesh, description and name actions); otherwise the console body (implicit name /
//   template conversions, def index compared on the left, non-POD CEngineGraphic). VC7.1 keeps the def index in the dead offset argument
//   slot and offset + i in a local.
// Name fix (agentNF7 2026-11-10): 0x0042AF3C is CDefinitionManager::GetDef<CThingBaseDef> (FableWin.pdb; NF5/NF6 STATUS): the out handle retyped CDefPointer<const CThingBaseDef> (shared engine/CThingBaseDef.h), so the template binds that instantiation.
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
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_DAMAGE
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA_DECL_ONLY
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_STATICENGINEGRAPHIC
#define FABLE_RETAIL_CENGINEGRAPHIC_CTOR
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CMENUENTRY_ACTIONLISTS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMEBOOL
#define FABLE_RETAIL_CACTIONPARAMCCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCHero.h"
#include "engine/CTCWeapon.h"
#include "engine/CWeaponDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CActionParamString.h"
#include "engine/NUISystem_CActionParamEBOOL.h"
#include "engine/NUISystem_CActionParamCCountedPointer.h"
#include "engine/CThingBaseDef.h"


template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x004CC340 (CIntelligentPointer<CThing>)

// Same-TU bodies of the def lookups retail's TU holds (out of line in retail: GetDef<CThingBaseDef> 0x0042AF3C, GetSubDef<CWeaponDef>
// 0x0042B1A6): VC7.1 sees that they only write through the out pointer, so the CDefPointer locals stay unaliased.
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

void CTCInventoryWeapons::FinalConstructWeaponItemEntriesPC(std::vector<std::pair<long, long> >& items, NUISystem::CList* list,
    CCountedPointer<NUISystem::CComponent>& augmentationEntry, NUISystem::CComponent* parent, NUISystem::CManager* manager,
    NUISystem::CViewport* viewport, CDefPointer<const NUISystem::CUIMiscThingsDef>& miscDef, float damage,
    CCountedPointer<NUISystem::CComponent>& title, CCountedPointer<NUISystem::CComponent>& wieldEntry, unsigned long offset)
{
    RefreshActiveWeaponData();
    NUISystem::CComponent* frame = parent->FindChild("PC_UI_FRAME");
    bool weaponsUsable = true;
    const CTCHero* hero;
    if (ParentThing->PeekTC(&hero) && !hero->WeaponsUsable)
        weaponsUsable = false;
    for (unsigned long i = 0; i < items.size(); ++i) {
        long defIndex = items[i].first;
        CDefPointer<const CThingBaseDef> def;
        GetDefinitionManager().GetDef(defIndex, &def);
        CDefPointer<const CWeaponDef> weaponDef;
        def->GetSubDef(&weaponDef);
        // the wield entry of this weapon's kind
        if (CTCWeapon::IsMeleeWeaponClass(weaponDef->Class))
            wieldEntry = ConstructWieldEntryPC(true, viewport, frame, defIndex);
        else
            wieldEntry = ConstructWieldEntryPC(false, viewport, frame, defIndex);
        CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f,
            "PC_MENU_ENTRY_BRANCH_TEMPLATE", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
        // the entry of the wielded weapon becomes its list's default, with the assigned-weapon icon
        if (IS_THING_ALIVE(PActiveMeleeWeapon) && defIndex == PActiveMeleeWeapon.Get()->GetDefGlobalIndex()) {
            list->SetDefaultChild(offset + i);
            PAssignedMeleeMenuEntry = entry.Get();
            PMeleeList = list;
            if (CTCInventoryItem::GetEngineGraphic(defIndex).BankIndex != 0) {
                CCountedPointer<NUISystem::CComponent> icon(manager->CreateComponent("UI_ASSIGNED_WEAPON_ICON", false));
                static_cast<NUISystem::CSprite*>(icon.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(defIndex).BankIndex);
                entry->AddChild(icon);
                PAssignedMeleeIcon = icon.Get();
            }
        } else if (IS_THING_ALIVE(PActiveRangedWeapon) && defIndex == PActiveRangedWeapon.Get()->GetDefGlobalIndex()) {
            PAssignedRangedMenuEntry = entry.Get();
            PRangedList = list;
            list->SetDefaultChild(offset + i);
            if (CTCInventoryItem::GetEngineGraphic(defIndex).BankIndex != 0) {
                CCountedPointer<NUISystem::CComponent> icon(manager->CreateComponent("UI_ASSIGNED_WEAPON_ICON", false));
                static_cast<NUISystem::CSprite*>(icon.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(defIndex).BankIndex);
                entry->AddChild(icon);
                PAssignedRangedIcon = icon.Get();
            }
        }
        NUISystem::CMenuEntry* menuEntry = static_cast<NUISystem::CMenuEntry*>(entry.Get());
        NUISystem::CComponent* uiFrame = parent->FindChild("PC_UI_FRAME");
        // selected: show the weapon's mesh
        CCountedPointer<NUISystem::CAction> addMesh(new NUISystem::CAction);
        addMesh->ActionType = NUISystem::UI_ACTION_ADD_MESH_CHILD;
        addMesh->UIActionParams.push_back(new NUISystem::CActionParamSLONG(defIndex));
        addMesh->AddParam(entry.Get(), addMesh);
        CWideString modelName(L"PC_MODEL");
        addMesh->UIActionParams.push_back(new NUISystem::CActionParamString(modelName));
        addMesh->AddParam(uiFrame, addMesh);
        entry->AddActionOnSelected(addMesh);
        // unselected: remove it
        CCountedPointer<NUISystem::CAction> removeMesh(new NUISystem::CAction);
        removeMesh->ActionType = NUISystem::UI_ACTION_REMOVE_MESH_CHILD;
        removeMesh->AddParam(entry.Get(), removeMesh);
        menuEntry->AddActionOnUnselected(removeMesh);
        // selected: the weapon's description in the viewport
        CCountedPointer<NUISystem::CAction> loadDescription(new NUISystem::CAction);
        loadDescription->ActionType = NUISystem::UI_ACTION_LOAD_WEAPON_DESC;
        loadDescription->UIActionParams.push_back(new NUISystem::CActionParamSLONG(defIndex));
        loadDescription->AddParam(viewport, loadDescription);
        loadDescription->AddParam(entry.Get(), loadDescription);
        loadDescription->AddParam(uiFrame, loadDescription);
        entry->AddActionOnSelected(loadDescription);
        CCountedPointer<NUISystem::CAction> unloadDescription(new NUISystem::CAction);
        unloadDescription->ActionType = NUISystem::UI_ACTION_UNLOAD_WEAPON_DESC;
        unloadDescription->AddParam(entry.Get(), unloadDescription);
        menuEntry->AddActionOnUnselected(unloadDescription);
        // visible: the entry's name (with the title, augmentation and wield entries)
        CCountedPointer<NUISystem::CAction> loadName(new NUISystem::CAction);
        loadName->ActionType = NUISystem::UI_ACTION_LOAD_WEAPON_MENU_ENTRY_NAME;
        loadName->UIActionParams.push_back(new NUISystem::CActionParamSLONG(defIndex));
        loadName->AddParam(entry.Get(), loadName);
        loadName->UIActionParams.push_back(new NUISystem::CActionParamCCountedPointer<NUISystem::CComponent>(title));
        loadName->UIActionParams.push_back(new NUISystem::CActionParamCCountedPointer<NUISystem::CComponent>(augmentationEntry));
        loadName->UIActionParams.push_back(new NUISystem::CActionParamEBOOL(weaponsUsable));
        loadName->UIActionParams.push_back(new NUISystem::CActionParamCCountedPointer<NUISystem::CComponent>(wieldEntry));
        loadName->AddParam(uiFrame, loadName);
        menuEntry->AddActionOnVisible(loadName);
        list->AddScrollingChildAndSetOffsets(entry);
        menuEntry->AddThingActionParam(defIndex);
        menuEntry->AddUIActionParam(entry.Get());
    }
}