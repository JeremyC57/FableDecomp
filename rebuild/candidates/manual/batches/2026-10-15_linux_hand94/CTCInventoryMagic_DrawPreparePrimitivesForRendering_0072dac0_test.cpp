#include <cstdio>
// Behaviour test for 0x0072DAC0: closed inventory does nothing; open passes (viewport, second-base value) to the base helper.
struct CViewport { int id; };
static CViewport* g_vp = 0; static long g_value = -1; static int g_calls = 0, g_gets = 0;
struct CTCInventoryBase {
    virtual void v00() {}
    bool InventoryOpen;
    void DrawPreparePrimitives(CViewport* viewport, long value) { ++g_calls; g_vp = viewport; g_value = value; }
};
namespace NHeroInformationScreens { struct CBase { virtual long GetValue() { ++g_gets; return 77; } }; }
struct CTCInventoryMagic : CTCInventoryBase, NHeroInformationScreens::CBase {
    virtual void v00() {}
    void DrawPreparePrimitivesForRendering(CViewport* viewport);
};
void CTCInventoryMagic::DrawPreparePrimitivesForRendering(CViewport* viewport)
{
    if (InventoryOpen)
        DrawPreparePrimitives(viewport, GetValue());
}
int main()
{
    CViewport vp; CTCInventoryMagic m; m.InventoryOpen = false;
    m.DrawPreparePrimitivesForRendering(&vp);
    if (g_calls != 0 || g_gets != 0) { std::printf("0072dac0_TEST FAIL\n"); return 1; }
    m.InventoryOpen = true;
    m.DrawPreparePrimitivesForRendering(&vp);
    if (g_calls != 1 || g_gets != 1 || g_vp != &vp || g_value != 77) { std::printf("0072dac0_TEST FAIL\n"); return 1; }
    std::printf("0072dac0_TEST PASS\n");
    return 0;
}
