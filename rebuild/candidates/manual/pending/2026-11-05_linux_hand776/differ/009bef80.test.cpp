// Behaviour test for 0x009BEF80 (parked DIFFER; models differ/009bef80.cpp): CDisplayManager::SetViewport -- clamp to the render target, flag off-screen boxes, skip unchanged boxes
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct C2DBoxI { long TLX, TLY, BRX, BRY; void Initialise(long a, long b, long c, long d) { TLX = a; TLY = b; BRX = c; BRY = d; } };
struct CViewport { long X, Y, Width, Height; float MinZ, MaxZ; };
static int g_set = 0, g_notify = 0;
struct CDisplayManager {
    long Width, Height; C2DBoxI ViewportBox; bool ViewportOffscreen; CViewport D3DViewport;
    void SetViewport(const C2DBoxI& viewport) {
        long height = Height, width = Width;
        C2DBoxI box;
        long tlx = viewport.TLX; long left;
        if (tlx < 0) left = 0; else { if (tlx > width) tlx = width; left = tlx; }
        box.TLX = left;
        long brx = viewport.BRX + 1; long right;
        if (brx < 0) right = 0; else right = brx > width ? width : brx;
        box.BRX = right;
        long tly = viewport.TLY; long top;
        if (tly < 0) top = 0; else { if (tly > height) tly = height; top = tly; }
        box.TLY = top;
        long bry = viewport.BRY + 1; long bottom;
        if (bry < 0) bottom = 0; else { if (bry > height) bry = height; bottom = bry; }
        box.BRY = bottom;
        if (left == ViewportBox.TLX && top == ViewportBox.TLY && right == ViewportBox.BRX && bottom == ViewportBox.BRY) return;
        ViewportOffscreen = false;
        if (left == 0 && right == 0) { box.BRX = 1; ViewportOffscreen = true; }
        else if (left == Width && right == Width) { box.TLX = Width - 1; ViewportOffscreen = true; }
        if (top == 0 && bottom == 0) { box.BRY = 1; ViewportOffscreen = true; }
        else if (top == Height && bottom == Height) { box.TLY = Height - 1; ViewportOffscreen = true; }
        D3DViewport.X = box.TLX; D3DViewport.Y = box.TLY; D3DViewport.Width = box.BRX - box.TLX; D3DViewport.Height = box.BRY - box.TLY;
        D3DViewport.MinZ = 0.0f; D3DViewport.MaxZ = 1.0f;
        ++g_set; ViewportBox = box; ++g_notify;
    }
};
int main()
{
    CDisplayManager d; d.Width = 640; d.Height = 480; d.ViewportBox.Initialise(-1, -1, -1, -1);
    C2DBoxI v; v.Initialise(-10, 20, 1000, 99);
    d.SetViewport(v);
    CHECK(d.D3DViewport.X == 0 && d.D3DViewport.Y == 20 && d.D3DViewport.Width == 640 && d.D3DViewport.Height == 80 && !d.ViewportOffscreen);
    d.SetViewport(v); CHECK(g_set == 1);
    v.Initialise(-5, 0, -3, -1); d.SetViewport(v);
    CHECK(d.ViewportOffscreen && d.D3DViewport.Width == 1 && d.D3DViewport.Height == 1);
    v.Initialise(700, 500, 800, 600); d.SetViewport(v);
    CHECK(d.ViewportOffscreen && d.D3DViewport.X == 639 && d.D3DViewport.Y == 479 && d.D3DViewport.Width == 1);
    if (g_fail) { std::printf("009bef80_TEST FAIL\n"); return 1; }
    std::printf("009bef80_TEST PASS\n");
    return 0;
}
