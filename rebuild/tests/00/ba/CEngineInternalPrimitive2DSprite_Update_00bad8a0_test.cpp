// Behaviour test for CEngineInternalPrimitive2DSprite::Update (0x00BAD8A0): descriptor copy, WrapTexture from the UV
// offsets, flag-derived render layer mask, and the clip-box growth from the bank frame (centred or not) or the texture.
#include <cstdio>

static const float g_eps = 0.0001f;
struct C2DVector { float X, Y; bool IsNonZero() const { return X * X + Y * Y > g_eps * g_eps; } };
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct C2DExtentsI { long X, Y; };
static int g_loads;
struct CTexture { long W, H; C2DExtentsI GetDimensions(unsigned long) { C2DExtentsI e; e.X = W; e.Y = H; return e; } };
struct Desc {
    long InterpolationSet; C2DBoxF ClipBox; long Layer; unsigned long BankIndex, Frame; float StartAngle, EndAngle;
    long BlendMode; unsigned long Colour; bool ClipBoxIsPos; unsigned long Flags; CTexture* Texture; C2DVector UVStart, UVEnd;
};
struct Frame { unsigned short FrameWidth, FrameHeight; void BeginLoading() { ++g_loads; } };
static Frame g_frames[3];
static Frame* GetFrame(unsigned long bank, unsigned long frame) { return bank < 2 ? &g_frames[frame] : 0; }
struct Sprite {
    unsigned long EnginePrimitiveFlags, RenderLayerMask; long InterpolationSet;
    C2DBoxF ClipBox; long Layer; unsigned long BankIndex, Frame; float StartAngle, EndAngle; unsigned long Colour;
    CTexture* Texture; bool Filter; C2DVector UVStart, UVEnd; bool WrapTexture; long BlendMode;
    void Update(const Desc& d, unsigned long flags)
    {
        EnginePrimitiveFlags = flags;
        ClipBox = d.ClipBox; Layer = d.Layer; BankIndex = d.BankIndex; Frame = d.Frame;
        StartAngle = d.StartAngle; EndAngle = d.EndAngle; BlendMode = d.BlendMode;
        unsigned long colour = d.Colour; Colour = colour;
        InterpolationSet = d.InterpolationSet; Texture = d.Texture;
        Filter = ((d.Flags >> 1) & 1) != 0;
        UVStart = d.UVStart; UVEnd = d.UVEnd;
        WrapTexture = UVStart.IsNonZero() || UVEnd.IsNonZero();
        if (flags & 0x80) EnginePrimitiveFlags |= 0x80;
        RenderLayerMask = (flags & 0x400) ? 0x00200000 : 0x80000000;
        if (BankIndex) {
            ::Frame* f = GetFrame(BankIndex, Frame);
            f->BeginLoading();
            if (d.ClipBoxIsPos) {
                if (d.Flags & 1) {
                    unsigned long hw = f->FrameWidth >> 1, hh = f->FrameHeight >> 1;
                    ClipBox.TLX -= hw; ClipBox.TLY -= hh; ClipBox.BRX += hw; ClipBox.BRY += hh;
                } else {
                    ClipBox.BRX += f->FrameWidth; ClipBox.BRY += f->FrameHeight;
                }
            }
        } else if (d.Texture && d.ClipBoxIsPos) {
            unsigned long w = Texture->GetDimensions(0).X; ClipBox.BRX += w;
            unsigned long h = Texture->GetDimensions(0).Y; ClipBox.BRY += h;
        }
    }
};

int main()
{
    g_frames[1].FrameWidth = 40; g_frames[1].FrameHeight = 20;
    CTexture tex; tex.W = 64; tex.H = 32;
    Desc d = { 3, { 100, 100, 100, 100 }, 2, 1, 1, 0.5f, 1.5f, 7, 0xff00ff00ul, true, 3, &tex, { 0, 0 }, { 0, 0 } };
    Sprite s;
    s.Update(d, 0x480);
    bool ok = s.ClipBox.TLX == 80 && s.ClipBox.TLY == 90 && s.ClipBox.BRX == 120 && s.ClipBox.BRY == 110 && s.Filter
        && !s.WrapTexture && s.EnginePrimitiveFlags == 0x480 && s.RenderLayerMask == 0x00200000 && g_loads == 1 && s.InterpolationSet == 3;
    d.Flags = 0; d.UVEnd.X = 0.5f;
    s.Update(d, 0);
    ok = ok && s.ClipBox.BRX == 140 && s.ClipBox.BRY == 120 && s.ClipBox.TLX == 100 && !s.Filter && s.WrapTexture && s.RenderLayerMask == 0x80000000;
    d.BankIndex = 0;
    s.Update(d, 0);
    ok = ok && s.ClipBox.BRX == 164 && s.ClipBox.BRY == 132 && g_loads == 2;
    d.ClipBoxIsPos = false;
    s.Update(d, 0);
    ok = ok && s.ClipBox.BRX == 100;
    if (!ok) { std::printf("00bad8a0_TEST FAIL\n"); return 1; }
    std::printf("00bad8a0_TEST PASS\n");
    return 0;
}