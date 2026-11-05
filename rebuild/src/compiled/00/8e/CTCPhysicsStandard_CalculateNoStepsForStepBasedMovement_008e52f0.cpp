// CTCPhysicsStandard::CalculateNoStepsForStepBasedMovement(float distance, C3DVector* step, long* noSteps, long stepsTaken) -- split a move longer than the step size into steps: scale the step vector down to one step and return the remaining step count; a short move is one step only if it is the first and non-negligible
// Leaf: CalculateNoStepsForStepBasedMovement
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E52F0, 128 bytes (`ret 10h`; `this` unused).
// Callees: _ftol @ 0x00BFEA70. Globals: the step size 0x012786A8 (0.15f), the minimum moving distance 0x0129BA3C (1e-4f, shared
//   with UpdateIsMoving 0x008E5270); 1.0f from the pool 0x0122DED8.
// Shape: scale = size / distance stays on the FPU stack for the count `(long)(1.0f / scale - stepsTaken)` (fisub) and the
//   three in-place multiplies (`*step *= scale`); the short-move branch is if / else on the count store.
#include "engine/C3DVector.h"
extern float g_fStepBasedMovementStepSize;      // 0x012786A8 (0.15)
extern const float DELTA_NOTIONAL_ZERO;    // 0x0129BA3C (1e-4)
struct CTCPhysicsStandard {
    void CalculateNoStepsForStepBasedMovement(float distance, C3DVector* step, long* noSteps, long stepsTaken);
};

void CTCPhysicsStandard::CalculateNoStepsForStepBasedMovement(float distance, C3DVector* step, long* noSteps, long stepsTaken)
{
    if (distance > g_fStepBasedMovementStepSize) {
        float scale = g_fStepBasedMovementStepSize / distance;
        *noSteps = (long)(1.0f / scale - stepsTaken);
        *step *= scale;
        return;
    }
    if (stepsTaken == 0 && distance > DELTA_NOTIONAL_ZERO)
        *noSteps = 1;
    else
        *noSteps = 0;
}