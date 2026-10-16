#include <cstdio>
// Behaviour model of CTCPhysicsStandard::CalculateNoStepsForStepBasedMovement (0x008E52F0)
static float g_fStepBasedMovementStepSize = 0.15f;
static float g_fMinMovingSpeed = 1e-4f;
struct C3DVector { float X, Y, Z; };
static void Calc(float distance, C3DVector* step, long* noSteps, long stepsTaken)
{
    if (distance > g_fStepBasedMovementStepSize) {
        float scale = g_fStepBasedMovementStepSize / distance;
        *noSteps = (long)(1.0f / scale - stepsTaken);
        step->X *= scale; step->Y *= scale; step->Z *= scale;
        return;
    }
    if (stepsTaken == 0 && distance > g_fMinMovingSpeed) *noSteps = 1; else *noSteps = 0;
}
int main()
{
    C3DVector s = { 3.0f, 0, 0 }; long n = -1;
    Calc(3.0f, &s, &n, 0);
    if (n < 19 || n > 20 || s.X < 0.149f || s.X > 0.151f) { std::printf("008e52f0_TEST FAIL\n"); return 1; }
    Calc(0.1f, &s, &n, 0); if (n != 1) { std::printf("008e52f0_TEST FAIL\n"); return 1; }
    Calc(0.1f, &s, &n, 2); if (n != 0) { std::printf("008e52f0_TEST FAIL\n"); return 1; }
    Calc(0.0f, &s, &n, 0); if (n != 0) { std::printf("008e52f0_TEST FAIL\n"); return 1; }
    std::printf("008e52f0_TEST PASS\n");
    return 0;
}