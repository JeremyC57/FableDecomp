// NUISystem::CChangingStateComponent::ProcessChangeState() -- when a state change is pending (TargetState != CurrentState), start the component moving towards the target state's record: a position delta and a zoom delta when they are non-zero vectors, and a colour delta when any channel differs, each over UpdateTime and with the record's LinearChange flag.
// Leaf: ?ProcessChangeState@CChangingStateComponent@NUISystem@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0052C920, 395 bytes. NUISystem::CSprite vtable 0x0122F5D4 slot 58; base call of
//   NUISystem::CMorphingSprite::ProcessChangeState 0x005546F0.
// Identity: Ego_r.pdb / FableWin.pdb `?ProcessChangeState@CChangingStateComponent@NUISystem@@MAEXXZ` (protected virtual).
// Callees (all virtual, through the vtable): GetState(unsigned long) slot 135 (+0x21c, 0x0052CEB0), ChangePositionDelta slot 32
//   (+0x80, 0x0052EA30), ChangeZoomDelta slot 36 (+0x90, 0x0052EC20), ChangeColourDelta slot 39 (+0x9c, 0x005305A0).
//   Float pool 0x0129BA3C = ?DELTA_NOTIONAL_ZERO@@3MB (inside the inline C2DVector::IsNonZero).
// Shape: GetState is called for the target and then for the current state; the second result is never used (the virtual
//   call stays, as in retail at 0x0052C951). Each vector delta is formed twice: one temporary for IsNonZero (kept on the
//   x87 stack) and a second one in the 8-byte frame slot passed by reference. The colour delta is a copy of the target
//   colour (CRGBColour's copy constructor reads all four channels through Initialise before any store) with the
//   component's channels subtracted one by one, tested as one dword. The LinearChange flag is an explicit if/else of two
//   calls: VC7.1 cross-jumps the two vector arms into `push 1 / push 0` and tail-duplicates the colour arm with its epilogue.
// Headers: engine/C2DVector.h opt-ins FABLE_RETAIL_C2DVECTOR + FABLE_RETAIL_C2DVECTOR_NORMALISE (IsNonZero, DELTA_NOTIONAL_ZERO)
//   engine/CRGBColour.h opt-in FABLE_RETAIL_CRGBCOLOUR (the Initialise-based copy constructor). No pragma, no flags.
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#include "engine/NUISystem_CChangingStateComponent.h"
#include "engine/NUISystem_CUIState.h"
namespace NUISystem {
void CChangingStateComponent::ProcessChangeState()
{
    if (TargetState == CurrentState)
        return;
    CUIState* target = GetState(TargetState);
    CUIState* current = GetState(CurrentState);
    if (target->StateChangeFlag & 1) {
        if ((target->Position - Position).IsNonZero())
            if (target->LinearChange)
                ChangePositionDelta(target->Position - Position, UpdateTime, true);
            else
                ChangePositionDelta(target->Position - Position, UpdateTime, false);
    }
    if (target->StateChangeFlag & 4) {
        if ((target->Zoom - Zoom).IsNonZero())
            if (target->LinearChange)
                ChangeZoomDelta(target->Zoom - Zoom, UpdateTime, true);
            else
                ChangeZoomDelta(target->Zoom - Zoom, UpdateTime, false);
    }
    if (target->StateChangeFlag & 2) {
        CRGBColour delta = target->Colour;
        delta.R -= Colour.R;
        delta.G -= Colour.G;
        delta.B -= Colour.B;
        delta.A -= Colour.A;
        if (delta.IntValue != 0)
            if (target->LinearChange)
                ChangeColourDelta(delta, UpdateTime, true);
            else
                ChangeColourDelta(delta, UpdateTime, false);
    }
}
}