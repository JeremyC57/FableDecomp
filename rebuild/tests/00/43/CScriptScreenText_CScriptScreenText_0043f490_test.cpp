#include <cstdio>
#include <stddef.h>
static int g_wide = 0, g_narrow = 0;
class CWideString { public: void* PStringData; CWideString(void* p) : PStringData(p) { } CWideString(const CWideString& o) : PStringData(o.PStringData) { ++g_wide; } };
class CCharString { public: void* PStringData; CCharString(void* p) : PStringData(p) { } CCharString(const CCharString& o) : PStringData(o.PStringData) { ++g_narrow; } };
class C2DVector { public: float X, Y; };
class CRGBColour { public: unsigned char B, G, R, A; void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } CRGBColour() { } CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); } };
struct CScriptScreenText {
    CWideString Text; C2DVector VPos; CRGBColour Colour; CCharString FontName; unsigned long FrameAdded;
    CScriptScreenText() : Text((void*)1), FontName((void*)2) { }
    CScriptScreenText(const CScriptScreenText& o) : Text(o.Text), VPos(o.VPos), Colour(o.Colour), FontName(o.FontName), FrameAdded(o.FrameAdded) { }
};
int main()
{
    CScriptScreenText a; a.VPos.X = 1.5f; a.VPos.Y = -2.0f; a.Colour.Set(10, 20, 30, 40); a.FrameAdded = 999;
    CScriptScreenText b(a);
    if (g_wide != 1 || g_narrow != 1 || b.Text.PStringData != (void*)1 || b.FontName.PStringData != (void*)2 || b.VPos.X != 1.5f || b.VPos.Y != -2.0f
        || b.Colour.R != 10 || b.Colour.G != 20 || b.Colour.B != 30 || b.Colour.A != 40 || b.FrameAdded != 999
        || offsetof(CScriptScreenText, Colour) != 0xc || offsetof(CScriptScreenText, FontName) != 0x10 || sizeof(b) != 0x18 || offsetof(CRGBColour, R) != 2) { std::printf("0043f490_TEST FAIL\n"); return 1; }
    std::printf("0043f490_TEST PASS\n"); return 0;
}