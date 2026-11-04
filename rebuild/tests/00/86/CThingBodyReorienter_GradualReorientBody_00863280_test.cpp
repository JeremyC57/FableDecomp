// Behaviour test for 0x00863280 CThingBodyReorienter::GradualReorientBody: a portable model of the retail logic (turns,
// wrap into [0, 1) with fmod, short way round, finish when |delta| < 1e-4 + 0.5, else clamp to +-0.5 and turn).
#include <cstdio>
#include <cmath>
static const float kEps = 1e-4f;
struct Body { float Facing; int Sets; void SetFacingAngleXY(float a) { Facing = a; ++Sets; } };
struct Reorienter {
    bool Done, StillChecking, Have; float Final; Body* B;
    void GradualReorientBody() {
        if (!Have) return;
        float current = B->Facing;
        float difference = Final - current;
        if (difference < 0.0f || difference >= 1.0f) {
            difference = (float)std::fmod((double)difference, 1.0);
            if (difference < 0.0f) difference += 1.0f;
        }
        float delta = difference;
        if (difference > 0.5f) delta = difference - 1.0f;
        if (std::fabs(delta) < kEps + 0.5f) {
            Have = false; B->SetFacingAngleXY(Final);
            if (!StillChecking) Done = true;
            return;
        }
        if (delta < -0.5f) delta = -0.5f; else if (delta > 0.5f) delta = 0.5f;
        B->SetFacingAngleXY(current + delta);
    }
};
int main() {
    bool ok = true;
    Body b = { 0.1f, 0 };
    Reorienter r = { false, false, true, 0.9f, &b };
    r.GradualReorientBody();                              // wrapped difference 0.8 -> delta -0.2: snaps to the final angle
    ok = ok && b.Facing == 0.9f && !r.Have && r.Done && b.Sets == 1;
    Body c = { 2.25f, 0 };
    Reorienter s = { false, true, true, 0.0f, &c };      // difference -2.25 wraps to 0.75 -> delta -0.25: snaps, still checking
    s.GradualReorientBody();
    ok = ok && c.Facing == 0.0f && !s.Have && !s.Done && c.Sets == 1;
    Reorienter t = { false, false, false, 0.5f, &c };    // no final angle: nothing happens
    t.GradualReorientBody();
    ok = ok && c.Sets == 1 && !t.Done;
    std::printf(ok ? "00863280_TEST PASS\n" : "00863280_TEST FAIL\n");
    return ok ? 0 : 1;
}