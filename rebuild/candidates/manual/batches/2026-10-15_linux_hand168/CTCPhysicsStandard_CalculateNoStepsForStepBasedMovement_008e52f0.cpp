// CTCPhysicsStandard::CalculateNoStepsForStepBasedMovement(float distance, C3DVector* step, long* noSteps, long stepsTaken) -- split a move longer than the step size into steps: scale the step vector down to one step and return the remaining step count; a short move is one step only if it is the first and non-negligible
// Leaf: CalculateNoStepsForStepBasedMovement
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E52F0, 128 bytes (`ret 10h`; `this` unused).
// Callees: _ftol @ 0x00BFEA70. Globals: the step size 0x012786A8 (0.15f), the minimum moving distance 0x0129BA3C (1e-4f, shared
//   with UpdateIsMoving 0x008E5270); 1.0f from the pool 0x0122DED8.
// Shape: scale = size / distance stays on the FPU stack for the count `(long)(1.0f / scale - stepsTaken)` (fisub) and the
//   three in-place multiplies (`*step *= scale`); the short-move branch is if / else on the count store.
struct C3DVector {
    float X, Y, Z;
    C3DVector& operator*=(float s) { X *= s; Y *= s; Z *= s; return *this; }
};
extern float g_fStepBasedMovementStepSize;      // 0x012786A8 (0.15)
extern float g_fMinMovingSpeed;                 // 0x0129BA3C (1e-4)
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
    if (stepsTaken == 0 && distance > g_fMinMovingSpeed)
        *noSteps = 1;
    else
        *noSteps = 0;
}
