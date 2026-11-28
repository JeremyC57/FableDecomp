// Leaf: ?HandleDebugCameraAction@CInputProcessMain@@AAEXAAVCProcessedInput@@@Z
// Flags:
#define FABLE_RETAIL_PROCESS_ROLLING_PASS30
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_ISSPELLMODE
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_CAINPUTPROCESS_ADDTOINTERFACE
#define FABLE_RETAIL_CAINPUTPROCESS_INIT
#define FABLE_RETAIL_INPUTPROCESS_VTABLE
#define FABLE_RETAIL_EXPRESSION_FRAME
#include "engine/CPlayer.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessMain.h"
#include "engine/EPlayerMode.h"
void CInputProcessMain::HandleDebugCameraAction(CProcessedInput& input)
{
    switch (MyPlayer->GetCurrentMode()) {
    case PLAYER_MODE_CONTROL_CAMERA:
        CInputProcessMain::AddManualCameraEvent(false, input);
        input.SetPriority(GAME_EVENT_PRIORITY_MAX);
        break;
    case PLAYER_MODE_NAVIGATE_INVENTORY:
    case PLAYER_MODE_NAVIGATE_INVENTORY_CLOTHING:
    case PLAYER_MODE_NAVIGATE_INVENTORY_WEAPONS:
    case PLAYER_MODE_NAVIGATE_INVENTORY_ABILITIES_SCREEN:
    case PLAYER_MODE_NAVIGATE_INVENTORY_MAP_SCREEN:
    case PLAYER_MODE_NAVIGATE_INVENTORY_MAGIC_SCREEN:
    case PLAYER_MODE_NAVIGATE_INVENTORY_STATS_SCREEN:
    case PLAYER_MODE_NAVIGATE_INVENTORY_EXPERIENCE_SCREEN:
    case PLAYER_MODE_NAVIGATE_INVENTORY_TRADE_SCREEN:
    case PLAYER_MODE_NAVIGATE_INVENTORY_QUESTS_SCREEN:
    case PLAYER_MODE_NAVIGATE_IN_GAME_MENU:
        break;
    case PLAYER_MODE_CONTROL_CREATURE:
        if (MyPlayer->IsUsingFreeCamera()) {
            AddUnbindCameraEvent(false, input);
            input.SetPriority(GAME_EVENT_PRIORITY_MAX);
        } else {
            CInputProcessMain::AddManualCameraEvent(true, input);
            AddUnbindCameraEvent(true, input);
            input.SetPriority(GAME_EVENT_PRIORITY_MAX);
        }
        break;
    default:
        CInputProcessMain::AddManualCameraEvent(true, input);
        AddUnbindCameraEvent(true, input);
        input.SetPriority(GAME_EVENT_PRIORITY_MAX);
        break;
    }
}
