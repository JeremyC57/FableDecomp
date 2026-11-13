// CTCTavernGameCardPairs::ProcessInput(const CInputEvent& event, CProcessedInput& processed_input, CInputProcessTavernGame* input_process) -- pairs input: while the TU's switch is on, the card base handles everything; otherwise the main-game state selects a card (or ends the round with the win / lose message) and offers the quit question, other states go to the base game, and the movement stick moves the pointer to the nearest card in the stick's direction
// Grade: RELOCATION_MATCH (1778v1778, 571/571 instructions equal unmasked; continuation session 2 of chunk 3018)
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
// Shape: the movement block sits INSIDE the switch -- `case ETGS_CARDS_MAIN_GAME:` (input handling) falls through into
//   `case ETGS_CARDS_TURNING_UP: case ETGS_CARDS_TURNING_BACK:` (movement), `default:` calls the base game (retail's 0x38 frame);
//   `if (aligned) break;` then `if (dot > 0.0f) return;` gives retail's two def-release copies; `C2DVector movement;` at function
//   scope; implicit const char* -> CCharString PlaySoundW arguments; GetRight is the default `Forward ^ Up`.
//   x87 / multiply operand orders (GetRight cross product 0x007D6C15..0x007D6C2B, the first dot 0x007D6C2F.., the final dot
//   0x007D6ED1..0x007D6EDB) are c2 tree-linearizer ties keyed on c2 record ids, i.e. on the amount of IL in the function: they come
//   out with the step-by-step spelling below (named intermediate values for the local-frame dots, the pointer offset, the highlighted
//   card, the loop card and the alignment test's movement components; `position.Z` takes the up component and is then flattened to 0).
//   FITTED, not proven: which of these named values the original had (NOTES.md "Session 2").
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
    switch (GetGameState()) {
    case ETGS_CARDS_MAIN_GAME:
        if (input_process->IsEventButtonPressed(event)) {
            bool yes_pressed = input_process->IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_YES, event);
            if (yes_pressed) {
                bool selected = SelectCard();
                if (!selected) {
                    CDefPointer<const CTavernGameDef> game_def;
                    GetDef(&game_def);
                    if (GameWon)
                        PlaySoundW("TAVERN_GAME_WIN_GAME");
                    else
                        PlaySoundW("TAVERN_GAME_LOSE_GAME");
                    const CTavernGameDef* game = game_def.Get();
                    unsigned long phrase = GameWon ? game->WinPhrase : game->LosePhrase;
                    SetStateAfterOK(GetText(phrase), GetText(game->Continue), ETGS_TURN_CARDS_BACK);
                } else if (SelectedCard && !SelectedCard->FaceDown && !SelectedCard->Turning) {
                    PlaySoundW("TAVERN_GAME_CARD_PICK");
                    PlaySoundW("TAVERN_GAME_CARD_FLIP");
                    SelectedCard->Turn();
                }
                processed_input.SetAsPerformedEvent();
            }
            if (input_process->IsEventGameAction(GAME_ACTION_ANSWER_QUESTION_THIRD, event)
                || input_process->IsEventGameAction(GAME_ACTION_TAVERN_GAME_QUIT, event)) {
                CPlayerGui& player_gui = CPlayerGui::Get();
                CSubtitleRenderer& subtitle_renderer = player_gui.GetSubtitleRenderer();
                subtitle_renderer.Clear(false);
                CDefPointer<const CTavernGameDef> game_def;
                GetDef(&game_def);
                ETavernGameStates state = GetGameState();
                SetStateAfterYesNo(GetTextWithNarrator(game_def->Quit), GetText(game_def->Yes), ETGS_QUIT_MID_GAME,
                                   GetText(game_def->No), state);
                processed_input.SetAsPerformedEvent();
            }
        }
    case ETGS_CARDS_TURNING_UP:
    case ETGS_CARDS_TURNING_BACK:
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
                {
                    CThing* pointer = (CThing*)Pointer;
                    const C3DVector& pointer_pos = pointer->GetPos();
                    offset = pointer_pos - GamePositionPos;
                }
                break;
            case ETGCT_ABSOLUTE:
                {
                    float height = def->PointerHeightOffset;
                    offset = C3DVector(0.0f, 0.0f, height);
                }
                break;
            }
            float impulse_scale = def->ImpulseScalePC;
            C3DVector position;
            const C3DVector& forward = GamePositionRHSet.GetForward();
            C3DVector right = GamePositionRHSet.GetRight();
            float right_dot = offset.Dot(right);
            float forward_dot = offset.Dot(forward);
            float move_x = movement.X * impulse_scale;
            float move_y = movement.Y * impulse_scale;
            position.X = right_dot + move_x;
            position.Y = forward_dot - move_y;
            const C3DVector& up = GamePositionRHSet.GetUp();
            float up_dot = offset.Dot(up);
            position.Z = up_dot;
            position.Z = 0.0f;
            CCard* highlighted = HighlightedCard;
            const C3DVector& highlighted_pos = highlighted->LocalPos;
            float highlighted_width = highlighted->Width;
            float highlighted_height = highlighted->Height;
            float dx = (highlighted_pos.X - position.X) / highlighted_width;
            float dy = (highlighted_pos.Y - position.Y) / highlighted_height;
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
            long frame = CWorld::GetFrame();
            LastFramePointerMoved = frame;
            if (position.X < MinX)
                position.X = MinX;
            else if (position.X > MaxX)
                position.X = MaxX;
            if (position.Y < MinY)
                position.Y = MinY;
            else if (position.Y > MaxY)
                position.Y = MaxY;
            CCard* best = 0;
            float best_distance = 0.0f;
            for (std::vector<CCard*>::iterator it = Cards.begin(); it != Cards.end(); ++it) {
                CCard* card = *it;
                if (card != HighlightedCard) {
                    const C3DVector& card_pos = card->LocalPos;
                    float card_width = card->Width;
                    float card_height = card->Height;
                    float card_dx = (card_pos.X - position.X) / card_width;
                    float card_dy = (card_pos.Y - position.Y) / card_height;
                    float distance = card_dx * card_dx + card_dy * card_dy;
                    if (best_distance == 0.0f || best_distance > distance) {
                        best_distance = distance;
                        best = card;
                    }
                }
            }
            float movement_x = movement.X;
            float movement_y = movement.Y;
            bool aligned = (GFFloatAbs(HighlightedCard->LocalPos.X - best->LocalPos.X) < DELTA_NOTIONAL_ZERO && GFFloatAbs(movement_y) < DELTA_NOTIONAL_ZERO)
                || (GFFloatAbs(HighlightedCard->LocalPos.Y - best->LocalPos.Y) < DELTA_NOTIONAL_ZERO && GFFloatAbs(movement_x) < DELTA_NOTIONAL_ZERO);
            if (aligned)
                break;
            const C3DVector& best_pos = best->LocalPos;
            float dot = (position - HighlightedCard->LocalPos).Dot(position - best_pos);
            if (dot > 0.0f)
                return;
            PositionMesh((CThing*)Pointer, GamePositionPos, GamePositionRHSet, best->LocalPos, 0.0f, 0.0f);
            processed_input.SetAsPerformedEvent();
        }
        break;
    default:
        CTCTavernGame::ProcessInput(event, processed_input, input_process);
        break;
    }
}
