// CTCTavernGameCoinGolf::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input, CInputProcessTavernGame* input_process) -- coin-golf input: while the action button is not held the camera stick turns (ZAngle) and tilts (XAngle, clamped to [0, 1]) the table camera; in the aim state the action button resets the aim (joystick average and mouse effect to (0, -0.2)) and points the held coin's velocity along the camera's heading, the alternate button zeroes the aim, movement with the action button held blends the (clamped, squared-off) mouse effect into the joystick average and re-aims the coin, and letting go with enough velocity flicks the coin (WATCH_COINS); other states go to the base game; while aiming or watching the quit buttons ask to leave
// Leaf: ?ProcessInput@CTCTavernGameCoinGolf@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z
// Flags: /d2inlT87
// Inline-budget: /d2inlT87 -- std::max<float> / std::min<float> at 0x007D3930 / 0x007D394A (the XAngle clamp of the camera block) are
//   expanded in retail; VC7.1's c2 inline trace (c2.dll 0x1078D1AA, tools/inl) gives both sites c1 16 at static frequency 0.03125 (the
//   five tests !held && IsEventGameAction && GetControlMovement && magnitude > 0.04 && GetCameraManager), score 86, so the default
//   threshold 74 calls them out of line (grade 1: DIFFER 1964v1883, two `call` + frame 0x50). T87 is the smallest threshold that expands
//   them; no other site of the TU moves (GetSubDef<CTavernGameCoinGolfDef> stays called at score 174, every other site is already
//   expanded). Traced control-flow alternatives (nested `if (!held) if (...)`, the magnitude and camera tests in one parenthesised
//   group, the camera test as its own if) all keep f 0.03125 / score 86. The sibling CTCTavernGameShoveHaPenny::ProcessInput 0x007CF9A0
//   needed /d2inlT116 for its own std::max site (pending/2026-11-11_acctB_c2007).
// Retail: Fable.exe 0x007D3820, 1883 bytes (first instruction `sub esp,44h`; the function ends with the `jmp 0x007D3DE2` of the flick
//   path at 0x007D3F76, the cold block after `ret 0Ch` at 0x007D3ECC; the next function, OnInitialActivate, starts at 0x007D3F80).
//   Vtable 0x0126F49C slot 28 (0x0126F50C); Ego_r.pdb / FableWin.pdb
//   ?ProcessInput@CTCTavernGameCoinGolf@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z (catalog label vfunc_28).
// Switch on TavernGameState (sub / dec chain, no jump table): 16 ETGS_AIM_COIN 0x007D3997, 17 ETGS_WATCH_COINS break, default
//   CTCTavernGame::ProcessInput 0x005EC270.
// Callees: GetSubDef<CTavernGameCoinGolfDef> 0x007D4BD0 (this TU, out of line); CInputProcessGameBase::IsGameActionEventInQueue
//   0x00687D80, IsEventGameAction 0x00687CF0, GetControlMovementFromGameActionEvent 0x00687D00, IsEventButtonPressed 0x00687A90;
//   CTCBase::GetWorld 0x00686A70 then the current game camera's slot 9 GetCameraManager(CGameCameraManager**); CProcessedInput::
//   SetAsPerformedEvent 0x00A0D390; GetCamera 0x007D3560 (this TU, out of line); C3DVector::AccurateNormalise 0x00A14440; _CIfmod
//   0x00BFEB28 (the expanded GetAngleXY); C2DVector::Rotate 0x00A141D0, Normalise 0x00A14540, AccurateNormalise 0x00A14510;
//   CTCTavernGame::PlaySoundW 0x005EA950 ("TAVERN_GAME_COIN_FLICK" 0x0126F1B0), SetState 0x005E9B20, GetDef 0x005EA000, GetText
//   0x005E9E70, GetTextWithNarrator 0x005EB3A0, SetStateAfterYesNo 0x005EAF30; CTCTavernGameCoinBase::UpdateCoinSounds(bool) 0x008EF910;
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Shape: the aim reset is two chained member stores (`MousePosEffect.X = AverageJoystickPos.X = 0.0f`: retail stores +0x160 then +0x13C
//   from one GPR and reuses the known 0.0 for the velocity's X product, `fmul [0.0]` at 0x007D39E1); the alternate button's
//   Held->Vel.X = Held->Vel.Y = 0.0f (Y first, Held reloaded); the camera direction is the look-at vector itself (`look_at -= cam_pos`,
//   Z zeroed; retail rewrites the look-at slot, 0x007D3A0D); the camera turn's magnitude is a named float (retail keeps it on the x87
//   stack and pops it after the Y product, 0x007D3905); the camera-manager pointer is declared in its own block (its frame word is shared
//   with later temporaries, [esp+1Ch]); the release flick re-tests `!held && |Vel| >= JoystickMagBelow` inside the combined condition
//   (retail recomputes the magnitude, 0x007D3EF9); `MousePosEffect.Y == -0.2f` compiles to the integer compare at 0x007D3F32.
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_ROTATE_ANGLES
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_ROTATE
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_INPUTPROCESS_EVENTINQUEUE
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_PRESSED_ACCESS
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_EVENTINQUEUE_ACCESS
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#define FABLE_RETAIL_CTAVERNGAMECOINGOLFDEF_SUBDEF
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCoinGolfDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCoinBase.h"
#include "engine/CTCTavernGameCoinGolf.h"
#include "engine/ThingAlive.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "fable_game.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessTavernGame.h"
#include <math.h>
class CGameCameraManager;

template <>
bool CDefClassBase::GetSubDef<CTavernGameCoinGolfDef>(CDefPointer<const CTavernGameCoinGolfDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernGameCoinGolfDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

#ifdef _MSC_VER
__declspec(noinline)
#endif
void CTCTavernGameCoinGolf::GetCamera(C3DVector& camPos, C3DVector& lookAt)
{
    C3DVector centre(BoardPos);
    C3DVector offset(0.0f, 0.05f, 1.75f);
    offset.RotateXY(ZAngle);
    offset += BoardPos;
    camPos = offset;
    lookAt = centre;
    if (Held && IS_THING_ALIVE(Held->Thing)) {
        lookAt = Held->Thing.Get()->GetPos();
        lookAt.Z += 0.1f;
        camPos = C3DVector(0.0f, XAngle * 0.9f + 0.25f, 0.0f);
        camPos.Rotate(0.0f, XAngle * 0.2f + 0.025f);
        camPos.Rotate(ZAngle, 0.0f);
        camPos += lookAt;
    }
}

void CTCTavernGameCoinGolf::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input,
                                         CInputProcessTavernGame* input_process)
{
    CDefPointer<const CTavernGameCoinGolfDef> def;
    ParentThing->PeekBaseDef().GetSubDef(&def);
    C2DVector movement;
    float time;
    bool action_held = input_process->IsGameActionEventInQueue(GAME_ACTION_TAVERN_GAME_ACTION_BUTTON, (EInputButtonEventType)2, time);
    if (!action_held
        && input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_CAMERA, event)
        && input_process->GetControlMovementFromGameActionEvent(GAME_ACTION_TAVERN_GAME_CAMERA, event, movement)
        && movement.GetMagnitude() > 0.04) {
        CGameCameraManager* camera_manager;
        if (GetWorld().GetGameCamera()->GetCameraManager(&camera_manager)) {
            float magnitude = movement.GetMagnitude();
            movement *= magnitude;
            ZAngle += movement.X * 0.04f;
            XAngle -= movement.Y * 0.08f;
            XAngle = std::max(XAngle, 0.0f);
            XAngle = std::min(XAngle, 1.0f);
            processed_input.SetAsPerformedEvent();
        }
    }
    switch (TavernGameState) {
    case ETGS_AIM_COIN:
        if (input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_ACTION_BUTTON, event)
            && input_process->IsEventButtonPressed(event)) {
            MousePosEffect.X = AverageJoystickPos.X = 0.0f;
            MousePosEffect.Y = AverageJoystickPos.Y = -0.2f;
            Held->Vel.X = AverageJoystickPos.X * def->JoystickScaleX;
            Held->Vel.Y = AverageJoystickPos.Y * def->JoystickScaleY;
            C3DVector cam_pos;
            C3DVector look_at;
            GetCamera(cam_pos, look_at);
            look_at -= cam_pos;
            look_at.Z = 0.0f;
            look_at.AccurateNormalise();
            float angle = look_at.GetAngleXY();
            Held->Vel.Rotate(angle - BoardRHSet.GetForward().GetAngleXY() + 0.5f);
        }
        if (input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_ALTERNATE_BUTTON, event)
            && input_process->IsEventButtonPressed(event)) {
            AverageJoystickPos.X = 0.0f;
            AverageJoystickPos.Y = 0.0f;
            Held->Vel.X = Held->Vel.Y = 0.0f;
        }
        if ((action_held
             && input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_MOVEMENT, event)
             && input_process->GetControlMovementFromGameActionEvent(GAME_ACTION_TAVERN_GAME_MOVEMENT, event, movement))
            || (!action_held && Held->Vel.GetMagnitude() >= def->JoystickMagBelow)) {
            if (!action_held && Held->Vel.GetMagnitude() >= def->JoystickMagBelow) {
                if (MousePosEffect.X == 0.0f && MousePosEffect.Y == -0.2f)
                    MousePosEffect.Y = 0.0f;
                PlaySoundW("TAVERN_GAME_COIN_FLICK");
                SetState(ETGS_WATCH_COINS);
            } else {
                MousePosEffect -= movement;
                if (MousePosEffect.GetMagnitude() > def->MaxPower) {
                    MousePosEffect.Normalise();
                    MousePosEffect *= def->MaxPower;
                }
                movement = MousePosEffect;
                if (movement.GetMagnitude() > 0.001) {
                    C2DVector direction = movement;
                    direction.AccurateNormalise();
                    movement *= std::max(fabs(direction.X), fabs(direction.Y));
                }
                AverageJoystickPos = AverageJoystickPos * (1.0f - def->AverageDrift) + movement * def->AverageDrift;
                Held->Vel.X = AverageJoystickPos.X * def->JoystickScaleX;
                Held->Vel.Y = AverageJoystickPos.Y * def->JoystickScaleY;
                C3DVector cam_pos;
                C3DVector look_at;
                GetCamera(cam_pos, look_at);
                look_at -= cam_pos;
                look_at.Z = 0.0f;
                look_at.AccurateNormalise();
                float angle = look_at.GetAngleXY();
                Held->Vel.Rotate(angle - BoardRHSet.GetForward().GetAngleXY() + 0.5f);
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
    if (TavernGameState >= ETGS_AIM_COIN && TavernGameState <= ETGS_WATCH_COINS) {
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
