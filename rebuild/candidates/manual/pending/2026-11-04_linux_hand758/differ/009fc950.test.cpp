// Behaviour test for CGraphicFrameData::InitialiseAsSurface (0x009FC950), g++ model: whole-texture surface; ScreenGraphic when Width or Height differs from the level-0 extents' X (retail compares both with X).
#include <cstdio>
struct Frame { const void* texture; unsigned short TLX, TLY, Width, Height, Pitch; bool Alpha, BooleanAlpha, UsesSurface, SectionOfTPage, IsInitialised, ScreenGraphic; };
static bool InitialiseAsSurface(Frame& f, const void* tex, long dimX, long width, long height, bool alpha, bool booleanAlpha)
{
    f.UsesSurface = true; f.texture = tex;
    f.Width = (unsigned short)width; f.Pitch = (unsigned short)width; f.Height = (unsigned short)height;
    f.Alpha = alpha; f.BooleanAlpha = booleanAlpha; f.SectionOfTPage = false; f.IsInitialised = true; f.TLX = 0; f.TLY = 0;
    f.ScreenGraphic = f.Width != dimX || f.Height != dimX;
    return true;
}
int main()
{
    int tex = 0; Frame f = { 0, 5, 5, 0, 0, 0, false, false, false, true, false, true };
    int ok = InitialiseAsSurface(f, &tex, 256, 256, 256, true, false) && !f.ScreenGraphic && f.Pitch == 256 && f.TLX == 0 && f.UsesSurface
          && !f.SectionOfTPage && f.IsInitialised && f.Alpha && !f.BooleanAlpha && f.texture == &tex;
    InitialiseAsSurface(f, &tex, 256, 256, 128, false, true); ok = ok && f.ScreenGraphic && f.BooleanAlpha;
    InitialiseAsSurface(f, &tex, 256, 640, 256, false, false); ok = ok && f.ScreenGraphic;
    std::printf(ok ? "009fc950_TEST PASS\n" : "009fc950_TEST FAIL\n");
    return ok ? 0 : 1;
}
