#include <cstdio>
// Behaviour test for CThingHolySite::DrawIsDrawable (0x0083B9F0): engine flag AND base predicate.
struct CDisplayEngine {
    bool enabled;
    bool IsDrawingEnabled() { return enabled; }
};
struct CThing {
    CDisplayEngine* engine;
    bool baseDrawable;
    int baseCalls;
    CDisplayEngine* GetDisplayEngine() { return engine; }
    bool BaseDrawIsDrawable() { ++baseCalls; return baseDrawable; }
};
struct CThingHolySite : CThing {
    bool DrawIsDrawable() { return GetDisplayEngine()->IsDrawingEnabled() && BaseDrawIsDrawable(); }
};
int main()
{
    CDisplayEngine e;
    CThingHolySite t;
    t.engine = &e;
    int fails = 0;
    for (int a = 0; a < 2; ++a)
        for (int b = 0; b < 2; ++b) {
            e.enabled = a != 0; t.baseDrawable = b != 0; t.baseCalls = 0;
            bool r = t.DrawIsDrawable();
            if (r != (a && b)) ++fails;
            if (!a && t.baseCalls != 0) ++fails;     // short-circuit: base not asked
        }
    if (fails) { std::printf("0083b9f0_TEST FAIL\n"); return 1; }
    std::printf("0083b9f0_TEST PASS\n");
    return 0;
}
