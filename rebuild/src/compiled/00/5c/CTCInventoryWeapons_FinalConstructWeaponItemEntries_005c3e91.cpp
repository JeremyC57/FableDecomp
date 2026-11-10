// CTCInventoryWeapons::FinalConstructWeaponItemEntries(std::vector<std::pair<long, long> >& items, NUISystem::CList* list, CCountedPointer<NUISystem::CComponent>& augmentationEntry, NUISystem::CComponent* parent, NUISystem::CManager* manager, NUISystem::CViewport* viewport, CDefPointer<const NUISystem::CUIMiscThingsDef>& miscDef, float damage, CCountedPointer<NUISystem::CComponent>& title, CCountedPointer<NUISystem::CComponent>& wieldEntry, unsigned long offset) -- adds one console menu entry per weapon item to the list: marks the active melee / ranged weapon's entry as the list default (with the assigned-weapon icon), and gives each entry its mesh, description and name actions
// Leaf: ?FinalConstructWeaponItemEntries@CTCInventoryWeapons@@AAEXAAV?$vector@U?$pair@JJ@std@@V?$allocator@U?$pair@JJ@std@@@2@@std@@PAVCList@NUISystem@@AAV?$CCountedPointer@VCComponent@NUISystem@@@@PAVCComponent@5@PAVCManager@5@PAVCViewport@5@AAV?$CDefPointer@$$CBVCUIMiscThingsDef@NUISystem@@@@M22K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C3E91, 1641 bytes, thiscall `ret 2Ch`, no EH frame; plain ebp frame (`push ebp ; mov ebp,esp ; sub esp,98h`): optimize("s",on) alone (the PC twin 0x005C47D5 gets the biased frame from the same pragma).
// Identity: Ego_r.pdb / FableWin.pdb ?FinalConstructWeaponItemEntries@CTCInventoryWeapons@@AAE... (private; Ego_r donor 0x00625B29,
//   FableWin 0x02381802); callers: the landed ConstructWeaponItemEntries 0x005C66B2 (two calls, the second with the entry offset).
// Callees: RefreshActiveWeaponData 0x005C30D3, the expanded CThing::PeekTC<CTCHero> (bitset test 0x00410DE0, const LowerBound 0x004365B0),
//   CManager::CreateMenuEntryComponent 0x00428494 / CreateComponent 0x0041DB1D, CCountedPointer<CComponent>(T*) 0x00429C15 / ReleaseData
//   0x004291DE, IS_THING_ALIVE 0x004CC340, CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCInventoryItem::GetEngineGraphic(long)
//   0x005D96A0 (static __fastcall, by value), CAction() 0x0042BE50 + CCountedPointer<CAction>(T*) 0x0042AA29 / ReleaseData 0x00429160,
//   operator new 0x00BFEA1A, list<CActionParam*>::push_back 0x0042B646, CAction::AddParam(CActionParam*, CCountedPointer<CAction>&)
//   0x0053FDE0, CActionParamString(CWideString&) 0x0054DAE0, CActionParamCCountedPointer<CComponent>(CCountedPointer<CComponent>&)
//   0x004854DF; inline CActionParamSLONG (vtable 0x012366F4) / CActionParamEBOOL (0x012366FC). Virtual slots: CList 158
//   SetDefaultChild(long) +0x278, 152 AddScrollingChildAndSetOffsets +0x260; CSprite 145 SetGraphicIndex +0x244; CComponent 59 AddChild
//   +0xEC, 67 AddActionOnSelected +0x10C; CMenuEntry 150 AddThingActionParam +0x258, 151 AddUIActionParam +0x25C, 152 AddActionOnUnselected
//   +0x260, 153 AddActionOnVisible +0x264 (vtable 0x0124662C).
// Data: L"INVALID NAME" 0x0123029C, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW" 0x01237208, "PC_BUTTON_BASTARD" 0x01236ED0, "" 0x0122D70E,
//   "UI_ASSIGNED_WEAPON_ICON" 0x01254268, L"UI_WEAPON_MODEL_NEW" 0x01254240.
// Shape: the entry name and template arguments are implicit conversions (VC7.1 builds both reference temporaries before the by-value
//   pushes, retail 0x005C3F1A..0x005C3F31; an explicit CWideString(...) is built in the push sequence instead); the def index is compared
//   on the left (`cmp edi,eax`, 0x005C3FBA); the melee half sets the list default before storing the assigned entry / list, the ranged
//   half after (0x005C3FCC v 0x005C40A3); the static GetEngineGraphic returns a non-POD CEngineGraphic (FABLE_RETAIL_CENGINEGRAPHIC_CTOR:
//   hidden result in ecx). VC7.1 hoists the viewport's CActionParam (null-guarded) out of the loop and keeps offset + i in the dead
//   viewport argument slot. miscDef, damage and parent are unused (as in retail).
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

template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& thing);          // 0x004CC340 (CIntelligentPointer<CThing>)

void CTCInventoryWeapons::FinalConstructWeaponItemEntries(std::vector<std::pair<long, long> >& items, NUISystem::CList* list,
    CCountedPointer<NUISystem::CComponent>& augmentationEntry, NUISystem::CComponent* parent, NUISystem::CManager* manager,
    NUISystem::CViewport* viewport, CDefPointer<const NUISystem::CUIMiscThingsDef>& miscDef, float damage,
    CCountedPointer<NUISystem::CComponent>& title, CCountedPointer<NUISystem::CComponent>& wieldEntry, unsigned long offset)
{
    RefreshActiveWeaponData();
    bool weaponsUsable = true;
    const CTCHero* hero;
    if (ParentThing->PeekTC(&hero) && !hero->WeaponsUsable)
        weaponsUsable = false;
    for (unsigned long i = 0; i < items.size(); ++i) {
        long defIndex = items[i].first;
        CCountedPointer<NUISystem::CComponent> entry(manager->CreateMenuEntryComponent(L"INVALID NAME", 0.0f, 1.0f,
            "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f, CCharString(""), 0.0f, CCharString("PC_BUTTON_BASTARD")));
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
        // selected: show the weapon's mesh
        CCountedPointer<NUISystem::CAction> addMesh(new NUISystem::CAction);
        addMesh->ActionType = NUISystem::UI_ACTION_ADD_MESH_CHILD;
        addMesh->UIActionParams.push_back(new NUISystem::CActionParamSLONG(defIndex));
        addMesh->AddParam(entry.Get(), addMesh);
        CWideString modelName(L"UI_WEAPON_MODEL_NEW");
        addMesh->UIActionParams.push_back(new NUISystem::CActionParamString(modelName));
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
        menuEntry->AddActionOnVisible(loadName);
        list->AddScrollingChildAndSetOffsets(entry);
        menuEntry->AddThingActionParam(defIndex);
        menuEntry->AddUIActionParam(entry.Get());
    }
}