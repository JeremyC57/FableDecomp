// Behaviour test for 0x008CF7D0 (parked row): the row's logic over a flat model.
#include <cstdio>

enum Next { LOOP, OUT_OF };
static Next FinishAction(bool interrupt, float fps, float strikeSecs, long now, long start) {
    if (!interrupt && (long)(fps * strikeSecs) >= now - start) return LOOP;
    return OUT_OF;
}
int main() {
    bool ok = FinishAction(false, 30, 2.0f, 100, 50) == LOOP && FinishAction(false, 30, 2.0f, 200, 50) == OUT_OF && FinishAction(true, 30, 2.0f, 60, 50) == OUT_OF;
    std::printf(ok ? "008cf7d0_TEST PASS\n" : "008cf7d0_TEST FAIL\n");
    return ok ? 0 : 1;
}
