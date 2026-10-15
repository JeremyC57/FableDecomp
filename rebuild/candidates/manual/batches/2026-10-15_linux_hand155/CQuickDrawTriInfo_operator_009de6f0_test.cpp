// Behaviour test for 0x009DE6F0: CQuickDrawTriInfo::operator= assigns each bit field, the window, string, texture, both union views and the tail.
#include <cstdio>
struct Box { float TLX, TLY, BRX, BRY; };
struct WStr { int id; int assigns; WStr& operator=(const WStr& o) { id = o.id; ++assigns; return *this; } };
struct Tex { int id; int assigns; Tex& operator=(const Tex& o) { id = o.id; ++assigns; return *this; } };
struct Info { unsigned char Line : 1; unsigned char IsText : 1; unsigned char Other : 6; Box Window; WStr String; Tex UploadedTexture;
  union { struct { unsigned int BlendMode; float ZAverage; } s0; struct { float StringX; float StringY; } s1; } u;
  float StringZ, StringScale; unsigned int StringFlags; unsigned int Colour; void* PFont;
  Info& operator=(const Info& o) { Line = o.Line; IsText = o.IsText; Window = o.Window; String = o.String; UploadedTexture = o.UploadedTexture;
    u.s0.BlendMode = o.u.s0.BlendMode; u.s0.ZAverage = o.u.s0.ZAverage; u.s1.StringX = o.u.s1.StringX; u.s1.StringY = o.u.s1.StringY;
    StringZ = o.StringZ; StringScale = o.StringScale; StringFlags = o.StringFlags; Colour = o.Colour; PFont = o.PFont; return *this; } };
int main()
{
    Info a, b; a.Line = 1; a.IsText = 0; a.Other = 5; a.Window.TLX = 1.0f; a.Window.BRY = 4.0f; a.String.id = 7; a.UploadedTexture.id = 8;
    a.u.s1.StringX = 2.5f; a.u.s1.StringY = 3.5f; a.StringZ = 0.5f; a.StringScale = 2.0f; a.StringFlags = 3; a.Colour = 0xff00ff00u; a.PFont = &a;
    b.Line = 0; b.IsText = 1; b.Other = 9; b.String.assigns = 0; b.UploadedTexture.assigns = 0;
    Info& r = (b = a);
    bool ok = &r == &b && b.Line == 1 && b.IsText == 0 && b.Other == 9 && b.Window.TLX == 1.0f && b.Window.BRY == 4.0f && b.String.id == 7 && b.String.assigns == 1
           && b.UploadedTexture.id == 8 && b.u.s1.StringX == 2.5f && b.u.s1.StringY == 3.5f && b.StringZ == 0.5f && b.StringFlags == 3 && b.Colour == 0xff00ff00u && b.PFont == &a;
    if (!ok) { std::printf("009de6f0_TEST FAIL\n"); return 1; }
    std::printf("009de6f0_TEST PASS\n");
    return 0;
}
