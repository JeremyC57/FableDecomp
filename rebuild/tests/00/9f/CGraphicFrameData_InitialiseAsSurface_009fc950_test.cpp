// Behaviour test for CGraphicFrameData::InitialiseAsSurface (0x009FC950): a model of the surface set-up -- the texture shared with
// count bookkeeping, the size / pitch / flags, origin 0,0, and the screen-graphic flag when the frame size differs from the
// texture's level-0 width (retail compares both dimensions with the width).
#include <cstdio>
struct Texture { long w, h; long GetWidth(unsigned long) const { return w; } };
struct Info { long refs; };
struct Frame {
    Texture* tex; Info* info; unsigned short TLX, TLY, Width, Height, Pitch;
    bool Alpha, BooleanAlpha, UsesSurface, SectionOfTPage, IsInitialised, ScreenGraphic;
    bool InitialiseAsSurface(Texture* t, Info* i, long width, long height, bool alpha, bool booleanAlpha)
    {
        UsesSurface = true;
        if (info != i) { if (info) --info->refs; tex = t; info = i; if (i) ++i->refs; }
        Width = (unsigned short)width; Height = (unsigned short)height; Pitch = (unsigned short)width;
        Alpha = alpha; BooleanAlpha = booleanAlpha; SectionOfTPage = false; IsInitialised = true; TLX = 0; TLY = 0;
        ScreenGraphic = Width != t->GetWidth(0) || Height != t->GetWidth(0);
        return true;
    }
};
int main()
{
    Texture t; t.w = 64; t.h = 32; Info in; in.refs = 1; Info old; old.refs = 1;
    Frame f; f.tex = 0; f.info = &old; f.SectionOfTPage = true; f.TLX = 5; f.TLY = 6;
    if (!f.InitialiseAsSurface(&t, &in, 64, 64, true, false)) { std::printf("009fc950_TEST FAIL\n"); return 1; }
    if (in.refs != 2 || old.refs != 0 || f.Pitch != 64 || f.Height != 64 || f.ScreenGraphic || f.SectionOfTPage || !f.IsInitialised
        || !f.Alpha || f.BooleanAlpha || f.TLX || f.TLY || !f.UsesSurface) { std::printf("009fc950_TEST FAIL\n"); return 1; }
    f.InitialiseAsSurface(&t, &in, 64, 32, false, true);   // same texture: no count change; height differs from the width
    if (in.refs != 2 || !f.ScreenGraphic || !f.BooleanAlpha) { std::printf("009fc950_TEST FAIL\n"); return 1; }
    std::printf("009fc950_TEST PASS\n");
    return 0;
}