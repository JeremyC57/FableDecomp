// Behaviour test for 0x006E7370 (CGameScriptInterface::StartFadeScreen: forward to the display engine, times swapped, hold -1).
#include <cstdio>

struct Colour {
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    Colour() { }
    Colour(const Colour& o) { Set(o.R, o.G, o.B, o.A); }
};
struct Display {
    float outTime, inTime, hold; Colour colour; int calls;
    void StartScreenFadeOut(float o, float i, Colour c, float h) { outTime = o; inTime = i; colour = c; hold = h; ++calls; }
};
struct ScriptInterface {
    Display* DisplayEngine;
    void StartFadeScreen(float fadeInTime, float fadeOutTime, Colour colour)
    {
        DisplayEngine->StartScreenFadeOut(fadeOutTime, fadeInTime, colour, -1.0f);
    }
};
#define FAIL { std::printf("006e7370_TEST FAIL\n"); return 1; }
int main()
{
    Display d; d.calls = 0;
    ScriptInterface si; si.DisplayEngine = &d;
    Colour c; c.Set(10, 20, 30, 255);
    si.StartFadeScreen(1.5f, 2.5f, c);
    if (d.calls != 1 || d.outTime != 2.5f || d.inTime != 1.5f || d.hold != -1.0f) FAIL;
    if (d.colour.R != 10 || d.colour.G != 20 || d.colour.B != 30 || d.colour.A != 255) FAIL;
    std::printf("006e7370_TEST PASS\n");
    return 0;
}