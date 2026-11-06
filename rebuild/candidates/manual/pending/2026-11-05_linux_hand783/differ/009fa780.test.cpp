// Behaviour test for CTexture::BltToRenderTargetWithGPU(bool fullRenderTarget, bool scaleHalf, bool pointSample) const
// (0x009FA780): a model of the row -- the quad size comes from the viewport extents, or from the render target (and the
// viewport is reset to it, then restored); four RHW vertices with UVs offset by a half-texel (point sample) or by one texel of
// the half-size target (scale half); render states set, point filtering only when asked; one indexed draw of 2 triangles.
#include <stdio.h>
struct Box { long TLX, TLY, BRX, BRY; };
struct Vtx { float X, Y, Z, RHW; unsigned long Diffuse; float U1, V1; };
struct Device {
    Box Viewport; long RTW, RTH; int SetViewportCalls; Box LastSet;
    bool ZWrite, Blend, AlphaTest, Dither; int Filter; unsigned long Fvf; float Bias;
    Vtx Drawn[4]; unsigned short Idx[6]; int Draws; const void* Tex;
};
static void SetViewport(Device& d, const Box& b) { d.Viewport = b; d.LastSet = b; ++d.SetViewportCalls; }
static void Blt(Device& d, const void* tex, bool full, bool scaleHalf, bool pointSample)
{
    Box oldViewport = d.Viewport;
    long width, height;
    if (!full && !scaleHalf) {
        width = oldViewport.BRX - oldViewport.TLX;
        height = oldViewport.BRY - oldViewport.TLY;
    } else {
        width = d.RTW; height = d.RTH;
        Box v = { 0, 0, width, height };
        SetViewport(d, v);
    }
    Vtx verts[4];
    verts[0].X = 0.0f; verts[1].X = (float)width; verts[2].X = 0.0f; verts[3].X = (float)width;
    verts[0].Y = 0.0f; verts[1].Y = 0.0f; verts[2].Y = (float)height; verts[3].Y = (float)height;
    verts[0].U1 = 0.0f; verts[1].U1 = 1.0f; verts[2].U1 = 0.0f; verts[3].U1 = 1.0f;
    verts[0].V1 = 0.0f; verts[1].V1 = 0.0f; verts[2].V1 = 1.0f; verts[3].V1 = 1.0f;
    float du = 0.0f, dv = 0.0f;
    if (scaleHalf) { du = 1.0f / ((float)width * 0.5f); dv = 1.0f / ((float)height * 0.5f); }
    else if (pointSample) { du = 0.5f / (float)width; dv = 0.5f / (float)height; }
    for (long i = 0; i < 4; ++i) { verts[i].RHW = 1.0f; verts[i].Z = 0.0f; verts[i].Diffuse = 0; verts[i].U1 += du; verts[i].V1 += dv; }
    d.ZWrite = false; d.Blend = true; d.AlphaTest = false; d.Dither = false;
    if (pointSample) d.Filter = 1;
    d.Fvf = 0x144; d.Bias = -1.0f;
    unsigned short indices[6] = { 0, 1, 2, 1, 3, 2 };
    for (int i = 0; i < 4; ++i) d.Drawn[i] = verts[i];
    for (int i = 0; i < 6; ++i) d.Idx[i] = indices[i];
    d.Tex = tex; ++d.Draws;
    if (full || scaleHalf) SetViewport(d, oldViewport);
}
static Device Fresh()
{
    Device d = {}; d.Viewport.TLX = 10; d.Viewport.TLY = 20; d.Viewport.BRX = 110; d.Viewport.BRY = 70; d.RTW = 640; d.RTH = 480; return d;
}
int main()
{
    int ok = 1; int tex = 0;
    Device a = Fresh(); Blt(a, &tex, false, false, false);
    ok &= a.SetViewportCalls == 0 && a.Drawn[3].X == 100.0f && a.Drawn[3].Y == 50.0f && a.Drawn[1].U1 == 1.0f && a.Filter == 0;
    ok &= a.Drawn[0].RHW == 1.0f && a.Fvf == 0x144 && a.Idx[4] == 3 && a.Draws == 1 && a.Tex == &tex && a.Blend && !a.ZWrite;
    Device b = Fresh(); Blt(b, &tex, false, false, true);
    ok &= b.Filter == 1 && b.Drawn[0].U1 == 0.5f / 100.0f && b.Drawn[2].V1 == 1.0f + 0.5f / 50.0f;
    Device c = Fresh(); Blt(c, &tex, false, true, false);
    ok &= c.SetViewportCalls == 2 && c.LastSet.BRX == 110 && c.Drawn[3].X == 640.0f && c.Drawn[0].U1 == 1.0f / 320.0f;
    Device e = Fresh(); Blt(e, &tex, true, false, false);
    ok &= e.SetViewportCalls == 2 && e.Viewport.TLX == 10 && e.Drawn[3].Y == 480.0f && e.Drawn[0].U1 == 0.0f;
    printf(ok ? "009fa780_TEST PASS\n" : "009fa780_TEST FAIL\n");
    return ok ? 0 : 1;
}
