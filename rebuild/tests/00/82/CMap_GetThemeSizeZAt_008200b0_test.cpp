#include <cstdio>
#include <cstring>
#include <new>

class C2DVector { public: float X; float Y; }; class C2DCoordI { public: long X; long Y; };
static long g_cx, g_cy; static const C2DVector* g_pos;
class CMap { public:
    float GetThemeMinCameraSizeZOffsetAt(C2DCoordI* cell) { g_cx = cell->X; g_cy = cell->Y; return (float)(cell->X * 10 + cell->Y); }
    float InterpolateHeightAtPosition(const C2DVector& pos) { g_pos = &pos; return 0.5f; }
    float HeightAt(const C2DVector& pos) { C2DCoordI cell; cell.X = (long)pos.X; cell.Y = (long)pos.Y;
        float offset = GetThemeMinCameraSizeZOffsetAt(&cell); return offset + InterpolateHeightAtPosition(pos); }
};
int main()
{
    CMap map; C2DVector pos; pos.X = 3.7f; pos.Y = 2.2f;
    float h = map.HeightAt(pos);
    if (g_cx != 3 || g_cy != 2 || g_pos != &pos || h != 32.5f) { std::printf("008200b0_TEST FAIL\n"); return 1; }
    std::printf("008200b0_TEST PASS\n");
    return 0;
}