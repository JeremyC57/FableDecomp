// CTCInventory::ClosePCInventory(bool closeInGameMenu) -- leave the PC inventory screens: cancel the key redefiner, close the edit boxes and the inventory menu, close the in-game menu, close the online scoreboard, delete the PC screens, reset the open menu, clear the UI manager's bastard child, observers and action params and the lists' previous-selection stack, restore the three audio group volumes from the user profile, close the base inventory, restart game rendering and stop the cutscene preload
// Leaf: ?ClosePCInventory@CTCInventory@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00587847, 506 bytes, `ret 4` (frame pointer, `leave`).
// Callees: NUISystem::CRedefinerList::Cancel 0x00556470, DeactivateAllEditBoxes 0x00582414, EditBoxes (+0x144) list clear 0x00592BA7,
//   ClosePCInventoryMenu 0x00584B42, the CThing component lookups (bitset test 0x00410DE0; the TCI_INVENTORY_WEAPONS / _CLOTHING ones
//   discarded, VC7.1 keeps only the presence tests; LowerBound 0x0040F020 out of line for TCI_IN_GAME_MENU / TCI_HERO_ONLINE_SCOREBOARD),
//   CTCInGameMenu::CloseInGameMenuPC 0x0047DBB8, CTCHeroOnlineScoreboard::Close 0x00562E70, vector<CComponent*>::erase 0x00485AA7,
//   NUISystem::CManager slots 37 / 6 / 29, the GPreviousSelected (0x013B8A98) list clear 0x004855A3, CUserProfileManager::Get 0x0040D2A0 +
//   inline GetSoundVolume / GetDialogueVolume, PSoundManager SetGainFader (slot 46), NInventory::CTCInventoryBase::CloseInventory 0x005BD597
//   (qualified; was the stand-in ClearQuestPersistentFlags), StartGameRendering 0x00581C1E, CIEngine slot 56 CutscenePreload.
// NF62 (2026-11-17, round 7): whole-row port onto the shared headers (the local CThing / CVectorMap / bitset / CTCInGameMenu / CManager /
//   CUserProfileManager / CMainGameComponent / CDisplayEngine / CTCInventory models and the CComponentVector / CTreePane / CInventoryCamera
//   stand-ins are gone). The two discarded lookups are GetTC calls whose CVectorMap::LowerBound (FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND,
//   an ordinary inline body) VC7.1 expands and then drops as dead; the later two exceed the inline budget and call it out of line.
//   Overlay opt-ins: FABLE_RETAIL_CTCINGAMEMENU_CLOSEPC, FABLE_RETAIL_CREDEFINERLIST_CANCEL, FABLE_RETAIL_CTCHEROONLINESCOREBOARD_CLOSE.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
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
#define FABLE_RETAIL_CUSERPROFILEMANAGER_VOLUME
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINGAMEMENU_CLOSEPC
#define FABLE_RETAIL_CREDEFINERLIST_CANCEL
#define FABLE_RETAIL_CTCHEROONLINESCOREBOARD_CLOSE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
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
#include "engine/CTCInGameMenu.h"
#include "engine/CTCHeroOnlineScoreboard.h"
#include "engine/NUISystem_CRedefinerList.h"

void CTCInventory::ClosePCInventory(bool closeInGameMenu)
{
    NUISystem::CRedefinerList::Cancel();
    DeactivateAllEditBoxes(false);
    EditBoxes.clear();
    ClosePCInventoryMenu();
    {
        CTCBase* weapons;
        ParentThing->GetTC(TCI_INVENTORY_WEAPONS, &weapons);
    }
    {
        CTCBase* clothing;
        ParentThing->GetTC(TCI_INVENTORY_CLOTHING, &clothing);
    }
    {
        CTCInGameMenu* menu;
        if (ParentThing->GetTC(TCI_IN_GAME_MENU, &menu))
            menu->CloseInGameMenuPC(closeInGameMenu);
    }
    {
        CTCHeroOnlineScoreboard* scoreboard;
        if (ParentThing->GetTC(TCI_HERO_ONLINE_SCOREBOARD, &scoreboard))
            scoreboard->Close();
    }
    for (unsigned long i = 0; i < Components.size(); ++i)
        delete Components[i];
    Components.clear();
    CurrentOpenMenu = (CURRENT_OPEN_MENU)0;
    NUISystem::CManager::GetInstance()->ClearBastardChild();
    NUISystem::CManager::GetInstance()->ClearObservers();
    NUISystem::CManager::GetInstance()->ClearActionParams();
    NUISystem::CList::GPreviousSelected.clear();
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
        PSoundManager->SetGainFader(0x40, CUserProfileManager::Get().GetDialogueVolume(), 0.0f);
        PSoundManager->SetGainFader(0x80, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
    }
    NInventory::CTCInventoryBase::CloseInventory();
    StartGameRendering();
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().CutscenePreload(false, false);
}
