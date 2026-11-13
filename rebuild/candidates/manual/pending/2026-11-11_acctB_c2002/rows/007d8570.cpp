// CTCTavernGameSpotTheAddition::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input, CInputProcessTavernGame* input_process) -- spot-the-addition input: the watch state skips to the add state, the select state moves the pointer over the board with the movement stick (clamped to the board bounds) and judges the picked knick-knack (win / correct / wrong, highlighting the new one), other states go to the base game; while a round runs the quit buttons ask to leave
// Leaf: ?ProcessInput@CTCTavernGameSpotTheAddition@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z
// Retail: Fable.exe 0x007D8570, 1340 bytes: code to `ret 0Ch` at 0x007D8A96, `lea ecx,[ecx]` pad, then the 4-entry jump table 0x007D8A9C
//   over TavernGameState - 0x27: 0x27 / 0x28 -> 0x007D89A5 (break), 0x29 -> 0x007D85AE, 0x2A -> 0x007D85C5; above 0x2A -> 0x007D898F
//   (default: CTCTavernGame::ProcessInput).
// Shape: `offset = Pos - GamePositionPos;` as an assignment (retail keeps the operator- temporary, 0x007D8609..0x007D863C), the clamped
//   position written field by field (X first), GetGameState() as SetStateAfterYesNo's last argument (one rotating temporary fewer:
//   TavernGameState in ecx at 0x007D8A09), the three SetStateAfterOK calls cross-jumped into 0x007D8909 by the compiler.
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
#define FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
#define FABLE_RETAIL_CTCTAVERNGAME_GETGAMESTATE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_CLEAR
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_DOT_GROUPED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_GETRIGHT_OUTOFLINE
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_PRESSED_ACCESS
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameSpotTheAdditionDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameSpotTheAddition.h"
#include "engine/CPlayerGui.h"
#include "engine/CWorld.h"
#include "fable_game.h"
#include "engine/CRGBColour.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessTavernGame.h"

namespace NGraphicAppearance {
enum EAppearanceMeshEffectPriority { };
class CTCGraphicAppearance { public:
    void SetAsHighlighted(float, float, const CRGBColour&, bool, bool, EAppearanceMeshEffectPriority, void*);   // 0x004C04C0
};
}

// GetSubDef<CTavernGameSpotTheAdditionDef> 0x007D9A80 (landed): defined in this TU as in retail's, so VC7.1 keeps it out of line
// and sees that it does not capture the def slot (the def pointer stays in ebp for the whole function).
template <>
bool CDefClassBase::GetSubDef<CTavernGameSpotTheAdditionDef>(CDefPointer<const CTavernGameSpotTheAdditionDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernGameSpotTheAdditionDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

void CTCTavernGameSpotTheAddition::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input,
                                                CInputProcessTavernGame* input_process)
{
    CDefPointer<const CTavernGameSpotTheAdditionDef> def;
    ParentThing->PeekBaseDef().GetSubDef(&def);
    switch (TavernGameState) {
    case ETGS_ADDITION_SETUP:
    case ETGS_ADDITION_ADD:
        break;
    case ETGS_ADDITION_WATCH:
        SetState(ETGS_ADDITION_ADD);
        processed_input.SetAsPerformedEvent();
        break;
    case ETGS_ADDITION_SELECT:
        if (input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_MOVEMENT, event)) {
            C2DVector movement;
            input_process->GetControlMovementFromGameActionEvent(GAME_ACTION_TAVERN_GAME_MOVEMENT, event, movement);
            C3DVector offset;
            offset = ((CThing*)Pointer)->GetPos() - GamePositionPos;
            C3DVector position;
            position.X = offset.Dot(GamePositionRHSet.GetRight()) + movement.X * def->ImpulseScale;
            position.Y = offset.Dot(GamePositionRHSet.GetForward()) + movement.Y * def->ImpulseScale;
            position.Z = 0.0f;
            if (position.X < MinX)
                position.X = MinX;
            else if (position.X > MaxX)
                position.X = MaxX;
            if (position.Y < MinY)
                position.Y = MinY;
            else if (position.Y > MaxY)
                position.Y = MaxY;
            PositionMesh((CThing*)Pointer, GamePositionPos, GamePositionRHSet, position, 0.0f, 0.0f);
            processed_input.SetAsPerformedEvent();
        }
        if (FindTarget(100000.0f) && input_process->IsEventButtonPressed(event)
            && input_process->IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_YES, event)) {
            CDefPointer<const CTavernGameDef> game_def;
            GetDef(&game_def);
            if (FindTarget(100000.0f) != NewKnickKnack) {
                PlaySoundW("TAVERN_GAME_LOSE_GAME");
                CThingObject* target = FindTarget(100000.0f);
                if (target) {
                    NGraphicAppearance::CTCGraphicAppearance* appearance = 0;
                    ((CThing*)target)->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &appearance);
                    appearance->SetAsHighlighted(def->HighlightWidth, 0.0f, def->HighlightColourError, false, false,
                                                 (NGraphicAppearance::EAppearanceMeshEffectPriority)3, this);
                }
                SetStateAfterOK(GetText(game_def->LosePhrase), GetText(game_def->Continue), ETGS_END_ROUND);
            } else if (CurrentRound + 1 == def->ItemsPerRound.size()) {
                GameWon = true;
                PlaySoundW("TAVERN_GAME_WIN_GAME");
                SetStateAfterOK(GetText(game_def->WinPhrase), GetText(game_def->Continue), ETGS_END_ROUND);
            } else {
                PlaySoundW("TAVERN_GAME_CORRECT");
                SetStateAfterOK(GetTextWithNarrator(game_def->WinRoundPhrase), GetText(game_def->Continue), ETGS_ADDITION_SETUP);
            }
            if (NewKnickKnack) {
                NGraphicAppearance::CTCGraphicAppearance* appearance = 0;
                ((CThing*)NewKnickKnack)->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &appearance);
                appearance->SetAsHighlighted(def->HighlightWidth, 0.0f, def->HighlightColour, false, false,
                                             (NGraphicAppearance::EAppearanceMeshEffectPriority)3, this);
            }
            processed_input.SetAsPerformedEvent();
        }
        break;
    default:
        CTCTavernGame::ProcessInput(event, processed_input, input_process);
        break;
    }
    if (TavernGameState >= ETGS_ADDITION_SETUP && TavernGameState <= ETGS_ADDITION_SELECT) {
        if (input_process->IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_THIRD, event)
            || input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_QUIT, event)) {
            CPlayerGui::Get().GetSubtitleRenderer().Clear(false);
            CDefPointer<const CTavernGameDef> game_def;
            GetDef(&game_def);
            SetStateAfterYesNo(GetTextWithNarrator(game_def->Quit), GetText(game_def->Yes), ETGS_QUIT_MID_GAME,
                               GetText(game_def->No), GetGameState());
            processed_input.SetAsPerformedEvent();
        }
    }
}
