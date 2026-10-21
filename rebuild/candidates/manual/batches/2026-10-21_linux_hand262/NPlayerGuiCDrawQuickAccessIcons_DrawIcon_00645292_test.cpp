// behaviour test for 00645292 NPlayerGui::CDrawQuickAccessIcons::DrawIcon
#include <cstdio>
struct Draw { int layer; long graphic; unsigned char alpha; int handleSet; long handle; };
struct Out { Draw d[4]; int n; };
static void DrawIcon(Out& out, float uiAlpha, bool forceOpaque, long emptyBack, float fade, long index, long backGraphic, long iconGraphic,
                     bool selected)
{
    unsigned char alpha = (unsigned char)(uiAlpha * 255.0f);
    if (forceOpaque && alpha < 0xff)
        alpha = 0xff;
    if (backGraphic > 0) {
        unsigned char backAlpha = alpha;
        if (backGraphic == emptyBack)
            backAlpha = (unsigned char)(backAlpha * 0.5f);
        Draw& d = out.d[out.n++];
        d.layer = 0x45; d.graphic = backGraphic; d.alpha = backAlpha; d.handleSet = 0; d.handle = 7;
    }
    if (iconGraphic > 0 && fade > 0.0001f) {
        float iconAlpha = alpha * fade;
        if (forceOpaque && alpha < 0xff)
            iconAlpha = selected ? 255.0f : 0.0f;
        Draw& d = out.d[out.n++];
        d.layer = 0x44; d.graphic = iconGraphic; d.alpha = (unsigned char)iconAlpha; d.handleSet = selected ? 1 : 0; d.handle = index;
    }
}
int main()
{
    int bad = 0;
    Out o; o.n = 0;
    DrawIcon(o, 1.0f, false, 9, 0.5f, 3, 9, 12, true);
    if (o.n != 2 || o.d[0].alpha != 127 || o.d[0].handle != 7 || o.d[1].alpha != 127 || o.d[1].handleSet != 1 || o.d[1].handle != 3) bad |= 1;
    o.n = 0;
    DrawIcon(o, 0.5f, true, 9, 0.5f, 2, 5, 12, false);
    if (o.n != 2 || o.d[0].alpha != 255 || o.d[1].alpha != 127 || o.d[1].handleSet != 0) bad |= 2;
    o.n = 0;
    DrawIcon(o, 1.0f, false, 9, 0.00001f, 2, 0, 12, false);
    if (o.n != 0) bad |= 4;
    if (bad) { std::printf("00645292_TEST FAIL\n"); return 1; }
    std::printf("00645292_TEST PASS\n");
    return 0;
}
