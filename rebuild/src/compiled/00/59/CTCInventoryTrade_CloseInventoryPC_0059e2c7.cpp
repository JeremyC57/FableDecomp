// CTCInventoryTrade::CloseInventoryPC() -- closes the PC trade screen: resets the trade lists, closes the base view, deletes the trade components and every UI component, clears the buy / sell maps, clears the UI manager's observers, action params, previous-selection stack and bastard child, restarts 3D scene rendering, restores the sound groups, plays the exit sound checks the weapons screen for sold wielded weapons and clears the open / initialised flags
// Leaf: ?CloseInventoryPC@CTCInventoryTrade@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0059E2C7, plain `ret`. Frame-pointer size TU: optimize("y",off) + ("s",on).
//   Identity: Ego_r.pdb / FableWin.pdb ?CloseInventoryPC@CTCInventoryTrade@@UAEXXZ; CTCInventoryTrade vtable 0x01253034 slot 87 (+0x15C) = 0x0059E2C7.
// NF62 (2026-11-17, round 7): whole-row port onto the shared headers (the local CTCInventoryTrade / NInventory::CTCInventoryBase / CTCBase /
//   CThing / NUISystem::CManager / CPlayerGui / CUserProfileManager / CMainGameComponent / CWorld models and the CComponentVector /
//   CComponentPairMap / node-list / player-pointer stand-ins are gone). Callees, retail -> shared spelling:
//   ResetTradeLists 0x0059D9AD; NInventory::CTCInventoryBase::CloseInventory 0x005BD597 (qualified;
//   was the stand-in BaseCloseInventory); DeleteComponents 0x0059C8AE; vector<CComponent*>::erase 0x00485AA7; ItemsToBuy / ItemsToSell tree clear 0x005B1C18;
//   NUISystem::CManager slots 6 ClearObservers / 29 ClearActionParams / 37 ClearBastardChild around the
//   NUISystem::CList::GPreviousSelected (0x013B8A98) clear 0x004855A3; CIEngine slot 76 Enable3DSceneRendering / slot 56 CutscenePreload;
//   CWorld::SetAsStarting3DRendering (inline: CTCBase::GetWorld 0x00686A70, CWorld::GetFrame 0x0049D870); CUserProfileManager::Get
//   0x0040D2A0 + inline GetSoundVolume / GetDialogueVolume; PSoundManager SetGainFader (slot 46); GetMiscThingsDef (slot 42) +
//   PlaySoundW (slot 40) with SoundExit +0x1A8; CThing::GetTC (0x00410DE0 / 0x0040F020) and CTCInventoryWeapons::CheckForSoldWieldedWeapons
//   0x005CB780; InventoryTradeOpen +0x218 / InventoryTradeInitialised +0x219 cleared.
// Header: overlay opt-in FABLE_RETAIL_CTCINVENTORYTRADE_CLOSEINVENTORY (the two virtual declarations).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYTRADE
#define FABLE_RETAIL_CTCINVENTORYTRADE_CLOSEINVENTORY
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_METHODS
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
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryTrade.h"
#include "engine/CTCInventoryWeapons.h"
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

void CTCInventoryTrade::CloseInventoryPC()
{
    ResetTradeLists();
    NInventory::CTCInventoryBase::CloseInventory();
    DeleteComponents();
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    ItemsToBuy.clear();
    ItemsToSell.clear();
    NUISystem::CManager::GetInstance()->ClearObservers();
    NUISystem::CManager::GetInstance()->ClearActionParams();
    NUISystem::CList::GPreviousSelected.clear();
    NUISystem::CManager::GetInstance()->ClearBastardChild();
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(true);
    GetWorld().SetAsStarting3DRendering();
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        PSoundManager->SetGainFader(0x40, CUserProfileManager::Get().GetDialogueVolume(), 0.0f);
        PSoundManager->SetGainFader(0x80, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
    }
    NUISystem::CManager::GetInstance()->PlaySoundW(NUISystem::CManager::GetInstance()->GetMiscThingsDef()->SoundExit, 1);
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().CutscenePreload(false, false);
    CTCInventoryWeapons* weapons;
    if (ParentThing->GetTC(TCI_INVENTORY_WEAPONS, &weapons))
        weapons->CheckForSoldWieldedWeapons();
    InventoryTradeOpen = false;
    InventoryTradeInitialised = false;
}
