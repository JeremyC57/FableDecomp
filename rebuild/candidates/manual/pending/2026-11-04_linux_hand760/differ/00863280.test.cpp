// Behaviour test for 0x00863280 (parked row): the row's logic over a flat model.
#include <cstdio>

#include <cmath>
struct Reorienter { bool Have, StillChecking, Done; float Final, Facing;
    void Gradual() {
        if (!Have) return;
        float current = Facing, diff = Final - current;
        if (diff < 0.0f || diff >= 1.0f) { diff = (float)std::fmod(diff, 1.0); if (diff < 0.0f) diff += 1.0f; }
        float delta = diff > 0.5f ? diff - 1.0f : diff;
        if (std::fabs(delta) < 0.05f) { Have = false; Facing = Final; if (!StillChecking) Done = true; return; }
        if (delta < -0.05f) delta = -0.05f; else if (delta > 0.05f) delta = 0.05f;
        Facing = current + delta;
    } };
int main() {
    Reorienter r = { true, false, false, 0.2f, 0.0f };
    r.Gradual(); bool ok = std::fabs(r.Facing - 0.05f) < 1e-6f && r.Have;
    for (int i = 0; i < 10; ++i) r.Gradual();
    ok = ok && !r.Have && r.Done && r.Facing == 0.2f;
    Reorienter w = { true, true, false, 0.9f, 0.1f }; w.Gradual(); ok = ok && w.Facing < 0.1f;   // wraps the short way round
    std::printf(ok ? "00863280_TEST PASS\n" : "00863280_TEST FAIL\n");
    return ok ? 0 : 1;
}
