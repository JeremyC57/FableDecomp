// Behaviour test for ComputeAzimuthElevation (0x00C599C0): elevation is the angle of y over the horizontal length (signed
// by y), azimuth the angle of x around z in degrees (0 ahead, 90 to the side, 180 behind, negative for x < 0, -180 folded
// to 180), and an output in mode 2 forces azimuth 0.
#include <cstdio>
#include <cmath>
class C3DVector { public: float X, Y, Z; };
static long g_outputMode = 0;
class CCodaSystem { public: long GetOutputMode() { return g_outputMode; } };
static CCodaSystem g_codaSystem;
static CCodaSystem* g_pCodaSystem = &g_codaSystem;
static void ComputeAzimuthElevation(float* azimuth, float* elevation, const C3DVector* direction)
{
    float y = direction->Y < 0.0f ? -direction->Y : direction->Y;
    float horizontal = (float)sqrt(direction->X * direction->X + direction->Z * direction->Z);
    if (y < horizontal)
        *elevation = (float)atan2(y, horizontal) * 57.29578f;
    else
        *elevation = 90.0f - (float)atan2(horizontal, y) * 57.29578f;
    if (direction->Y < 0.0f)
        *elevation = -*elevation;
    float x = direction->X < 0.0f ? -direction->X : direction->X;
    float z = direction->Z < 0.0f ? -direction->Z : direction->Z;
    if (z > x)
        *azimuth = (float)atan2(x, z) * 57.29578f;
    else if (x == 0.0f)
        *azimuth = 0.0f;
    else
        *azimuth = 90.0f - (float)atan2(z, x) * 57.29578f;
    if (direction->Z < 0.0f)
        *azimuth = 180.0f - *azimuth;
    if (direction->X < 0.0f)
        *azimuth = -*azimuth;
    if (*azimuth <= -180.0f)
        *azimuth = 180.0f;
    if (g_pCodaSystem->GetOutputMode() == 2)
        *azimuth = 0.0f;
}
static int near(float a, float b) { return std::fabs(a - b) < 0.01f; }
static int check(float x, float y, float z, float az, float el)
{
    C3DVector v; v.X = x; v.Y = y; v.Z = z;
    float a = -1234.0f, e = -1234.0f;
    ComputeAzimuthElevation(&a, &e, &v);
    return near(a, az) && near(e, el);
}
int main()
{
    int ok = check(0.0f, 0.0f, 1.0f, 0.0f, 0.0f)          // straight ahead
          && check(1.0f, 0.0f, 0.0f, 90.0f, 0.0f)        // right
          && check(-1.0f, 0.0f, 0.0f, -90.0f, 0.0f)      // left
          && check(0.0f, 0.0f, -1.0f, 180.0f, 0.0f)      // behind
          && check(1.0f, 0.0f, 1.0f, 45.0f, 0.0f)        // front right diagonal (x == z takes the 90 - atan arm)
          && check(-1.0f, 0.0f, -1.0f, -135.0f, 0.0f)    // back left diagonal
          && check(0.0f, 1.0f, 0.0f, 0.0f, 90.0f)        // above (no horizontal length)
          && check(0.0f, -1.0f, 0.0f, 0.0f, -90.0f)      // below
          && check(0.0f, 1.0f, 1.0f, 0.0f, 45.0f)        // 45 degrees up
          && check(-1e-30f, 0.0f, -1.0f, 180.0f, 0.0f);  // -180 folds to 180
    g_outputMode = 2;
    ok = ok && check(1.0f, 1.0f, 1.0f, 0.0f, 35.26f);   // mode 2: azimuth forced to 0, elevation kept
    if (!ok) { std::printf("00c599c0_TEST FAIL\n"); return 1; }
    std::printf("00c599c0_TEST PASS\n");
    return 0;
}