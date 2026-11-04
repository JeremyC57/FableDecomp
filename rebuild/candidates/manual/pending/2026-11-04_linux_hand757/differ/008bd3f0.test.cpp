// Behaviour test for 0x008BD3F0 CCreatureAction_TeleportOut::FinishAction: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <cmath>
static float Heading(float dx, float dy)
{
    float h = (float)std::atan2(dx, dy) * 0.15915494f;
    if (h < 0.0f || h >= 1.0f) { h = (float)std::fmod((double)h, 1.0); if (h < 0.0f) h += 1.0f; }
    return h;
}
int main() {
    int ok = std::fabs(Heading(0.0f, 1.0f)) < 1e-6f && std::fabs(Heading(1.0f, 0.0f) - 0.25f) < 1e-5f && std::fabs(Heading(-1.0f, 0.0f) - 0.75f) < 1e-5f;
    std::printf(ok ? "008bd3f0_TEST PASS\n" : "008bd3f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
