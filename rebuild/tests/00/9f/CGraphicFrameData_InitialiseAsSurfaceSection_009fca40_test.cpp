// Behaviour test for CGraphicFrameData::InitialiseAsSurfaceSection (0x009FCA40): the top-left corner is the normalised section
// coordinate times 32767 rounded to nearest (GFFastFloatToLong), size / pitch / flags are copied, the frame becomes an initialised
// surface section that is not a screen graphic.
#include <stdio.h>
#include "engine/GFFastFloatToLong.h"
#pragma pack(push, 1)
struct Frame {
    unsigned short TLX, TLY, Width, Height, Pitch;
    unsigned char Alpha : 1, BooleanAlpha : 1, UsesSurface : 1, SectionOfTPage : 1, IsInitialised : 1, ScreenGraphic : 1;
    bool Init(float tlx, float tly, long width, long height, bool alpha, bool booleanAlpha)
    {
        UsesSurface = true;
        Width = (unsigned short)width;
        Pitch = (unsigned short)width;
        Height = (unsigned short)height;
        Alpha = alpha;
        BooleanAlpha = booleanAlpha;
        TLX = (unsigned short)GFFastFloatToLong(tlx * 32767.0f);
        TLY = (unsigned short)GFFastFloatToLong(tly * 32767.0f);
        ScreenGraphic = false;
        SectionOfTPage = true;
        IsInitialised = true;
        return true;
    }
};
#pragma pack(pop)
int main()
{
    Frame f;
    f.ScreenGraphic = true;
    bool r = f.Init(0.5f, 0.25f, 64, 32, true, false);
    bool ok = r && f.TLX == 16384 && f.TLY == 8192 && f.Width == 64 && f.Pitch == 64 && f.Height == 32 && f.Alpha && !f.BooleanAlpha
              && f.UsesSurface && f.SectionOfTPage && f.IsInitialised && !f.ScreenGraphic;
    f.Init(1.0f, 0.0f, 8, 8, false, true);
    ok = ok && f.TLX == 32767 && f.TLY == 0 && !f.Alpha && f.BooleanAlpha;
    printf(ok ? "009fca40_TEST PASS\n" : "009fca40_TEST FAIL\n");
    return ok ? 0 : 1;
}