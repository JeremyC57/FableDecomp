#include <cstdio>
#ifdef _MSC_VER
#pragma optimize("y", off)
__forceinline long FloorToLong(float value)
{
    long result; float one; float half; float next;
    half = 0.5f; one = 1.0f;
    __asm {
        fld value
        fsub half
        fistp result
        fild result
        fadd one
        fstp next
        mov eax, value
        cmp eax, next
        jne done
        inc result
    done:
    }
    return result;
}
#else
#include <math.h>
inline long FloorToLong(float value) { return (long)floor((double)value); }   // the FPU block is floor() for the magnitudes used here
#endif
#include <math.h>
class C3DVector { public: float X, Y, Z; };
struct SCamera_01436ea0 { unsigned char _pad[0x54]; C3DVector Position; };
static SCamera_01436ea0 g_camera; SCamera_01436ea0* g_pCamera_01436ea0 = &g_camera;
static int g_rebuilt = 0;
class CEngineWeatherRenderer { public:
    unsigned char _pad000[0x11a]; bool FreezeBoxes; unsigned char _pad11b[0x1d4 - 0x11b]; C3DVector BoxSize; unsigned char _pad1e0[0x1f0 - 0x1e0]; long BoxCentre[3];
    void RebuildBoxes() { ++g_rebuilt; }
    void UpdateBoxPositions()
    {
        if (FreezeBoxes) return;
        long sizeX = FloorToLong(BoxSize.X * 0.5f);
        long sizeY = FloorToLong(BoxSize.Y * 0.5f);
        long sizeZ = FloorToLong(BoxSize.Z * 0.5f);
        const C3DVector& camera = g_pCamera_01436ea0->Position;
        long distX = FloorToLong((float)fabs((float)BoxCentre[0] - camera.X));
        long distY = FloorToLong((float)fabs((float)BoxCentre[1] - camera.Y));
        long distZ = FloorToLong((float)fabs((float)BoxCentre[2] - camera.Z));
        if (distX >= sizeX || distY >= sizeY || distZ >= sizeZ) RebuildBoxes();
    }
};
int main()
{
    static CEngineWeatherRenderer w; w.FreezeBoxes = false; w.BoxSize.X = 20.0f; w.BoxSize.Y = 20.0f; w.BoxSize.Z = 8.0f;
    w.BoxCentre[0] = 100; w.BoxCentre[1] = 100; w.BoxCentre[2] = 10;
    g_camera.Position.X = 105.0f; g_camera.Position.Y = 95.5f; g_camera.Position.Z = 12.0f;
    w.UpdateBoxPositions(); bool ok = g_rebuilt == 0;                 // 5 < 10, 4 < 10, 2 < 4
    g_camera.Position.Z = 14.5f; w.UpdateBoxPositions(); ok = ok && g_rebuilt == 1;   // floor(4.5) = 4 >= 4
    g_camera.Position.Z = 12.0f; g_camera.Position.X = 89.0f; w.UpdateBoxPositions(); ok = ok && g_rebuilt == 2;   // 11 >= 10
    w.FreezeBoxes = true; w.UpdateBoxPositions(); ok = ok && g_rebuilt == 2;
    std::printf(ok ? "00b52990_TEST PASS\n" : "00b52990_TEST FAIL\n");
    return ok ? 0 : 1;
}