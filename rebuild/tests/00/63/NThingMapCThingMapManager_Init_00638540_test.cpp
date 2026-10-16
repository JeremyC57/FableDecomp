#include <cstdio>
struct Box { long TLX, TLY, BRX, BRY; long GetWidth() const { return BRX - TLX; } long GetHeight() const { return BRY - TLY; } };
struct WorldMap { Box b; const Box& GetBounds() const { return b; } };
struct World { WorldMap m; WorldMap* GetWorldMap() { return &m; } };
static int g_init = 0, g_clear = 0;
struct Mgr { World* PWorld; unsigned int cells; long Width, Height;
    void Init(World* w) { ++g_init; PWorld = w; ++g_clear;
        Width = w->GetWorldMap()->GetBounds().GetWidth() / 128;
        Height = w->GetWorldMap()->GetBounds().GetHeight() / 128;
        cells = Width * Height; } };
int main()
{
    World w = { { { -256, 0, 512, 384 } } };
    Mgr m = { 0, 0, 0, 0 };
    m.Init(&w);
    bool ok = m.PWorld == &w && m.Width == 6 && m.Height == 3 && m.cells == 18 && g_init == 1 && g_clear == 1;
    if (!ok) { std::printf("00638540_TEST FAIL\n"); return 1; }
    std::printf("00638540_TEST PASS\n");
    return 0;
}