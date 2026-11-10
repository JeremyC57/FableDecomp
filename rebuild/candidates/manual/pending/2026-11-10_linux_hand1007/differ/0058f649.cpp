// CTCInventory::OpenPCInventory() -- open the PC inventory: fade the music / sound / ambient groups out, cancel the hero's fireball move, reset the open menu, prepare the screen with 3D scene rendering off, create the mouse pointer and the top-level list (the skills entry made unavailable with its arrow deleted when the hero has no abilities, else given the open-skills pre-action; the photojournal entry likewise when there are no screenshots), hand the inventory and the hero to the UI manager, keep both components, initialise every component, put the list in state 5, open the base inventory and open the shortcut screen asked for
// Grade: DIFFER(883v883) -- frame-slot colouring only: retail gives the manager [ebp-4] and shares [ebp-8] between the fireball lookup key, the CCharString temporaries and the entry / arrow pointers; ours shares [ebp-8] between the key and the manager (temporaries / entry [ebp-4]); plus the photojournal arrow vtable load scheduled after the Method store. Inert: entry declared before the manager, manager declared at the top, the fireball lookup in its own block; HasTC + GetTC(type) reference is worse (870 bytes)
// Leaf: ?OpenPCInventory@CTCInventory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058F649, 883 bytes, thiscall `leave ; ret`; frame-pointer size TU (optimize("y",off) + ("s",on)),
//   frame 0x10. Identity: Ego_r.pdb / FableWin.pdb ?OpenPCInventory@CTCInventory@@QAEXXZ (the PC twin of the console OpenInventory).
// Callees: PSoundManager 0x013B8394 CASoundManager::SetGroupVolume (slot 46) for 0x10 / 0x40 / 0x80 with (0.0f, 500.0f @ 0x01237584);
//   CThing::GetTC (bitset test 0x00410DE0, LowerBound 0x0040F020) for TCI_FIREBALL_SPELL and CTCFireballSpell::CancelMove 0x007C8FD0;
//   CTCInventoryBase::CaptureBackBufferToTexture 0x005BC6ED (Ego_r.pdb; the landed spelling PrepareScreen is descriptive) and OpenInventory 0x005BD4EA (qualified); CMainGameComponent::GetDisplayEngine (inline)
//   / CDisplayEngine::Get3DEngine (inline) / CIEngine::Enable3DSceneRendering (slot 76); NUISystem::CManager::GetInstance 0x0041E5F2 /
//   CreateComponent 0x0041DB1D / SetInventory (slot 21) / SetPlayerThing (slot 26); CCharString 0x0099EBF0 / ~ 0x0099EAE0; CComponent
//   FindChild (slot 3), Initialise (43), ChangeState (48), SetDeletion (62), AddPreAction (69), MakeUnavailable (92); NUISystem::CDeletion
//   0x0042BED4 / copy 0x0042CD84 / list dtor 0x0042ABCA; operator new 0x00BFEA1A, CAction() 0x0042BE50, CCountedPointer<CAction>(T*)
//   0x0042AA29 / ReleaseData 0x00429160; CTCInventory::HasScreenshots 0x00581B33 / OpenInventoryShortcut 0x0058E99F;
//   vector<CComponent*>::push_back 0x00486024.
// Header: overlay FABLE_HDR_OVERLAY (FABLE_RETAIL_CTCINVENTORY_OPENPC, FABLE_RETAIL_CTCINVENTORYBASE_CAPTUREBACKBUFFER).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_CAPTUREBACKBUFFER
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU
#define FABLE_RETAIL_CTCINVENTORY_OPENPC
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CTCFIREBALLSPELL
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_SCOPED_PTR
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCFireballSpell.h"
#include "engine/CASoundManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/CCountedPointer.h"

using NUISystem::CComponent;
using NUISystem::CAction;

void CTCInventory::OpenPCInventory()
{
    if (PSoundManager) {
        PSoundManager->SetGroupVolume(0x10, 0.0f, 500.0f);
        PSoundManager->SetGroupVolume(0x40, 0.0f, 500.0f);
        PSoundManager->SetGroupVolume(0x80, 0.0f, 500.0f);
    }
    {
        CTCFireballSpell* fireball;
        if (ParentThing->GetTC(TCI_FIREBALL_SPELL, &fireball))
            fireball->CancelMove();
    }
    CurrentOpenMenu = (CURRENT_OPEN_MENU)0;
    CaptureBackBufferToTexture();
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(false);
    AccumFadeTime = 0.0f;

    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    PMouse_Pointer = manager->CreateComponent("UI_MOUSE_POINTER", false);
    PTop_Level_List = manager->CreateComponent("PC_TOP_LEVEL_LIST", false);

    // the skills entry is unavailable (its arrow deleted) until the hero has abilities
    CComponent* entry = PTop_Level_List->FindChild("PC_MENU_ENTRY_SKILLS");
    if (entry) {
        if (!ParentThing->HasTC(TCI_HERO_ABILITIES)) {
            entry->MakeUnavailable();
            CComponent* arrow = entry->FindChild("PC_MENU_ENTRY_TOP_LEVEL_ARROW");
            if (arrow) {
                NUISystem::CDeletion deletion;
                deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        } else {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = NUISystem::UI_ACTION_TYPE_OPEN_PC_SKILLS_MENU;
            entry->AddPreAction(action);
        }
    }
    // the photojournal entry likewise until there are screenshots
    entry = PTop_Level_List->FindChild("PC_MENU_ENTRY_PHOTOJOURNAL");
    if (entry) {
        if (!HasScreenshots()) {
            entry->MakeUnavailable();
            CComponent* arrow = entry->FindChild("PC_MENU_ENTRY_TOP_LEVEL_ARROW");
            NUISystem::CDeletion deletion;
            deletion.Method = NUISystem::CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(deletion);
        } else {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = NUISystem::UI_ACTION_TYPE_OPEN_PHOTOJOURNAL;
            entry->AddPreAction(action);
        }
    }
    manager->SetInventory(this);
    manager->SetPlayerThing(ParentThing);

    Components.push_back(PMouse_Pointer);
    Components.push_back(PTop_Level_List);
    for (unsigned long i = 0; i < Components.size(); ++i)
        Components[i]->Initialise();
    PTop_Level_List->ChangeState(5);
    NInventory::CTCInventoryBase::OpenInventory();
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(false);
    if (ScreenToShortcut != -1) {
        OpenInventoryShortcut(ScreenToShortcut);
        ScreenToShortcut = -1;
    }
}
