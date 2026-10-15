#include <cstdio>
// Behaviour test for CThingSwitch::DrawIsDrawable (0x0083BE50): engine flag AND base predicate.
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
struct CThingSwitch : CThing {
    bool DrawIsDrawable() { return GetDisplayEngine()->IsDrawingEnabled() && BaseDrawIsDrawable(); }
};
int main()
{
    CDisplayEngine e;
    CThingSwitch t;
    t.engine = &e;
    int fails = 0;
    for (int a = 0; a < 2; ++a)
        for (int b = 0; b < 2; ++b) {
            e.enabled = a != 0; t.baseDrawable = b != 0; t.baseCalls = 0;
            bool r = t.DrawIsDrawable();
            if (r != (a && b)) ++fails;
            if (!a && t.baseCalls != 0) ++fails;     // short-circuit: base not asked
        }
    if (fails) { std::printf("0083be50_TEST FAIL\n"); return 1; }
    std::printf("0083be50_TEST PASS\n");
    return 0;
}