// CTCFlourishTarget::ProcessActionMovementDelta(C3DVector& movement_delta) const -- re-express an action's movement delta in the frame facing the live attacker on the ground plane (x towards the attacker, y sideways, z world up), optionally culling backward (negative x) and all sideways (y) movement
// Leaf: ?ProcessActionMovementDelta@CTCFlourishTarget@@QBEXAAVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E8A40, 482 bytes, thiscall `ret 4`, no frame pointer. Ego_r.pdb / FableWin.pdb
//   ?ProcessActionMovementDelta@CTCFlourishTarget@@QBEXAAVC3DVector@@@Z; Ego_d.pdb (tc_flourish_target.obj, after OnInitialActivate):
//   parameter movement_delta (C3DVector&), x_axis in the outer block, components / z_axis / y_axis in the inner block.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3), CThing::GetPos 0x004C73D0 (x2), C3DVector::AccurateNormalise 0x00A14440.
// Data: DELTA_NOTIONAL_ZERO 0x0129BA3C (squared), 0.0f 0x0122DEDC.
// Shape: `if (PAttacker.Get() && !PAttacker->IsDead())` (retail `test byte ptr [eax+91h],1 ; jne`; IsAlive's `!Dead` gives not/test);
//   x_axis from the CThing::GetPos difference with Z then zeroed (the Z subtraction is dead), IsNonZero, AccurateNormalise; z_axis (0,0,1),
//   y_axis = x_axis.Cross(z_axis), components from the three dots, the two culls, then the axes recombined.
// FITTED: FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP (existing opt-in, the Cross text with the X component parenthesised; used by five
//   landed rows, e.g. 0x00697CC0 / 0x007C1000). Without it 4 hunks: the y_axis dot sums Z, Y, X (0x007E8B2E) where retail sums X, Z, Y.
// Header: NEW engine/CTCFlourishTarget.h (hdr/README.md). Inline-budget: none.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCFlourishTarget.h"

void CTCFlourishTarget::ProcessActionMovementDelta(C3DVector& movement_delta) const
{
    if (PAttacker.Get() && !PAttacker->IsDead()) {
        C3DVector x_axis = PAttacker->GetPos() - GetParentThing().GetPos();
        x_axis.Z = 0.0f;
        if (x_axis.IsNonZero()) {
            x_axis.AccurateNormalise();
            C3DVector z_axis(0.0f, 0.0f, 1.0f);
            C3DVector y_axis = x_axis.Cross(z_axis);
            C3DVector components(x_axis.Dot(movement_delta), y_axis.Dot(movement_delta), z_axis.Dot(movement_delta));
            if (CullNegativeXAxisMovement && components.X < 0.0f)
                components.X = 0.0f;
            if (CullAllYAxisMovement)
                components.Y = 0.0f;
            movement_delta = x_axis * components.X + y_axis * components.Y + z_axis * components.Z;
        }
    }
}
