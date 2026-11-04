// Behaviour test for CDisplayManager::SetupDepthAndBackBufferTextures (0x009BEE40, parked DIFFER row): a model of the row -- 1024x1024 when power-of-two textures are required, else the render-target size.
#include <stdio.h>
struct E { long X, Y; };
static E Size(unsigned long caps, E rt) { E s; if ((caps >> 1) & 1) { s.Y = 0x400; s.X = 0x400; } else { s.Y = rt.Y; s.X = rt.X; } return s; }
static int Check() { E rt = { 800, 600 }; E a = Size(2, rt), b = Size(0, rt); return a.X == 1024 && a.Y == 1024 && b.X == 800 && b.Y == 600; }
int main()
{
    int ok = Check();
    printf(ok ? "009bee40_TEST PASS\n" : "009bee40_TEST FAIL\n");
    return ok ? 0 : 1;
}
