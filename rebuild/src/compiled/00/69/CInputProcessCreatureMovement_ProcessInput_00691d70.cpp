// CInputProcessCreatureMovement::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input) -- queue the creature movement event, toggle sneaking
// Leaf: ?ProcessInput@CInputProcessCreatureMovement@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe 0x00691D70, 528 bytes (code to `ret 8` at 0x00691F59, then the switch tables 0x00691F5C / 0x00691F64; next function 0x00691F80).
// Flags: /GS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_TOGGLE_SNEAK
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CGAMEEVENT_INLINE_ADDMORE
#define FABLE_RETAIL_CPROCESSEDINPUT_ADDEVENT
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#define FABLE_RETAIL_CINPUTEVENT_DEVICETYPE
#define FABLE_RETAIL_CTCSTEALTH_SNEAKTOGGLE
#define FABLE_RETAIL_INPUTPROCESS_CREATUREMOVEMENT_PROCESSINPUT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/EGameAction.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCStealth.h"
#include "engine/CInputEvent.h"
#include "engine/CGameEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CPlayer.h"
#include "engine/CInputProcessCreatureMovement.h"

void CInputProcessCreatureMovement::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input)
{
    if (!PCreature.Get() || PCreature->IsDead()) return;
    C2DVector movement;
    if (GetControlMovementFromGameActionEvent(GAME_ACTION_MOVEMENT, event, movement)) {
        bool button = false;
        if (event.GetDeviceType() == 2) button = true;
        else if (event.GetDeviceType() == 3) {
            switch (event.GetType()) {
            case 4: case 5: case 7: case 8: case 10: case 11:
            case 22: case 23: case 24: case 25: case 26: case 27: case 28: case 29: case 30: case 31:
                button = true;
                break;
            }
        }
        if (button && !IsEventButtonReleased(event)) return;
        CGameEvent game_event(0x10);
        if (PCreature.Get() && !PCreature->IsDead()) {
            long key = -1;
            if (event.GetDeviceType() == 2) key = event.GetKey();
            game_event.AddUHUGE(PCreature->GetUniqueID());
            game_event.AddFloat(movement.X);
            game_event.AddFloat(movement.Y);
            game_event.AddSLONG(key);
            processed_input.AddGameEvent(game_event);
        }
    }
    if (GetGamePlayerInterface().IsToggleSneak() && IsEventButtonPressed(event) && IsEventGameAction(GAME_ACTION_SNEAK, event)) {
        CThingPlayerCreature* hero = MyPlayer->GetPControlledCreature();
        CTCStealth* stealth;
        if (hero->GetTC(TCI_STEALTH, &stealth)) {
            stealth->SetSneakingToggleSet(!stealth->IsSneaking());
            processed_input.SetAsPerformedEvent();
        }
    }
}