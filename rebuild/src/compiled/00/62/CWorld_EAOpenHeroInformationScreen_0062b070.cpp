// CWorld::EAOpenHeroInformationScreen
// Leaf: ?EAOpenHeroInformationScreen@CWorld@@QAEXPBVCGameEvent@@@Z
// Flags: /GS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_EVENTACTIONS
#define FABLE_RETAIL_CGAMEEVENT_DATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE
#define FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_GETULONG_PASS26
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_GETTC_DONOR_NAMES_PASS23
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_RENDERING_NORMAL_PASS26
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_LIVEGUI
#define FABLE_RETAIL_CPLAYERGUI_TOGGLE_LIVE_PASS26
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_ISOPEN
#define FABLE_RETAIL_CTCQUICKACCESSMENU_DEACTIVATE
#define FABLE_RETAIL_CTCQUICKACCESSMENU_ISACTIVE
#define FABLE_RETAIL_CTCBOASTUI
#define FABLE_RETAIL_CTCBOASTUI_ACTIVE_PASS26
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CTCINGAMEMENU
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORY_SHORTCUT_PASS26
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CTCHERO_TUTORIAL
#define FABLE_RETAIL_TUTORIAL_CATEGORIES_PASS26
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINPUTMANAGER_JOYSTICKS
#define FABLE_RETAIL_CINPUTMANAGER_JOYSTICKQUEUE
#define FABLE_RETAIL_CJOYSTICK
#define FABLE_RETAIL_CJOYSTICK_VIRTUALS
#include "engine/CGameEvent.h"
#include "engine/CGameEventDataPackage.h"
#include "engine/CWorld.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/CTCQuickAccessMenu.h"
#include "engine/CTCBoastUI.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CTCInGameMenu.h"
#include "engine/CTCInventory.h"
#include "engine/CTCHero.h"
#include "engine/ETutorialCategory.h"
#include "engine/CInputManager.h"
#include "engine/ThingAlive.h"

void CWorld::EAOpenHeroInformationScreen(const CGameEvent* event)
{
    {
        CPlayer* player = GetPlayer(event->GetPlayerNumber());
        if (player->HasMode(PLAYER_MODE_VIEW_HERO))
            return;
    }
    if (WaitingForInventoryTutorialToFinish)
        return;
    CThingPlayerCreature* pcreature = GetPlayer(event->GetPlayerNumber())->GetPControlledCreature();
    if (!IS_THING_ALIVE(pcreature) || !(pcreature->GetHealth() > 0.0f))
        return;
    if (CMainGameComponent::Get()->PeekDisplayEngine().IsRenderingNormalScene()) {
        const CTCQuickAccessMenu* ptc_menu;
        const CTCBoastUI* ptc_boast_ui;
        const CTCQuestCompletionUI* ptc_qc_ui;
        if (pcreature->PeekTC(&ptc_menu) && !ptc_menu->IsActive() &&
            pcreature->PeekTC(&ptc_boast_ui) && !ptc_boast_ui->IsActive() &&
            pcreature->PeekTC(&ptc_qc_ui) && !ptc_qc_ui->IsActive() &&
            !CPlayerGui::Get().GetSubtitleRenderer().IsDisplayingText()) {
            CGameEventDataPackage event_data;
            event->GetDataPackage(&event_data);
            CPlayer* player = GetPlayer(event->GetPlayerNumber());
            EPlayerMode player_mode = static_cast<EPlayerMode>(event_data.GetULONG());
            if (player_mode == PLAYER_MODE_NAVIGATE_IN_GAME_MENU && CPlayerGui::Get().GetPHeroInfoScreensManager()->IsOpen())
                return;
            {
                const CTCInGameMenu* ptc_in_game_menu;
                if (pcreature->PeekTC(&ptc_in_game_menu) && ptc_in_game_menu->IsOpen())
                    return;
            }
            if (player_mode == PLAYER_MODE_NAVIGATE_IN_GAME_MENU) {
                PInputManager->GetJoystick(pcreature->GetJoystickDeviceNumber()).ClearAllRumbles();
            } else {
                const CTCInventory* ptc_inventory = 0;
                pcreature->PeekTC(&ptc_inventory);
                if (ptc_inventory && ptc_inventory->GetScreenToShortcut() == 4) {
                    CTCHero* ptc_hero = 0;
                    pcreature->GetTC(&ptc_hero);
                    if (ptc_hero && ptc_hero->GiveTutorialForCategory(TUTORIAL_CATEGORY_WORLD_MAP, true)) {
                        WaitingForInventoryTutorialToFinish = true;
                        HeroInformationScreenModeAfterTutorial = player_mode;
                        return;
                    }
                } else {
                    CTCHero* ptc_hero = 0;
                    pcreature->GetTC(&ptc_hero);
                    if (ptc_hero && ptc_hero->GiveTutorialForCategory(TUTORIAL_CATEGORY_INVENTORY, true)) {
                        WaitingForInventoryTutorialToFinish = true;
                        HeroInformationScreenModeAfterTutorial = player_mode;
                        return;
                    }
                }
            }
            OpenHeroInformationScreen(player_mode);
        }
    } else if (CPlayerGui::Get().IsLiveGUIMode()) {
        CPlayerGui::Get().ToggleLiveGui();
    }
}