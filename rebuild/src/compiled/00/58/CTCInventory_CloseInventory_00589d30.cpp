// CTCInventory::CloseInventory() -- shut the inventory: close the base inventory view, leave quick-access assign / remove mode, release the primitive handles, delete the PC screens, clear the UI manager's observers, action params and bastard child and the lists' previous-selection stack, close any open sibling inventory screen, clear the menu maps, restart 3D scene rendering, restore audio volumes, play the exit sound and close the menus
// Leaf: ?CloseInventory@CTCInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00589D30, 919 bytes, thiscall, frame-pointer size TU (optimize("y",off) + ("s",on)).
//   Identity: Ego_r.pdb / FableWin.pdb ?CloseInventory@CTCInventory@@UAEXXZ; CTCInventory vtable 0x01250DE4 slot 31 (+0x7C) = 0x00589D30.
// NF62 (2026-11-17, round 7): whole-row port onto the shared headers (the local CTCInventory / NInventory::CTCInventoryBase / CTCBase /
//   NUISystem::CManager / CUserProfileManager / CMainGameComponent / CDisplayEngine / CWorld / CEnginePrimitiveHandle models and the
//   CHandleVector / CComponentVector / CTextInput / CInventoryCamera stand-ins are gone). Callees, retail -> shared spelling:
//   0x005BD597 NInventory::CTCInventoryBase::CloseInventory (qualified; was the stand-in ClearQuestPersistentFlags);
//   0x00581A14 SetQuickAccessAssign(false), EnableQuickAccessRemove +0x1CD stored in place; 0x0058771C ResetPrimitiveHandles;
//   std::for_each 0x0043F360 + std::mem_fun_ref(&CEnginePrimitiveHandle::Clear 0x0041BE70) over QuickAccessPrimitiveHandles +0x178
//   and QuickAccessUnusableIconPrimitiveHandles +0x184; vector<CComponent*>::erase 0x00485AA7; NUISystem::CManager slots 37
//   ClearBastardChild / 6 ClearObservers / 29 ClearActionParams; NUISystem::CList::GPreviousSelected (0x013B8A98) clear 0x004855A3;
//   CThing::GetTC (bitset test 0x00410DE0, LowerBound 0x0040F020) for the five inventory screens and their slot 31 CloseInventory
//   when IsInventoryOpen (+0x58); ItemsMenuEntries / AssignedIcons tree clear 0x00593235; CIEngine slot 76 Enable3DSceneRendering /
//   slot 56 CutscenePreload; CWorld::SetAsStarting3DRendering (inline GetFrame 0x0049D870 into +0x8C); CUserProfileManager::Get
//   0x0040D2A0 with the inline GetSoundVolume / GetDialogueVolume; PSoundManager SetGainFader (slot 46); NUISystem::CManager
//   GetMiscThingsDef (slot 42) + PlaySoundW (slot 40) with SoundExit +0x1A8; NUISystem::CManager::OnCloseMenus 0x0041D165.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_ISOPEN
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CBASE_ORIGINAL_MEMBERS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_ORIGINAL_COMPLETE_CLASS_PASS22
#define FABLE_RETAIL_CTCINVENTORY_IMPLICIT_GENERATED_SPECIALS_PASS22
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_CLEAR_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CMANAGER_MENUCOUNT
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_QUESTCOMPLETIONUI
#define FABLE_RETAIL_CUSERPROFILEMANAGER_VOLUME
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CWorld.h"
#include "engine/CUserProfileManager.h"
#include "engine/CASoundManager.h"

void CTCInventory::CloseInventory()
{
    NInventory::CTCInventoryBase::CloseInventory();
    SetQuickAccessAssign(false);
    EnableQuickAccessRemove = false;
    ResetPrimitiveHandles();
    std::for_each(QuickAccessPrimitiveHandles.begin(), QuickAccessPrimitiveHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    std::for_each(QuickAccessUnusableIconPrimitiveHandles.begin(), QuickAccessUnusableIconPrimitiveHandles.end(),
                  std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    NUISystem::CManager::GetInstance()->ClearBastardChild();
    NUISystem::CManager::GetInstance()->ClearObservers();
    NUISystem::CManager::GetInstance()->ClearActionParams();
    NUISystem::CList::GPreviousSelected.clear();
    {
    NInventory::CTCInventoryBase* weapons = 0;
    NInventory::CTCInventoryBase* map = 0;
    NInventory::CTCInventoryBase* abilities = 0;
    NInventory::CTCInventoryBase* stats = 0;
    NInventory::CTCInventoryBase* clothing = 0;
    ParentThing->GetTC(TCI_INVENTORY_WEAPONS, &weapons);
    NInventory::CTCInventoryBase* inventory;
    ParentThing->GetTC(TCI_INVENTORY, &inventory);
    ParentThing->GetTC(TCI_INVENTORY_MAP, &map);
    ParentThing->GetTC(TCI_HERO_ABILITIES, &abilities);
    ParentThing->GetTC(TCI_INVENTORY_STATS, &stats);
    ParentThing->GetTC(TCI_INVENTORY_CLOTHING, &clothing);
    if (weapons && weapons->IsInventoryOpen())
        weapons->CloseInventory();
    if (map && map->IsInventoryOpen())
        map->CloseInventory();
    if (abilities && abilities->IsInventoryOpen())
        abilities->CloseInventory();
    if (stats && stats->IsInventoryOpen())
        stats->CloseInventory();
    if (clothing && clothing->IsInventoryOpen())
        clothing->CloseInventory();
    }
    ItemsMenuEntries.clear();
    AssignedIcons.clear();
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(true);
    GetWorld().SetAsStarting3DRendering();
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        PSoundManager->SetGainFader(0x40, CUserProfileManager::Get().GetDialogueVolume(), 0.0f);
        PSoundManager->SetGainFader(0x80, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
    }
    NUISystem::CManager::GetInstance()->PlaySoundW(NUISystem::CManager::GetInstance()->GetMiscThingsDef()->SoundExit, 1);
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().CutscenePreload(false, false);
    NUISystem::CManager::OnCloseMenus();
}
