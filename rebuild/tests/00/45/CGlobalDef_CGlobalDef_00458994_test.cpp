#include <cstdio>
// Behaviour model of CGlobalDef::CGlobalDef (0x00458994): member constructors zero the engine graphics and vectors.
struct Str { const char* p; Str() : p("") {} };
struct Graphic { unsigned int BankIndex; float AnimStep, RenderSizeX; bool AdditiveAlpha; unsigned char Type;
                 Graphic() : BankIndex(0), AnimStep(0), RenderSizeX(0), AdditiveAlpha(false), Type(0) {} };
struct Vec { void* a; void* b; void* c; Vec() : a(0), b(0), c(0) {} };
struct GlobalDef { Str Gold, Item, Click, Mime; Graphic Switch, Track; Vec Narrators, Layers; };
int main()
{
    GlobalDef d;
    if (d.Switch.BankIndex || d.Track.AnimStep != 0 || d.Track.Type || d.Narrators.a || d.Layers.c || d.Mime.p[0]) { std::printf("00458994_TEST FAIL\n"); return 1; }
    std::printf("00458994_TEST PASS\n");
    return 0;
}