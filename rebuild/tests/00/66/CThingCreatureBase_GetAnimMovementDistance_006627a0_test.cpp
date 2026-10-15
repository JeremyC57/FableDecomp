#include <cstdio>
#include <cstring>
#include <math.h>

struct C3DVector { float X, Y, Z; };
struct C3DAnimationInfo { float Duration; C3DVector MovementVector; };
struct CMeshDataBank { C3DAnimationInfo m_infos[4]; C3DAnimationInfo* GetAnimation(long index) { return &m_infos[index]; } };
struct CWorld { CMeshDataBank* PMainMeshBank; };
static CMeshDataBank g_bank; static CWorld g_world;
static int g_worldPeeks = 0;

inline float Magnitude(float x, float y, float z) { return (float)sqrt(x * x + y * y + z * z); }
struct CThing { const CWorld& PeekWorld() const { ++g_worldPeeks; return g_world; } };
struct CThingCreatureBase : CThing {
    float GetAnimMovementDistance(long animIndex) const
    {
        if (animIndex) {
            CMeshDataBank* bank = PeekWorld().PMainMeshBank;
            const C3DVector& v = bank->GetAnimation(animIndex)->MovementVector;
            return Magnitude(v.X, v.Y, v.Z) * 0.01f;
        }
        return 0.0f;
    }
};
int main()
{
    g_world.PMainMeshBank = &g_bank;
    g_bank.m_infos[2].MovementVector.X = 300.0f; g_bank.m_infos[2].MovementVector.Y = 0.0f; g_bank.m_infos[2].MovementVector.Z = 400.0f;
    CThingCreatureBase c;
    float d = c.GetAnimMovementDistance(2);
    if (d < 4.999f || d > 5.001f || g_worldPeeks != 1) { std::printf("006627a0_TEST FAIL\n"); return 1; }
    if (c.GetAnimMovementDistance(0) != 0.0f || g_worldPeeks != 1) { std::printf("006627a0_TEST FAIL\n"); return 1; }
    std::printf("006627a0_TEST PASS\n");
    return 0;
}