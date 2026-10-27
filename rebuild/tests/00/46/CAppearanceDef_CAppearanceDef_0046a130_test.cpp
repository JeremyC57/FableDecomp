// behaviour test for 0x0046a130 CAppearanceDef::CAppearanceDef(): sub-def base, then both engine graphics default
// (bank 0, zero floats, no additive alpha, type 0) and the animation set built by its own constructor; the remaining
// plain members stay as the base left them
#include <cstdio>
static int gSubDefCtors = 0, gAnimSetCtors = 0;
struct SubDefBase { int vt; SubDefBase() : vt(1) { ++gSubDefCtors; } };
struct Graphic { unsigned int BankIndex; float AnimStep, RenderSizeX; bool AdditiveAlpha; unsigned char Type;
    Graphic() : BankIndex(0), AnimStep(0.0f), RenderSizeX(0.0f), AdditiveAlpha(false), Type(0) { } };
struct AnimSet { int DefaultGroup; AnimSet() : DefaultGroup(-1) { ++gAnimSetCtors; } };
struct AppearanceDef : SubDefBase { Graphic G; AnimSet A; unsigned char AlphaRef; Graphic Fade; bool HasFade; };
int main()
{
    int fails = 0;
    AppearanceDef d;
    if (gSubDefCtors != 1 || gAnimSetCtors != 1) fails |= 1;
    if (d.G.BankIndex || d.G.AnimStep != 0.0f || d.G.RenderSizeX != 0.0f || d.G.AdditiveAlpha || d.G.Type) fails |= 2;
    if (d.Fade.BankIndex || d.Fade.AnimStep != 0.0f || d.Fade.RenderSizeX != 0.0f || d.Fade.AdditiveAlpha || d.Fade.Type) fails |= 4;
    if (d.A.DefaultGroup != -1) fails |= 8;
    if (fails) { std::printf("0046a130_TEST FAIL %d\n", fails); return 1; }
    std::printf("0046a130_TEST PASS\n");
    return 0;
}