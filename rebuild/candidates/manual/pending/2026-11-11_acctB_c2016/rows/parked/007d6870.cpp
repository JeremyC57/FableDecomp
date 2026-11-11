// CTCTavernGameCardPairs::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input, CInputProcessTavernGame* input_process) -- pairs input: while the TU's switch is on, the card base handles everything; otherwise the main-game state selects a card (or ends the round with the win / lose message) and offers the quit question, other states go to the base game, and the movement stick moves the pointer to the nearest card in the stick's direction
// Grade: DIFFER(1778v1778) -- 540/571 instructions equal with frame offsets masked: frame 0x30 v 0x38 (the |dy| GFFloatAbs copy and dy share the WIN / PICK string-temporary slots; retail gives each its own word, [esp+10h] / [esp+24h]); the x87 operand order of the inline GetRight cross product (0x007D6BFC, 0x007D6C15..0x007D6C1D) and of the first offset Dot (0x007D6C3D..0x007D6C4D); the final (position - best).Dot(position - highlighted) product order (retail X, Z, then Y: 0x007D6ED1..0x007D6EDB); the eax / edx choice of the two inline def releases (0x007D6F0B / 0x007D6F53, eax / edx swapped)
// Leaf: ?ProcessInput@CTCTavernGameCardPairs@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D6870, 1778 bytes (last `ret 0Ch` at 0x007D6F5F, int3 padding to 0x007D6F70). Identity (two sources):
//   CTCTavernGameCardPairs vtable 0x0126F6FC slot 28 (0x0126F76C, the only reference; catalog "vfunc_28"), the slot every CTCTavernGame
//   subclass fills with ProcessInput (CTCTavernGame 0x005EC270, CTCTavernGameCardBase / CardSorting 0x008F12D0, ShoveHaPenny 0x007CF9A0,
//   SpotTheAddition 0x007D8570); Ego_r.pdb 0x007D3BC0 / FableWin.pdb 0x02749060
//   ?ProcessInput@CTCTavernGameCardPairs@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z.
// Callees: CTCTavernGameCardBase::ProcessInput 0x008F12D0 / GetCardDef 0x008F0B20, CTCTavernGame::ProcessInput 0x005EC270 / GetDef 0x005EA000 /
//   PlaySoundW 0x005EA950 / GetText 0x005E9E70 / GetTextWithNarrator 0x005EB3A0 / SetStateAfterOK 0x005EB080 / SetStateAfterYesNo 0x005EAF30 /
//   PositionMesh 0x005EABC0, the slot-59 virtual SelectCard (+0xEC), CCard::Turn 0x008F0620, CInputProcessGameBase::IsEventButtonPressed
//   0x00687A90 / IsEventGameAction 0x00687CF0 / GetControlMovementFromGameActionEvent 0x00687D00, CProcessedInput::SetAsPerformedEvent
//   0x00A0D390, CSubtitleRenderer::Clear 0x00492BAB on CPlayerGui::Get() (0x013B8790), CThing::GetPos 0x004C73D0, CWorld::GetFrame 0x0049D870,
//   CCharString 0x0099EBF0 / ~ 0x0099EAE0. Data: the TU's switch 0x01383C3F (a static bool initialised true, never written: descriptive
//   name), DELTA_NOTIONAL_ZERO 0x0129BA3C, 0.0f 0x0122DEDC.
// Layout: hdr/engine/CTCTavernGameCardBase.h / CTCTavernGameCardPairs.h / CCard.h (new; Ego_r.pdb members at the retail offsets) and the
//   CTavernGameCardBaseDef retail branch (ControlType +0x28, ImpulseScalePC +0x38, PointerHeightOffset +0x48).
// Shape so far: switch with `default: base; return;`; `C2DVector movement;` at function scope (with it block-scoped the frame is 0x2C);
//   implicit const char* -> CCharString PlaySoundW arguments (named locals cross-jump the WIN / LOSE calls); the phrase id a separate
//   statement before SetStateAfterOK; `impulse_scale` a named float (retail loads ImpulseScalePC once, 0x007D6BF3); `best_distance >
//   distance` (retail `fld [best] ; fcomp st(1)`); the plain C3DVector::Dot (DOT_GROUPED misorders the first two dots).
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
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_RIGHT_COMPONENTS
#define FABLE_RETAIL_INPUTPROCESS
#define FABLE_RETAIL_INPUTPROCESS_ZTARGET
#define FABLE_RETAIL_INPUTPROCESS_MOVEMENT
#define FABLE_RETAIL_CINPUTPROCESSTAVERNGAME_PRESSED_ACCESS
#define FABLE_RETAIL_CPROCESSEDINPUT_PERFORMED
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CTHINGOBJECT
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CTCTavernGame.h"
#include "engine/CTCTavernGameCardBase.h"
#include "engine/CTCTavernGameCardPairs.h"
#include "engine/CPlayerGui.h"
#include "engine/CWorld.h"
#include "fable_game.h"
#include "engine/EGameAction.h"
#include "engine/CInputEvent.h"
#include "engine/CProcessedInput.h"
#include "engine/CInputProcessGameBase.h"
#include "engine/CInputProcessTavernGame.h"

