// CTCInventoryMap::CloseInventory() -- closes the map screen: forgets the zooming map and the map list; unless it is the inventory screen, restores the sound groups, plays the exit sound, restarts 3D scene rendering, returns the hero-info manager to the previous screen type, closes the base view, deletes every UI component, clears the UI manager's observers / action params / bastard child and the lists' previous-selection stack, stops the cutscene preload and closes the menus; then marks it the inventory screen, resets its primitive handles and clears the open and pan flags
// Leaf: ?CloseInventory@CTCInventoryMap@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FCE60, 584 bytes, plain `ret`. Speed TU (no pragmas, no frame pointer).
//   Identity: Ego_r.pdb 0x0064F970 / FableWin.pdb 0x0208DC40 ?CloseInventory@CTCInventoryMap@@UAEXXZ; CTCInventoryMap vtable slot 31 (0x01257B18) = 0x005FCE60.
// NF62 (2026-11-17, round 7): whole-row port onto the shared headers (the local CTCInventoryMap / NInventory::CTCInventoryBase / CTCBase /
//   NHeroInformationScreens::CBase / CManager / NUISystem::CManager / CPlayerGui / CUserProfileManager / CMainGameComponent / CWorld models and
//   the CComponentVector / node-list / player-pointer stand-ins are gone). Callees, retail -> shared spelling: IsInventoryScreen (the
//   NHeroInformationScreens::CBase slot 3 at +0x130); CUserProfileManager::Get 0x0040D2A0 + inline GetSoundVolume / GetDialogueVolume;
//   PSoundManager SetGainFader (slot 46); NUISystem::CManager GetMiscThingsDef (slot 42) + PlaySoundW (slot 40) with SoundExit +0x1A8;
//   CIEngine slot 76 Enable3DSceneRendering / slot 56 CutscenePreload; CWorld::SetAsStarting3DRendering (inline: CTCBase::GetWorld
//   0x00686A70, CWorld::GetFrame 0x0049D870); CPlayerGui::Get (PClass 0x013B8790) -> GetPHeroInfoScreensManager 0x00437CD0 ->
//   SelectScreenOfType 0x0066E799 (PreviousScreenType +0x244); NInventory::CTCInventoryBase::CloseInventory 0x005BD597 (qualified; was the
//   stand-in BaseCloseInventory); the inline STLport vector<CComponent*>::clear (memmove 0x00BFEAE6) and list<CList*>::clear of
//   NUISystem::CList::GPreviousSelected 0x013B8A98 (free 0x00BFEA14); NUISystem::CManager slots 6 / 29 / 37; OnCloseMenus 0x0041D165;
//   ResetPrimitiveHandles 0x005FC510. Members: pmap_real +0x188, MapList +0x214, InventoryScreen +0x240, PanMode +0x248, InventoryOpen +0x58.
// Header: overlay opt-in FABLE_RETAIL_CTCINVENTORYMAP_CLOSEINVENTORY (the two declarations).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYMAP_MAPMEMBERS
#define FABLE_RETAIL_CTCINVENTORYMAP_HISCREEN
#define FABLE_RETAIL_CTCINVENTORYMAP_CLOSEINVENTORY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
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
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCInventoryMap.h"
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
#include "engine/CPlayerGui.h"
#include "engine/NHeroInformationScreens_CManager.h"

void CTCInventoryMap::CloseInventory()
{
    MapList = 0;
    pmap_real = 0;
    if (!IsInventoryScreen()) {
        if (PSoundManager) {
            PSoundManager->SetGainFader(0x10, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
            PSoundManager->SetGainFader(0x40, CUserProfileManager::Get().GetDialogueVolume(), 0.0f);
            PSoundManager->SetGainFader(0x80, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        }
        NUISystem::CManager::GetInstance()->PlaySoundW(NUISystem::CManager::GetInstance()->GetMiscThingsDef()->SoundExit, 1);
        CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(true);
        GetWorld().SetAsStarting3DRendering();
        CPlayerGui::Get().GetPHeroInfoScreensManager()->SelectScreenOfType(PreviousScreenType);
        NInventory::CTCInventoryBase::CloseInventory();
        for (unsigned long i = 0; i < Components.size(); ++i)
            delete Components[i];
        Components.clear();
        NUISystem::CManager::GetInstance()->ClearObservers();
        NUISystem::CManager::GetInstance()->ClearActionParams();
        NUISystem::CList::GPreviousSelected.clear();
        NUISystem::CManager::GetInstance()->ClearBastardChild();
        CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().CutscenePreload(false, false);
        NUISystem::CManager::OnCloseMenus();
    }
    InventoryScreen = true;
    ResetPrimitiveHandles();
    InventoryOpen = false;
    PanMode = false;
}
