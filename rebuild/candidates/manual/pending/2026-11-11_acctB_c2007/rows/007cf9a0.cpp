// CTCTavernGameShoveHaPenny::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input, CInputProcessTavernGame* input_process) -- shove-ha'penny input: positioning slides the held coin along the base line (clamped to the def's band) and the yes button starts aiming; aiming blends the joystick into the coin's velocity, flicks the coin when the action button is let go and lets the cancel button go back to positioning; other states go to the base game; while a coin game state runs the quit buttons ask to leave
// Leaf: ?ProcessInput@CTCTavernGameShoveHaPenny@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z
// Flags: /d2inlT116
// Inline-budget: /d2inlT116 -- std::max<float> at 0x007CFB45 (the clamp of the held coin's Y velocity in the aim state) is expanded in
//   retail like the two other max / min sites (0x007CFD51, 0x007CFD7D); VC7.1's c2 inline trace (c2.dll 0x1078D1AA) gives that site
//   c1 19 at static frequency 0.0273 (switch case 1/4 x `(A && B) || (C && D)` 0.4375 x two if/else halvings), score 115, so the
//   default threshold 74 calls it out of line (DIFFER 1433v1434, and the extra call shifts def / input_process to ebx / ebp).
//   T115 still calls it, T116 expands it and the whole row is RELOCATION_MATCH; no other decision in the TU moves (GetSubDef<..> stays
//   out of line at score 174). Named-bool / else-if / nested spellings of the aim branches reach at most f 0.039 (score 86).
// Retail: Fable.exe 0x007CF9A0, 1434 bytes (`ret 0Ch` at 0x007CFF37). Vtable 0x0126F2F4 slot 28 (0x0126F364); Ego_r.pdb / FableWin.pdb
//   ?ProcessInput@CTCTavernGameShoveHaPenny@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z.
// Switch on TavernGameState (sub / dec chain, no jump table): 15 ETGS_POSITION_COIN 0x007CFCF5, 16 ETGS_AIM_COIN 0x007CFA09, 17
//   ETGS_WATCH_COINS break, default CTCTavernGame::ProcessInput 0x005EC270.
// Callees: CMainGameComponent::ConstantFPS 0x01375550 (inline GetConstantFPS); CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef>
//   0x007D1260 (this TU, out of line); CInputProcessGameBase::IsEventButtonPressed 0x00687A90, IsEventGameAction 0x00687CF0,
//   GetControlMovementFromGameActionEvent 0x00687D00, IsGameActionEventInQueue(EGameAction, EInputButtonEventType, float&) 0x00687D80;
//   CTCTavernGame::SetState 0x005E9B20, PlaySoundW 0x005EA950 ("TAVERN_GAME_COIN_FLICK" 0x0126F1B0), GetDef 0x005EA000, GetText
//   0x005E9E70, GetTextWithNarrator 0x005EB3A0, SetStateAfterYesNo 0x005EAF30; CProcessedInput::SetAsPerformedEvent 0x00A0D390;
//   C2DVector::Normalise 0x00A14540; CTCTavernGameCoinBase::UpdateCoinSounds(bool) 0x008EF910; CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Shape: the y impulse `-(AverageJoystickPos.Y * JoystickScalePCY)` is a named float (retail negates with fchs and then adds the
//   translation, 0x007CFB3E / 0x007CFE3D; one expression folds to fsubr); `AverageJoystickPos.Y != 0.0f` loads the constant first for
//   fucompp; the joystick blend uses C2DVector's operator* / operator+ (the right operand is built first, 0x007CFBF4); the clamps are
//   STLport std::max / std::min through references; GetGameState() as SetStateAfterYesNo's last argument (as the sibling 0x007D8570).
//   The position state's frame time is computed before the def lookup and shares its slot with the clamp temporaries.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_PRESSED_ACCESS
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_EVENTINQUEUE_ACCESS
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameShoveHaPennyDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCoinBase.h"
#include "engine/CTCTavernGameShoveHaPenny.h"
#include "fable_game.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessTavernGame.h"

