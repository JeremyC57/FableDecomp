#include <cstdio>
// Behaviour test for CThingHolySite::DrawPreparePrimitivesForRendering (0x0083BA70).
struct CEnginePrimitivePrepareInfo { int frame; };
struct CTCGraphicAppearance {
    int calls; const CEnginePrimitivePrepareInfo* last;
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { ++calls; last = &info; }
};
struct CThingHolySite {
    CTCGraphicAppearance* PTCGraphicAppearance;
    bool baseResult; int baseCalls;
    bool BasePrepare(const CEnginePrimitivePrepareInfo&) { ++baseCalls; return baseResult; }
    bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
    {
        if (BasePrepare(info)) { PTCGraphicAppearance->DrawPreparePrimitivesForRendering(info); return true; }
        return false;
    }
};
int main()
{
    CTCGraphicAppearance app; app.calls = 0; app.last = 0;
    CThingHolySite t; t.PTCGraphicAppearance = &app; t.baseCalls = 0;
    CEnginePrimitivePrepareInfo info; info.frame = 3;
    int fails = 0;
    t.baseResult = false;
    if (t.DrawPreparePrimitivesForRendering(info) || app.calls != 0) ++fails;
    t.baseResult = true;
    if (!t.DrawPreparePrimitivesForRendering(info) || app.calls != 1 || app.last != &info) ++fails;
    if (t.baseCalls != 2) ++fails;
    if (fails) { std::printf("0083ba70_TEST FAIL\n"); return 1; }
    std::printf("0083ba70_TEST PASS\n");
    return 0;
}