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

#include <stdio.h>
#include <string.h>
static unsigned long camera[256], main_obj[32], output[44];
static int mode,log_count,priority;
static unsigned long calls[3];
EPlayerMode CPlayer::GetCurrentMode() const {return static_cast<EPlayerMode>(mode);}
void FABLE_FASTCALL CInputProcessMain::AddManualCameraEvent(unsigned char on,CProcessedInput&) {calls[log_count++]=11+100*on;}
void FABLE_FASTCALL CInputProcessMain::AddUnbindCameraEvent(unsigned char on,CProcessedInput&) {calls[log_count++]=12+100*on;}
#ifndef FABLE_ROLL_REAL_PRIORITY
void CProcessedInput::SetPriority(EGameEventPriority p) {Priority=p;}
#endif
void CInputProcessMain::ProcessInput(const CInputEvent&,CProcessedInput& input) {HandleDebugCameraAction(input);}
int main() {
    main_obj[0x18/4]=reinterpret_cast<unsigned long>(camera);
    unsigned long event[16]={0};
    for(mode=0;mode<48;++mode)for(int b=0;b<2;++b){
        reinterpret_cast<unsigned char*>(camera)[0x220]=b!=0;
        log_count=0;output[0xac/4]=0;
        reinterpret_cast<CInputProcessMain*>(main_obj)->CInputProcessMain::ProcessInput(*reinterpret_cast<CInputEvent*>(event),*reinterpret_cast<CProcessedInput*>(output));
        bool skip=(mode>=4&&mode<=13)||mode==23;
        if(output[0xac/4]!=(skip?0:5))return 1;
        if(skip) {if(log_count!=0)return 2;}
        else if(mode==19) {if(log_count!=1||calls[0]!=11)return 3;}
        else if(mode==1&&b) {if(log_count!=1||calls[0]!=12)return 4;}
        else if(log_count!=2||calls[0]!=111||calls[1]!=112)return 5;
    }
    puts("00687ed0_TEST PASS"); return 0;
}
