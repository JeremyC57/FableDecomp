#include <cstdio>
// Model of the 2D static mesh queueing: viewport and clip-box culling, layer enable, per-sub-mesh render flags
// (alpha blend / forced blend for alpha-tested materials under flag 0x100 / alpha test, plus the 0x800 and 8 flag bits),
// sort list 0x10/0xf from the material flag, and the depth bias applied when the render flags have bit 0.
struct Material { unsigned char Flags; bool AlphaTest; bool AlphaBlend; };
struct Queued { long List; unsigned long Flags; long SortList; float Depth; int Alpha; };
static Queued g_queue[8];
static int g_count = 0;
static unsigned long RenderFlagsFor(unsigned long meshFlags, const Material& m)
{
    bool alphaTest = m.AlphaTest;
    bool blend = m.AlphaBlend;
    if ((meshFlags & 0x100) && alphaTest)
        blend = true;
    unsigned long flags = 0;
    if (blend)
        flags = 2;
    else if (alphaTest)
        flags = 5;
    if (meshFlags & 0x800)
        flags |= 0xc0;
    if (meshFlags & 8)
        flags |= 0x180;
    return flags;
}
static void Render(bool enable, float tlx, float tly, float brx, float bry, const long vp[4], bool layerOn, unsigned long meshFlags,
                   const Material* mats, int count, long type, int alpha, float layerDepth, float bias)
{
    if (!enable)
        return;
    if (tlx > vp[2] || brx < vp[0] || tly > vp[3] || bry < vp[1])
        return;
    if (!layerOn)
        return;
    for (int i = 0; i < count; ++i) {
        unsigned long flags = RenderFlagsFor(meshFlags, mats[i]);
        float depth = layerDepth;
        if (flags & 1)
            depth -= bias * 9.3132257e-10f;
        Queued q;
        q.List = type; q.Flags = flags; q.SortList = (mats[i].Flags & 1) ? 0x10 : 0xf; q.Depth = depth; q.Alpha = alpha;
        g_queue[g_count++] = q;
    }
}
int main()
{
    int bad = 0;
    const long vp[4] = { 0, 0, 640, 480 };
    Material mats[3];
    mats[0].Flags = 1; mats[0].AlphaTest = false; mats[0].AlphaBlend = true;
    mats[1].Flags = 0; mats[1].AlphaTest = true; mats[1].AlphaBlend = false;
    mats[2].Flags = 0; mats[2].AlphaTest = false; mats[2].AlphaBlend = false;
    Render(true, 700.0f, 10.0f, 800.0f, 20.0f, vp, true, 0, mats, 3, 4, 255, 1.0f, 0.0f);
    if (g_count != 0) bad = 1;
    Render(true, 10.0f, 10.0f, 20.0f, 20.0f, vp, false, 0, mats, 3, 4, 255, 1.0f, 0.0f);
    if (g_count != 0) bad = 1;
    Render(false, 10.0f, 10.0f, 20.0f, 20.0f, vp, true, 0, mats, 3, 4, 255, 1.0f, 0.0f);
    if (g_count != 0) bad = 1;
    Render(true, 10.0f, 10.0f, 20.0f, 20.0f, vp, true, 0x808, mats, 3, 4, 128, 2.0f, 1073741824.0f);
    if (g_count != 3) bad = 1;
    if (g_queue[0].Flags != (2 | 0xc0 | 0x180) || g_queue[0].SortList != 0x10 || g_queue[0].Depth != 2.0f) bad = 1;
    if (g_queue[1].Flags != (5 | 0xc0 | 0x180) || g_queue[1].SortList != 0xf || g_queue[1].Depth != 1.0f) bad = 1;
    if (g_queue[2].Flags != (0xc0 | 0x180) || g_queue[2].List != 4 || g_queue[2].Alpha != 128) bad = 1;
    if (RenderFlagsFor(0x100, mats[1]) != 2) bad = 1;
    if (bad) { std::printf("00b9e950_TEST FAIL\n"); return 1; }
    std::printf("00b9e950_TEST PASS\n");
    return 0;
}