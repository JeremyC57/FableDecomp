// Behaviour test for 0x00861950 NCreatureMode::GetMovementSpeedFromAnimation(long, ...) (chunk 402): movement-vector
// length * 0.01 * render size * physical scale / (frames per second * duration), 0 for a duration <= 1e-4.
#include <cstdio>
#include <cmath>
#if !defined(_MSC_VER)
#define __fastcall
#endif

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float GetLength() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
};
class C3DAnimationInfo {
public:
    float     Duration;
    C3DVector MovementVector;
    C3DAnimationInfo() : Duration(0.0f), MovementVector(0.0f, 0.0f, 0.0f) {}
};
class CMeshDataBank {
public:
    C3DAnimationInfo Infos[4];
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const { return &Infos[index]; }
};
class CTCGraphicAppearance {
public:
    float RenderSizeX;
};
class CThingCreatureBase {
public:
    CTCGraphicAppearance* PGraphicAppearance;
    float                 Scale;
};
long g_FramesPerSecond = 15;
namespace NCreatureMode {
float __fastcall GetMovementSpeedFromAnimation(long bankIndex, const CThingCreatureBase& creature, const CMeshDataBank& bank);
}

float __fastcall NCreatureMode::GetMovementSpeedFromAnimation(long bankIndex, const CThingCreatureBase& creature, const CMeshDataBank& bank)
{
    const C3DAnimationInfo* info = bank.PeekAnimation(bankIndex);
    float duration = info->Duration;
    if (duration > 0.0001f) {
        float distance = info->MovementVector.GetLength() * 0.01f;
        return distance * creature.PGraphicAppearance->RenderSizeX * creature.Scale / (g_FramesPerSecond * duration);
    }
    return 0.0f;
}

static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }

int main()
{
    int failures = 0;
    CMeshDataBank bank;
    bank.Infos[1].Duration = 2.0f;
    bank.Infos[1].MovementVector = C3DVector(3.0f, 4.0f, 0.0f);    // length 5
    bank.Infos[2].Duration = 0.00005f;
    bank.Infos[2].MovementVector = C3DVector(1.0f, 2.0f, 2.0f);
    bank.Infos[3].Duration = 1.0f;
    bank.Infos[3].MovementVector = C3DVector(0.0f, 2.0f, 0.0f);
    CTCGraphicAppearance appearance;
    appearance.RenderSizeX = 200.0f;
    CThingCreatureBase creature;
    creature.PGraphicAppearance = &appearance;
    creature.Scale = 1.5f;
    // 5 * 0.01 * 200 * 1.5 / (15 * 2) = 0.5
    if (!Near(NCreatureMode::GetMovementSpeedFromAnimation(1, creature, bank), 0.5f)) { printf("  speed 1 wrong\n"); ++failures; }
    if (NCreatureMode::GetMovementSpeedFromAnimation(2, creature, bank) != 0.0f) { printf("  tiny duration not 0\n"); ++failures; }
    if (NCreatureMode::GetMovementSpeedFromAnimation(0, creature, bank) != 0.0f) { printf("  zero duration not 0\n"); ++failures; }
    g_FramesPerSecond = 30;
    // 2 * 0.01 * 200 * 1.5 / (30 * 1) = 0.2
    if (!Near(NCreatureMode::GetMovementSpeedFromAnimation(3, creature, bank), 0.2f)) { printf("  speed 3 wrong\n"); ++failures; }
    if (failures == 0) {
        printf("00861950_TEST PASS\n");
        return 0;
    }
    printf("00861950_TEST FAIL\n");
    return 1;
}