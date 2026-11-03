// Behaviour test for CEngine::PeekGlobalAmbientColour (0xb25d50): theme channel 0's Ambient colour of the lighting manager, by value.
#include <cstdio>
struct Colour { float R, G, B, A; };
struct Channel { Colour c[5]; };                 // Diffuse, Ambient, Backlight, Reflection, Fog
struct Manager { Channel* start; const Colour& Peek() const { return start[0].c[1]; } };
static Manager* g_manager;
static Colour PeekGlobalAmbientColour() { return g_manager->Peek(); }
int main()
{
    Channel ch[2]; for (int k = 0; k < 2; ++k) for (int j = 0; j < 5; ++j) { ch[k].c[j].R = (float)(k * 10 + j); ch[k].c[j].G = 0.5f; ch[k].c[j].B = 0.25f; ch[k].c[j].A = 1.0f; }
    Manager m; m.start = ch; g_manager = &m;
    Colour c = PeekGlobalAmbientColour();
    if (c.R != (float)1 || c.G != 0.5f || c.A != 1.0f) { std::printf("00b25d50_TEST FAIL\n"); return 1; }
    ch[0].c[1].R = 42.0f; if (PeekGlobalAmbientColour().R != 42.0f || c.R != (float)1) { std::printf("00b25d50_TEST FAIL\n"); return 1; }
    std::printf("00b25d50_TEST PASS\n");
    return 0;
}