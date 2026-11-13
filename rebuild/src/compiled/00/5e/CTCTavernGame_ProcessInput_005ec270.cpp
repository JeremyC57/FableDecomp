// CTCTavernGame::ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*) -- the base tavern game's input
// handler: in the game state (5) a quit press asks "quit?" (SetStateAfterYesNo -> 6 / stay); on the OK prompt (44), the yes/no
// prompt (45, with the bet stick: up/down ChangeBet, left = MinBet, right = LastBet, clamped to the money and MaxBet) and the
// yes/no/alt prompt (46) a press answers it (SetState to the answer's state, prompts cleared, subtitles cleared); state 47 eats
// the input; in the wait-for-click state (13) the continue press moves on to NextTavernGameState.
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EC270, 843 bytes (+ the 6-entry dword / 43-entry byte jump tables 0x005EC578/0x005EC590),
//   `ret 0xc`. Vtable 0x012560BC slot 28. Ego_r.pdb / FableWin.pdb
//   ?ProcessInput@CTCTavernGame@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z.
// Callees: CDefClassBase::GetSubDef<CTavernGameDef> 0x005ED4A0 (via the inline GetDef), CInputProcessGameBase::IsEventButtonPressed
//   0x00687A90 / IsEventGameAction 0x00687CF0 / GetControlMovementFromGameActionEvent 0x00687D00, CInputEvent::GetType 0x00A03B40,
//   CProcessedInput::SetAsPerformedEvent 0x00A0D390, CTCTavernGame::GetText 0x005E9E70 / GetTextWithNarrator 0x005EB3A0 /
//   SetStateAfterYesNo 0x005EAF30 / ChangeBet 0x005EB810 / SetState 0x005E9B20, CSubtitleRenderer::Clear 0x00492BAB on
//   CPlayerGui::Get() (PClass 0x013B8790, SubtitleRenderer +0x18); def release through its vtable slot 1. Floats -0.2f / 0.2f.
// Shape: CDefClassBase::GetSubDef<CTavernGameDef> carries its real body here, copied verbatim from the landed row
//   rebuild/src/compiled/00/5e/CDefClassBase_GetSubDefCTavernGameDef_005ed4a0.cpp (retail's TU saw it: removing the body changes
//   the bytes); SetState 0x005E9B20 (landed as CTCTavernGame_SetState_005e9b20.cpp) is only declared, since retail calls it out
//   of line at every site here and its body has no effect on the bytes. The register choices follow the source: the case bodies
//   in the order 5, 46, 45, 44, 47; the case 5 "stay" state read through the inline FableWin.pdb GetGameState(); and each answer
//   clears the prompts written in the source order Alt, No, Yes, which produces retail's store order +0xA8 (YesState), +0xAC
//   (NoState), +0xB0 (AltState) at 0x005EC4F7..0x005EC503. The six answer tails are identical text, so /O2 tail-merges them.
// Names: ETavernGameStates / EGameAction values stay numeric (enumerator names unproven).
// Leaf: ?ProcessInput@CTCTavernGame@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_CLEAR
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_INPUTPROCESS_TAVERNGAME_FRIEND
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#include "engine/CTavernGameDef.h"
#include "engine/CDefinitionManager.h"
template <>
bool CDefClassBase::GetSubDef<CTavernGameDef>(CDefPointer<const CTavernGameDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernGameDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#include "engine/CTCTavernGame.h"
#include "engine/CPlayerGui.h"
#include "engine/C2DVector.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessTavernGame.h"

void CTCTavernGame::ProcessInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessTavernGame* input)
{
    CDefPointer<const CTavernGameDef> def;
    GetDef(&def);
    switch (TavernGameState) {
    case 5:
        if (input->IsEventButtonPressed(event) &&
            (input->IsEventGameAction((EGameAction)0x18, event) || input->IsEventGameAction((EGameAction)0x53, event))) {
            GameFinished = true;
            GameWon = false;
            SetStateAfterYesNo(GetTextWithNarrator(def->Quit), GetText(def->Yes), (ETavernGameStates)6,
                               GetText(def->No), GetGameState());
            processed.SetAsPerformedEvent();
        }
        break;
    case 0x2e:
        if (input->IsEventButtonPressed(event)) {
            if (input->IsEventGameAction((EGameAction)0x16, event)) {
                SetState(YesState);
                AltState = (ETavernGameStates)0; NoState = (ETavernGameStates)0; YesState = (ETavernGameStates)0;
                CPlayerGui::Get().GetSubtitleRenderer().Clear(false);
                processed.SetAsPerformedEvent();
            } else if (input->IsEventGameAction((EGameAction)0x17, event)) {
                SetState(NoState);
                AltState = (ETavernGameStates)0; NoState = (ETavernGameStates)0; YesState = (ETavernGameStates)0;
                CPlayerGui::Get().GetSubtitleRenderer().Clear(false);
                processed.SetAsPerformedEvent();
            } else if (input->IsEventGameAction((EGameAction)0x18, event)) {
                SetState(AltState);
                AltState = (ETavernGameStates)0; NoState = (ETavernGameStates)0; YesState = (ETavernGameStates)0;
                CPlayerGui::Get().GetSubtitleRenderer().Clear(false);
                processed.SetAsPerformedEvent();
            }
        }
        break;
    case 0x2d:
        if (YesState == 5 && input->IsEventGameAction((EGameAction)0x72, event) && event.GetType() == 2) {
            C2DVector stick;
            input->GetControlMovementFromGameActionEvent((EGameAction)0x72, event, stick);
            if (stick.Y < -0.2f)
                ChangeBet(false);
            else if (stick.Y > 0.2f)
                ChangeBet(true);
            if (stick.X < -0.2f)
                BaseBet = def->MinBet;
            else if (stick.X > 0.2f)
                BaseBet = LastBet;
            if (BaseBet > CurrentMoney)
                BaseBet = CurrentMoney ? CurrentMoney : def->MinBet;
            if (BaseBet > def->MaxBet)
                BaseBet = def->MaxBet;
            processed.SetAsPerformedEvent();
        }
        if (input->IsEventButtonPressed(event)) {
            if ((BaseBet != 0 || YesState != 5) && input->IsEventGameAction((EGameAction)0x16, event)) {
                SetState(YesState);
                AltState = (ETavernGameStates)0; NoState = (ETavernGameStates)0; YesState = (ETavernGameStates)0;
                CPlayerGui::Get().GetSubtitleRenderer().Clear(false);
                processed.SetAsPerformedEvent();
            } else if (input->IsEventGameAction((EGameAction)0x17, event)) {
                SetState(NoState);
                AltState = (ETavernGameStates)0; NoState = (ETavernGameStates)0; YesState = (ETavernGameStates)0;
                CPlayerGui::Get().GetSubtitleRenderer().Clear(false);
                processed.SetAsPerformedEvent();
            }
        }
        break;
    case 0x2c:
        if (input->IsEventButtonPressed(event) &&
            (input->IsEventGameAction((EGameAction)0x14, event) || input->IsEventGameAction((EGameAction)6, event))) {
            SetState(YesState);
            AltState = (ETavernGameStates)0; NoState = (ETavernGameStates)0; YesState = (ETavernGameStates)0;
            CPlayerGui::Get().GetSubtitleRenderer().Clear(false);
            processed.SetAsPerformedEvent();
        }
        break;
    case 0x2f:
        processed.SetAsPerformedEvent();
        break;
    }
    if (TavernGameState == 0xd && input->IsEventButtonPressed(event) && input->IsEventGameAction((EGameAction)0x16, event)) {
        SetState(NextTavernGameState);
        processed.SetAsPerformedEvent();
    }
}