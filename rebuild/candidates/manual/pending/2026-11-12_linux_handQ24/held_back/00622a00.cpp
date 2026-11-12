// CTCInventoryQuests::OpenInventory() -- opens the quest screen: hands the main player's creature to the UI manager, forgets the previous hero-info screen; when not already the inventory screen fades the music / sound / ambient groups, hands over the hero, turns 3D scene rendering off, counts the menu open and remembers the current hero-info screen (else selects this screen's type); records the guild validity, rebuilds the quest card list and clamps the selected index, opens the base inventory, and outside the inventory screen builds the quest selection menu from the building's hero-centre quest cards (live ones only)
// Leaf: ?OpenInventory@CTCInventoryQuests@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00622A00, 877 bytes, thiscall `ret`; speed TU (no frame pointer, `sub esp,24h`). Identity: Ego_r.pdb /
//   FableWin.pdb ?OpenInventory@CTCInventoryQuests@@UAEXXZ, CTCInventoryQuests vtable 0x012585B4 slot 30.
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 / SetPlayerThing (slot 26) / SetQuestInventory (slot 17) / CreateComponent 0x0041DB1D /
//   OnOpenMenus 0x0041D122; CMainGameComponent::Get (PMainGameComponent 0x013B86A0) -> GetPlayerManager (inline, +0x1C) ->
//   CPlayerManager::GetMainPlayer 0x00449970 -> CPlayer::GetPControlledCreature 0x00487DC0; GetDisplayEngine / Get3DEngine (inline) ->
//   CIEngine::Enable3DSceneRendering (slot 76); PSoundManager 0x013B8394 CASoundManager::SetGainFader (slot 46); NHeroInformationScreens::CBase
//   GetScreenType (slot 2) / IsInventoryScreen (slot 3) through the +0x130 base; CPlayerGui::Get (PClass 0x013B8790) ->
//   GetPHeroInfoScreensManager 0x00437CD0 -> NHeroInformationScreens::CManager::GetCurrentScreen 0x0066E7BD / SelectScreenOfType 0x0066E799;
//   CTCInventoryBase GetInventoryGuildValidity (slot 49), GetSelectedCategoryIndex 0x005BC4B2 / SetSelectedCategoryIndex 0x005BC4B6,
//   GetBuildingThing 0x005BC4C4, OpenInventory 0x005BD4EA (qualified); CTCInventoryQuests::InitialiseQuestCardList 0x00621A20 /
//   ConstructQuestList 0x0061B610; CThing::GetTC<CTCHeroCentre> (expanded: LowerBound 0x0040F020); CTCHeroCentre::GetQuestCards 0x0077DBC0;
//   CBaseIntelligentPointer::GetPItem 0x00A01B50; vector<CThing*> _M_insert_overflow 0x00436AC0, vector<CComponent*> 0x00485DBC;
//   _Rb_tree<CThing*,..>::_M_erase 0x00623560 (AvailableQuests.clear() inline); vector<CIntelligentPointer<CThing> >::erase 0x004AC0F0
//   (DisplayQuests.clear()); free 0x00BFEA14.
// Layout: Ego_r.pdb members at the retail offsets: IsGuildValid +0x14C, QuestCardCount +0x150, ActiveTestQuestMode +0x157,
//   PreviousScreenType +0x168, AvailableQuests +0x16C, DisplayQuests +0x178 (overlay FABLE_RETAIL_CTCINVENTORYQUESTS_OPENINVENTORY).
// Shape: the menu is held as a CList* (ConstructQuestList's parameter type): the push_back into Components then copies it into a
//   temporary (`mov [esp+18h],ebx`), where a CComponent* local is itself address-taken and spilled. The four GetPItem calls per card are
//   retail's (the third result unused). ConstructQuestList's body is visible in retail's TU (see the stand-in note below).
// Headers: overlay FABLE_RETAIL_CTCINVENTORYQUESTS_OPENINVENTORY, FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDCATEGORYINDEX,
//   FABLE_RETAIL_CTCHEROCENTRE_GETQUESTCARDS (hdr/README.md). Locals manager, list, quests, building, centre, cards are descriptive.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDCATEGORYINDEX
#define FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
#define FABLE_RETAIL_CTCINVENTORYQUESTS_OPENINVENTORY
#define FABLE_RETAIL_CTCHEROCENTRE_GETQUESTCARDS
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CMANAGER_MENUCOUNT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCInventoryQuests.h"
#include "engine/CTCHeroCentre.h"
#include "engine/CASoundManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"

using NUISystem::CComponent;

// Stand-in body (NOT the retail body): retail defines ConstructQuestList (0x0061B610, ~2 KB, not landed) in this same TU, and c2 sees
// from that visible body that it keeps no reference to the vector: the local quest list's _M_finish then stays in ebp across the
// card loop (retail 0x00622C49 `cmp ebp,[esp+30h]` ... `add ebp,4`). With a declaration only, the vector escapes at the call and every
// push_back reloads it. This body makes the real body's first call (NUISystem::CManager::GetInstance, 0x0061B643) and nothing else
// (an empty body is folded away with its call); it is kept out of line so the row still calls 0x0061B610.
#pragma auto_inline(off)
void CTCInventoryQuests::ConstructQuestList(const std::vector<CThing*>& quests, NUISystem::CList* list, QuestScreenType type, NUISystem::CList* parent)
{
    NUISystem::CManager::GetInstance();
}
#pragma auto_inline(on)
void CTCInventoryQuests::OpenInventory()
{
    NUISystem::CManager::GetInstance()->SetPlayerThing(CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature());
    PreviousScreenType = NHeroInformationScreens::NULL_SCREEN_TYPE;
    if (!IsInventoryScreen()) {
        if (PSoundManager) {
            PSoundManager->SetGainFader(0x10, 0.0f, 500.0f);
            PSoundManager->SetGainFader(0x40, 0.0f, 500.0f);
            PSoundManager->SetGainFader(0x80, 0.0f, 500.0f);
        }
        NUISystem::CManager::GetInstance()->SetPlayerThing(ParentThing);
        CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(false);
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
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    NUISystem::CList* list = static_cast<NUISystem::CList*>(manager->CreateComponent("UI_QUESTS_SELECTION_MENU", false));
    std::vector<CThing*> quests;
    CThing* building = GetBuildingThing();
    if (building && !building->IsDead()) {
        CTCHeroCentre* centre = 0;
        building->GetTC(TCI_HERO_CENTRE, &centre);
        std::vector<CIntelligentPointer<CThing> > cards;
        centre->GetQuestCards(cards);
        for (std::vector<CIntelligentPointer<CThing> >::iterator it = cards.begin(); it != cards.end(); ++it) {
            if (it->Get() && !it->Get()->IsDead()) {
                it->Get();
                quests.push_back(it->Get());
            }
        }
    }
    AvailableQuests.clear();
    DisplayQuests.clear();
    ConstructQuestList(quests, list, (QuestScreenType)0, 0);
    list->Initialise();
    list->ChangeState(5);
    Components.push_back(list);
}