// GetSubDef<CTavernGameShoveHaPennyDef> 0x007D1260 (landed): defined in this TU as in retail's, so VC7.1 keeps it out of line
// and sees that it does not capture the def slot (the def pointer stays in ebp for the whole function).
template <>
bool CDefClassBase::GetSubDef<CTavernGameShoveHaPennyDef>(CDefPointer<const CTavernGameShoveHaPennyDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernGameShoveHaPennyDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

void CTCTavernGameShoveHaPenny::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input,
                                             CInputProcessTavernGame* input_process)
{
    float frame_time = 1.0f / CMainGameComponent::GetConstantFPS();
    CDefPointer<const CTavernGameShoveHaPennyDef> def;
    ParentThing->PeekBaseDef().GetSubDef(&def);
    C2DVector movement;
    float time;
    switch (TavernGameState) {
    case ETGS_POSITION_COIN:
        if (input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_MOVEMENT, event)
            && input_process->GetControlMovementFromGameActionEvent(GAME_ACTION_TAVERN_GAME_MOVEMENT, event, movement)) {
            Held->Pos.X += movement.X * def->PositionCoinSpeedPC * frame_time;
            Held->Pos.X = std::max(Held->Pos.X, def->PositionCoinMinimum * BoardSize.X);
            Held->Pos.X = std::min(Held->Pos.X, def->PositionCoinMaximum * BoardSize.X);
            processed_input.SetAsPerformedEvent();
        }
        if (input_process->IsEventButtonPressed(event)
            && input_process->IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_YES, event)) {
            SetState(ETGS_AIM_COIN);
            MousePosEffect = C2DVector(0.0f, -0.2f);
            AverageJoystickPos = C2DVector(0.0f, -0.2f);
            Held->Vel.X = -(AverageJoystickPos.X * def->JoystickScalePCX);
            float vel_y = -(AverageJoystickPos.Y * def->JoystickScalePCY);
            Held->Vel.Y = vel_y + def->JoystickTranslateY;
            processed_input.SetAsPerformedEvent();
        }
        break;
    case ETGS_AIM_COIN:
        if (input_process->IsEventButtonPressed(event)
            && input_process->IsEventGameAction(GAME_ACTION_COIN_GOLF_CANCEL_AIM, event)) {
            SetState(ETGS_POSITION_COIN);
            processed_input.SetAsPerformedEvent();
        }
        if ((input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_MOVEMENT, event)
             && input_process->GetControlMovementFromGameActionEvent(GAME_ACTION_TAVERN_GAME_MOVEMENT, event, movement))
            || (AverageJoystickPos.Y != 0.0f && AverageJoystickPos.GetMagnitude() > def->AverageJoystickMagAbove)) {
            if (AverageJoystickPos.GetMagnitude() > def->AverageJoystickMagAbove) {
                if (!input_process->IsGameActionEventInQueue(GAME_ACTION_TAVERN_GAME_ACTION_BUTTON, (EInputButtonEventType)2, time)) {
                    PlaySoundW("TAVERN_GAME_COIN_FLICK");
                    SetState(ETGS_WATCH_COINS);
                } else {
                    Held->Vel.X = AverageJoystickPos.X * def->JoystickScalePCX;
                    float vel_y = -(AverageJoystickPos.Y * def->JoystickScalePCY);
                    Held->Vel.Y = vel_y + def->JoystickTranslateY;
                    Held->Vel.Y = std::max(Held->Vel.Y, def->JoystickTranslateY);
                }
            } else {
                Held->Vel.X = 0.0f;
                Held->Vel.Y = 0.0f;
            }
            if (input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_MOVEMENT, event)
                && input_process->IsGameActionEventInQueue(GAME_ACTION_TAVERN_GAME_ACTION_BUTTON, (EInputButtonEventType)2, time)) {
                MousePosEffect.X += movement.X;
                MousePosEffect.Y -= movement.Y;
                if (MousePosEffect.GetMagnitude() > 1.0f)
                    MousePosEffect.Normalise();
                movement = MousePosEffect;
                AverageJoystickPos = AverageJoystickPos * (1.0f - def->AverageDrift) + movement * def->AverageDrift;
                processed_input.SetAsPerformedEvent();
            }
        }
        if (input_process->IsEventButtonPressed(event)
            && input_process->IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_YES, event)) {
            if (Held->Vel.GetSquaredMagnitude() > 0.03f) {
                Held = 0;
                PlaySoundW("TAVERN_GAME_COIN_FLICK");
                SetState(ETGS_WATCH_COINS);
            }
            processed_input.SetAsPerformedEvent();
        }
        break;
    case ETGS_WATCH_COINS:
        break;
    default:
        CTCTavernGameCoinBase::ProcessInput(event, processed_input, input_process);
        break;
    }
    if (TavernGameState >= ETGS_POSITION_COIN && TavernGameState <= ETGS_WATCH_COINS) {
        if (input_process->IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_THIRD, event)
            || input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_QUIT, event)) {
            CDefPointer<const CTavernGameDef> game_def;
            GetDef(&game_def);
            SetStateAfterYesNo(GetTextWithNarrator(game_def->Quit), GetText(game_def->Yes), ETGS_QUIT_MID_GAME,
                               GetText(game_def->No), GetGameState());
            processed_input.SetAsPerformedEvent();
            UpdateCoinSounds(true);
        }
    }
}