inline float __fastcall GFFloatAbs(float f)
{
    *(unsigned long*)&f &= 0x7fffffff;
    return f;
}

static bool UseCardBaseInput = true;                                  // 0x01383C3F

void CTCTavernGameCardPairs::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input,
                                          CInputProcessTavernGame* input_process)
{
    C2DVector movement;
    if (UseCardBaseInput) {
        CTCTavernGameCardBase::ProcessInput(event, processed_input, input_process);
        return;
    }
    switch (TavernGameState) {
    case ETGS_CARDS_TURNING_UP:
    case ETGS_CARDS_TURNING_BACK:
        break;
    case ETGS_CARDS_MAIN_GAME:
        if (input_process->IsEventButtonPressed(event)) {
            if (input_process->IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_YES, event)) {
                if (!SelectCard()) {
                    CDefPointer<const CTavernGameDef> game_def;
                    GetDef(&game_def);
                    if (GameWon)
                        PlaySoundW("TAVERN_GAME_WIN_GAME");
                    else
                        PlaySoundW("TAVERN_GAME_LOSE_GAME");
                    unsigned long phrase = GameWon ? game_def->WinPhrase : game_def->LosePhrase;
                    SetStateAfterOK(GetText(phrase), GetText(game_def->Continue), ETGS_TURN_CARDS_BACK);
                } else if (SelectedCard && !SelectedCard->FaceDown && !SelectedCard->Turning) {
                    PlaySoundW("TAVERN_GAME_CARD_PICK");
                    PlaySoundW("TAVERN_GAME_CARD_FLIP");
                    SelectedCard->Turn();
                }
                processed_input.SetAsPerformedEvent();
            }
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
        break;
    default:
        CTCTavernGame::ProcessInput(event, processed_input, input_process);
        return;
    }
    if (input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_MOVEMENT, event)) {
        input_process->GetControlMovementFromGameActionEvent(GAME_ACTION_TAVERN_GAME_MOVEMENT, event, movement);
        if (movement.X == 0.0f && movement.Y == 0.0f) {
            NumFramesRepeated = 0;
            return;
        }
        CDefPointer<const CTavernGameCardBaseDef> def;
        GetCardDef(&def);
        C3DVector offset;
        switch (def->ControlType) {
        case ETGCT_RELATIVE:
            offset = ((CThing*)Pointer)->GetPos() - GamePositionPos;
            break;
        case ETGCT_ABSOLUTE:
            offset = C3DVector(0.0f, 0.0f, def->PointerHeightOffset);
            break;
        }
        float impulse_scale = def->ImpulseScalePC;
        C3DVector position;
        position.X = offset.Dot(GamePositionRHSet.GetRight()) + movement.X * impulse_scale;
        position.Y = offset.Dot(GamePositionRHSet.GetForward()) - movement.Y * impulse_scale;
        position.Z = 0.0f;
        float dx = (HighlightedCard->LocalPos.X - position.X) / HighlightedCard->Width;
        float dy = (HighlightedCard->LocalPos.Y - position.Y) / HighlightedCard->Height;
        long direction;
        if (GFFloatAbs(dx) > GFFloatAbs(dy)) {
            if (dx > 0.0f)
                direction = 0;
            else
                direction = 1;
        } else
            direction = dy > 0.0f ? 2 : 3;
        if (PreviousDirection != direction)
            NumFramesRepeated = 0;
        PreviousDirection = direction;
        LastFramePointerMoved = CWorld::GetFrame();
        if (position.X < MinX)
            position.X = MinX;
        else if (position.X > MaxX)
            position.X = MaxX;
        if (position.Y < MinY)
            position.Y = MinY;
        else if (position.Y > MaxY)
            position.Y = MaxY;
        float best_distance = 0.0f;
        CCard* best = 0;
        for (std::vector<CCard*>::iterator it = Cards.begin(); it != Cards.end(); ++it) {
            CCard* card = *it;
            if (card == HighlightedCard)
                continue;
            float card_dx = (card->LocalPos.X - position.X) / card->Width;
            float card_dy = (card->LocalPos.Y - position.Y) / card->Height;
            float distance = card_dx * card_dx + card_dy * card_dy;
            if (best_distance == 0.0f || best_distance > distance) {
                best_distance = distance;
                best = card;
            }
        }
        if ((GFFloatAbs(HighlightedCard->LocalPos.X - best->LocalPos.X) < DELTA_NOTIONAL_ZERO && GFFloatAbs(movement.Y) < DELTA_NOTIONAL_ZERO)
            || (GFFloatAbs(HighlightedCard->LocalPos.Y - best->LocalPos.Y) < DELTA_NOTIONAL_ZERO && GFFloatAbs(movement.X) < DELTA_NOTIONAL_ZERO)) {
        } else {
            if ((position - HighlightedCard->LocalPos).Dot(position - best->LocalPos) > 0.0f)
                return;
            PositionMesh((CThing*)Pointer, GamePositionPos, GamePositionRHSet, best->LocalPos, 0.0f, 0.0f);
            processed_input.SetAsPerformedEvent();
        }
    }
}
