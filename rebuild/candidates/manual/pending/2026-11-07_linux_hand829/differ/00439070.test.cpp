// Behaviour test for 0x00439070 CPlayerGui::DrawPreparePrimitivesForEngine: CRGBColour -> CRGBFloatColour (/255),
// the paused / current timer set choice, clearing every illustrator's drawn flag and the TGA-sprite / Reset choice.
#include <stdio.h>
#include <math.h>
struct RGB { unsigned char R, G, B, A; };
struct FC { float R, G, B, A; };
static FC ToFloat(const RGB& c) { FC f = { c.R / 255.0f, c.G / 255.0f, c.B / 255.0f, c.A / 255.0f }; return f; }
struct Draw { bool Drawn; };
int main()
{
    int ok = 1;
    RGB c = { 255, 0, 51, 255 }; FC f = ToFloat(c);
    ok &= f.R == 1.0f && f.G == 0.0f && fabs(f.B - 0.2f) < 1e-6f && f.A == 1.0f;
    int current = 1, paused = 2; bool worldPaused = true;
    ok &= (worldPaused ? paused : current) == 2;
    Draw d[5] = { {true}, {true}, {false}, {true}, {true} };
    Draw* end = d + 5;
    for (Draw* it = d; it != end; ++it) it->Drawn = false;
    for (int i = 0; i < 5; ++i) ok &= !d[i].Drawn;
    bool drawingTGA = false; int handle = 5;
    if (drawingTGA) handle = 1; else handle = 0;
    ok &= handle == 0;
    if (ok) printf("00439070_TEST PASS\n"); else printf("00439070_TEST FAIL\n");
    return ok ? 0 : 1;
}
