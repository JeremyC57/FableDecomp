// Behaviour test for CHeightMap::Clear (0x008F6050): a model of the row -- every cell of the (SizeX+1) x (SizeY+1) grid is reset to
// height 0, palette indices {1,1,1}, blend {0xff, 0}, Changed/Passable/ShorePoint/NavPreferred clear and CameraPassable set.
#include <stdio.h>
struct Cell { float Height; unsigned char Palette[3]; unsigned char Blend[2];
    unsigned char Changed : 1, Passable : 1, ShorePoint : 1, CameraPassable : 1, NavPreferred : 1; unsigned char pad[2];
    void Clear() { Changed = 0; Height = 0.0f; Blend[0] = 0xff; Blend[1] = 0; for (long i = 0; i < 3; ++i) Palette[i] = 1;
                   Passable = 0; ShorePoint = 0; CameraPassable = 1; NavPreferred = 0; } };
int main()
{
    const long sx = 3, sy = 2; Cell cells[sx * sy];
    for (long i = 0; i < sx * sy; ++i) { cells[i].Height = 5.0f; cells[i].Palette[0] = 9; cells[i].Changed = 1; cells[i].Passable = 1;
        cells[i].ShorePoint = 1; cells[i].CameraPassable = 0; cells[i].NavPreferred = 1; cells[i].Blend[0] = 3; cells[i].Blend[1] = 4; }
    for (long y = 0; y < sy; ++y) for (long x = 0; x < sx; ++x) cells[sx * y + x].Clear();
    int ok = 1;
    for (long i = 0; i < sx * sy; ++i)
        ok = ok && cells[i].Height == 0.0f && cells[i].Palette[0] == 1 && cells[i].Palette[2] == 1 && cells[i].Blend[0] == 0xff &&
             cells[i].Blend[1] == 0 && !cells[i].Changed && !cells[i].Passable && !cells[i].ShorePoint && cells[i].CameraPassable && !cells[i].NavPreferred;
    printf(ok ? "008f6050_TEST PASS\n" : "008f6050_TEST FAIL\n");
    return ok ? 0 : 1;
}