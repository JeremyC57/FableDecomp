// Behaviour test for 0048dc3c: body copied from src/0048dc3c.cpp (CSubtitleRenderer::GetTextOffsetFromEmbeddedTagGraphic), PeekGuiDef stubbed.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct C2DVector { float X, Y; };
class CSubtitleRenderer {
public:
    void* __vftable;
    const C2DVector& GetTextOffsetFromEmbeddedTagGraphic(long graphic) const;
};
struct CPlayerGuiDef {
    unsigned char _pad000[0x66C];
    long          ControllerThumbLeftGraphic;
    long          ControllerThumbRightGraphic;
    long          ControllerThumbClickLeftGraphic;
    long          ControllerThumbClickRightGraphic;
    long          ControllerBackGraphic;
    long          ControllerStartGraphic;
    long          ControllerTriggerLeftGraphic;
    long          ControllerTriggerRightGraphic;
    long          ControllerTriggerBlackGraphic;
    long          ControllerTriggerWhiteGraphic;
    C2DVector     ControllerGraphicOffset;
    C2DVector     ControllerGraphicTextOffset;
    C2DVector     ControllerClickGraphicTextOffset;
    C2DVector     ControllerTriggerGraphicTextOffset;
};
static CPlayerGuiDef g_def;
CPlayerGuiDef* __fastcall PeekGuiDef() { return &g_def; }
const C2DVector& CSubtitleRenderer::GetTextOffsetFromEmbeddedTagGraphic(long graphic) const
{
    const CPlayerGuiDef* def = PeekGuiDef();
    if (graphic == def->ControllerThumbLeftGraphic || graphic == def->ControllerThumbRightGraphic)
        return def->ControllerGraphicTextOffset;
    if (graphic == def->ControllerThumbClickLeftGraphic)
        return def->ControllerClickGraphicTextOffset;
    if (graphic == def->ControllerThumbClickRightGraphic)
        return def->ControllerClickGraphicTextOffset;
    if (graphic == def->ControllerTriggerLeftGraphic || graphic == def->ControllerTriggerRightGraphic)
        return def->ControllerTriggerGraphicTextOffset;
    return def->ControllerGraphicTextOffset;
}
int main()
{
    memset(&g_def, 0, sizeof(g_def));
    g_def.ControllerThumbLeftGraphic = 1; g_def.ControllerThumbRightGraphic = 2;
    g_def.ControllerThumbClickLeftGraphic = 3; g_def.ControllerThumbClickRightGraphic = 4;
    g_def.ControllerTriggerLeftGraphic = 5; g_def.ControllerTriggerRightGraphic = 6;
    CSubtitleRenderer r; r.__vftable = 0;
    CHECK(&r.GetTextOffsetFromEmbeddedTagGraphic(1) == &g_def.ControllerGraphicTextOffset);
    CHECK(&r.GetTextOffsetFromEmbeddedTagGraphic(2) == &g_def.ControllerGraphicTextOffset);
    CHECK(&r.GetTextOffsetFromEmbeddedTagGraphic(3) == &g_def.ControllerClickGraphicTextOffset);
    CHECK(&r.GetTextOffsetFromEmbeddedTagGraphic(4) == &g_def.ControllerClickGraphicTextOffset);
    CHECK(&r.GetTextOffsetFromEmbeddedTagGraphic(5) == &g_def.ControllerTriggerGraphicTextOffset);
    CHECK(&r.GetTextOffsetFromEmbeddedTagGraphic(6) == &g_def.ControllerTriggerGraphicTextOffset);
    CHECK(&r.GetTextOffsetFromEmbeddedTagGraphic(99) == &g_def.ControllerGraphicTextOffset);
    if (fails == 0) printf("0048dc3c_TEST PASS\n"); else printf("0048dc3c_TEST FAIL\n");
    return fails;
}