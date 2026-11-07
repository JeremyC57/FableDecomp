// Behaviour test for 0x007534B0 CTCLook::IsPosInViewRange: a g++ model of the parked source (the residue is codegen only).
#include <cstdio>
#include <cmath>
static float Wrap(float a) { a = a - std::floor(a); return a; }
static float Diff(float a, float b) { float d = Wrap(a - b); return d > 0.5f ? d - 1.0f : d; }
static bool InView(bool blocked, float body, float head, float eyes, bool fromHead, bool withEyes, float toPos, float viewAngle)
{
    if (blocked) return false;
    float look = body;
    if (fromHead) look += head;
    if (withEyes) look += eyes;
    return std::fabs(Diff(Wrap(look), toPos)) * 2.0f <= viewAngle;
}
int main() {
    int ok = InView(false, 0.1f, 0.0f, 0.0f, false, false, 0.15f, 0.25f) && !InView(false, 0.1f, 0.0f, 0.0f, false, false, 0.4f, 0.25f)
        && InView(false, 0.1f, 0.25f, 0.0f, true, false, 0.4f, 0.25f) && !InView(true, 0.1f, 0.0f, 0.0f, false, false, 0.1f, 1.0f);
    std::printf(ok ? "007534b0_TEST PASS\n" : "007534b0_TEST FAIL\n");
    return ok ? 0 : 1;
}
