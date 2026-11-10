// CInputProcessControlCreatureActivateZTarget::ProcessInput -- queue target lock activation.
// Leaf: ?ProcessInput@CInputProcessControlCreatureActivateZTarget@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe 0x00688260,137 bytes.
// Flags: /GS
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_TOGGLE_ZTARGET
#define FABLE_RETAIL_CGAMEEVENT_INLINE_CTOR
#define FABLE_RETAIL_CPROCESSEDINPUT_ADDEVENT
#include "engine/EPlayerMode.h"
#include "engine/EGameAction.h"
class CInputEvent;
class CProcessedInput;
#include "engine/CGameEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CPlayer.h"
#include "engine/CInputProcessControlCreatureActivateZTarget.h"

void CInputProcessControlCreatureActivateZTarget::ProcessInput(const CInputEvent& event,CProcessedInput& processed_input)
{
    if(MyPlayer->GetCurrentMode()==PLAYER_MODE_Z_TARGET)return;
    bool activate=false;
    if(GetGamePlayerInterface().IsToggleZTarget()) {
        if(IsEventButtonHeld(event))activate=true;
    } else {
        if(IsEventButtonPressed(event))activate=true;
    }
    if(activate && IsEventGameAction(GAME_ACTION_LOCK_TARGET,event)) {
        CGameEvent action(0x15);
        processed_input.AddGameEvent(action);
    }
}