// CTCInventoryQuests::OpenInventoryPC() -- open the PC quest screen: capture the background, turn 3D rendering off, hand the hero to the UI manager; when not reached from another hero-info screen fade the sound groups, hand over the quest-screen owner, open the menus and remember the previous screen, else switch the hero-info manager to this screen; read the guild validity, rebuild the quest-card list and clamp the selected category; open the base inventory, then (as a standalone screen) build the mouse pointer and the quest selection menu, hook its scroll buttons, frame children and clickables, list the live quest cards of the building's hero centre, keep both components
// Grade: DIFFER(1335v1338) -- the local quest list escapes into the declaration-only ConstructQuestListPC 0x0061E6D0: retail keeps its _M_finish in ebx through the card loop (0x00622839 `cmp ebx,[esp+34h]` .. 0x0062284C) and packs quests at [esp+2Ch] / cards at [esp+20h]; ours reloads it ([esp+24h]) with quests at [esp+20h]. A same-TU noinline ConstructQuestListPC body (probe/c4.cpp) gives RELOCATION_MATCH: land as a pair with 0x0061E6D0 (retail compiles it first in this TU)
// Leaf: ?OpenInventoryPC@CTCInventoryQuests@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006224C0, 1338 bytes, plain `ret`; speed TU without a frame pointer (`sub esp,28h`). Identity:
//   CTCInventoryQuests vtable 0x012585B4 slot 86; Ego_r.pdb / FableWin.pdb ?OpenInventoryPC@CTCInventoryQuests@@UAEXXZ (Ego_r 0x005A0020,
//   right after ConstructQuestListPC 0x0059CED0 as retail's 0x006224C0 follows 0x0061E6D0's TU neighbours).
// Callees: CTCInventoryBase::CaptureBackBufferToTexture 0x005BC6ED, OpenInventory 0x005BD4EA (qualified), GetSelectedCategoryIndex 0x005BC4B2 /
//   SetSelectedCategoryIndex 0x005BC4B6, GetBuildingThing 0x005BC4C4; CIEngine::Enable3DSceneRendering (slot 76); NUISystem::CManager::GetInstance
//   0x0041E5F2, SetPlayerThing (slot 26), SetQuestInventory (slot 17), CreateComponent 0x0041DB1D, OnOpenMenus 0x0041D122, AddObserver (CObservable
//   slot 2); CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0; PSoundManager SetGainFader (slot 46);
//   CPlayerGui::GetPHeroInfoScreensManager 0x00437CD0, NHeroInformationScreens::CManager::GetCurrentScreen 0x0066E7BD / SelectScreenOfType 0x0066E799;
//   InitialiseQuestCardList 0x00621A20, ConstructQuestListPC 0x0061E6D0; CComponent FindChild (slot 3), Initialise (43), ChangeState (48),
//   AddPositionChild (124), CObserver::ObserveEvent (slot 3 of the +4 base); CTCHeroCentre::GetQuestCards 0x0077DBC0; CVectorMap LowerBound
//   0x0040F020; CBaseIntelligentPointer::GetPItem 0x00A01B50; map _M_erase 0x00623560; vector<CIntelligentPointer<CThing>>::erase 0x004AC0F0;
//   vector _M_insert_overflow 0x00436AC0 (CThing*) / 0x00485DBC (CComponent*); free 0x00BFEA14.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_CAPTUREBACKBUFFER
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDCATEGORYINDEX
#define FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
#define FABLE_RETAIL_CTCINVENTORYQUESTS_OPENPC
#define FABLE_RETAIL_CTCHEROCENTRE_GETQUESTCARDS
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CMANAGER_MENUCOUNT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CTCInventoryQuests.h"
#include "engine/CTCHeroCentre.h"
#include "engine/CASoundManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"

using NUISystem::CComponent;

