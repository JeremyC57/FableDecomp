#include <cstdio>
// Behaviour test for CThingMarker::DrawIsDrawable (0x0083D100): base && (engine flag || has appearance).
struct CDisplayEngine { bool enabled; bool IsDrawingEnabled() { return enabled; } };
struct CThingMarker {
    CDisplayEngine* engine; void* PTCGraphicAppearance; bool base; int engineCalls;
    CDisplayEngine* GetDisplayEngine() { ++engineCalls; return engine; }
    bool BaseDrawIsDrawable() { return base; }
    bool DrawIsDrawable()
    {
        if (BaseDrawIsDrawable()) return GetDisplayEngine()->IsDrawingEnabled() || PTCGraphicAppearance != 0;
        return false;
    }
};
int main()
{
    CDisplayEngine e; CThingMarker m; m.engine = &e; int dummy;
    int fails = 0;
    for (int b = 0; b < 2; ++b) for (int en = 0; en < 2; ++en) for (int ap = 0; ap < 2; ++ap) {
        m.base = b != 0; e.enabled = en != 0; m.PTCGraphicAppearance = ap ? &dummy : 0; m.engineCalls = 0;
        bool r = m.DrawIsDrawable();
        if (r != (b && (en || ap))) ++fails;
        if (!b && m.engineCalls) ++fails;
    }
    if (fails) { std::printf("0083d100_TEST FAIL\n"); return 1; }
    std::printf("0083d100_TEST PASS\n");
    return 0;
}