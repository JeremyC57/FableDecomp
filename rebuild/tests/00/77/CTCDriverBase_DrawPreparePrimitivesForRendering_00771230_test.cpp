// Behaviour test for CTCDriverBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) @ 0x00771230:
// the call is forwarded (with the same argument) only when the parent has a graphic-appearance component.
#include <cstdio>
struct CEnginePrimitivePrepareInfo { int id; };
static int g_calls = 0;
static const CEnginePrimitivePrepareInfo* g_arg = 0;
struct CTCGraphicAppearance {
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { ++g_calls; g_arg = &info; }
};
struct CThing {
    unsigned char         _pad[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;
};
struct CTCDriverBase {
    virtual void v00() { }
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};
void CTCDriverBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    CTCGraphicAppearance* appearance = ParentThing->PTCGraphicAppearance;
    if (appearance != 0)
        appearance->DrawPreparePrimitivesForRendering(info);
}
int main()
{
    CThing thing; CTCGraphicAppearance app; CEnginePrimitivePrepareInfo info;
    CTCDriverBase d; d.ParentThing = &thing;
    thing.PTCGraphicAppearance = 0;
    d.DrawPreparePrimitivesForRendering(info);
    if (g_calls != 0) { std::printf("00771230_TEST FAIL\n"); return 1; }
    thing.PTCGraphicAppearance = &app;
    d.DrawPreparePrimitivesForRendering(info);
    if (g_calls != 1 || g_arg != &info) { std::printf("00771230_TEST FAIL\n"); return 1; }
    std::printf("00771230_TEST PASS\n");
    return 0;
}