void CTCInventoryQuests::OpenInventoryPC()
{
    CaptureBackBufferToTexture();
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(false);
    AccumFadeTime = 0.0f;
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    manager->SetPlayerThing((CThing*)CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature());
    PreviousScreenType = NHeroInformationScreens::NULL_SCREEN_TYPE;
    if (!IsInventoryScreen()) {
        if (PSoundManager) {
            PSoundManager->SetGainFader(0x10, 0.0f, 500.0f);
            PSoundManager->SetGainFader(0x40, 0.0f, 500.0f);
            PSoundManager->SetGainFader(0x80, 0.0f, 500.0f);
        }
        NUISystem::CManager::GetInstance()->SetPlayerThing(ParentThing);
        NUISystem::CManager::OnOpenMenus();
        PreviousScreenType = CPlayerGui::Get().GetPHeroInfoScreensManager()->GetCurrentScreen()->GetScreenType();
        ActiveTestQuestMode = false;
    } else {
        CPlayerGui& gui = CPlayerGui::Get();
        gui.GetPHeroInfoScreensManager()->SelectScreenOfType(GetScreenType());
        ActiveTestQuestMode = true;
    }
    IsGuildValid = GetInventoryGuildValidity();
    InitialiseQuestCardList();
    if (QuestCardCount > 0) {
        long last = QuestCardCount - 1;
        if (GetSelectedCategoryIndex() > last)
            SetSelectedCategoryIndex(last);
    }
    NInventory::CTCInventoryBase::OpenInventory();
    if (IsInventoryScreen())
        return;

    NUISystem::CManager::GetInstance()->SetQuestInventory(this);
    manager = NUISystem::CManager::GetInstance();
    CComponent* mousePointer = manager->CreateComponent("UI_MOUSE_POINTER", false);
    mousePointer->Initialise();
    NUISystem::CList* list = static_cast<NUISystem::CList*>(manager->CreateComponent("PC_QUESTS_SELECTION_MENU", false));

    CComponent* scrollListUp = list->FindChild("SCROLL_LIST_UP");
    if (scrollListUp)
        scrollListUp->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    CComponent* scrollDescriptionUp = list->FindChild("SCROLL_DESCRIPTION_UP");
    if (scrollDescriptionUp)
        scrollDescriptionUp->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    CComponent* scrollListDown = list->FindChild("SCROLL_LIST_DOWN");
    if (scrollListDown)
        scrollListDown->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    CComponent* scrollDescriptionDown = list->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (scrollDescriptionDown)
        scrollDescriptionDown->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);

    CComponent* frame = list->FindChild("PC_UI_MAP_FRAME");
    CComponent* title = list->FindChild("PC_TITLE_QUEST_SELECTION");
    frame->AddPositionChild(title);
    CComponent* viewport = list->FindChild("PC_DESCRIPTION_VIEWPORT");
    if (viewport)
        frame->AddPositionChild(viewport);

    std::vector<CThing*> quests;
    CThing* building = GetBuildingThing();
    if (IS_THING_ALIVE(building)) {
        CTCHeroCentre* centre = 0;
        building->GetTC(TCI_HERO_CENTRE, &centre);
        std::vector<CIntelligentPointer<CThing> > cards;
        centre->GetQuestCards(cards);
        for (std::vector<CIntelligentPointer<CThing> >::iterator it = cards.begin(); it != cards.end(); ++it) {
            if (IS_THING_ALIVE(*it)) {
                it->Get();
                quests.push_back(it->Get());
            }
        }
    }
    AvailableQuests.clear();
    DisplayQuests.clear();
    ConstructQuestListPC(0, quests, list, (QuestScreenType)0, 0);
    list->Initialise();

    CComponent* backClickable = list->FindChild("PC_BACK_CLICKABLE");
    if (backClickable) {
        NUISystem::CManager::GetInstance()->AddObserver(backClickable);
        backClickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    CComponent* closeClickable = list->FindChild("PC_CLOSE_CLICKABLE");
    if (closeClickable) {
        NUISystem::CManager::GetInstance()->AddObserver(closeClickable);
        closeClickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    list->ChangeState(5);
    Components.push_back(list);
    Components.push_back(mousePointer);
}
