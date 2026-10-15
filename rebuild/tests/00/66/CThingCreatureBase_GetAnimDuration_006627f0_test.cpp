#include <cstdio>
#include <cstring>
#include <math.h>

struct C3DVector { float X, Y, Z; };
struct C3DAnimationInfo { float Duration; C3DVector MovementVector; };
struct CMeshDataBank { C3DAnimationInfo m_infos[4]; C3DAnimationInfo* GetAnimation(long index) { return &m_infos[index]; } };
struct CWorld { CMeshDataBank* PMainMeshBank; };
static CMeshDataBank g_bank; static CWorld g_world;
static int g_worldPeeks = 0;

struct CThing { const CWorld& PeekWorld() const { ++g_worldPeeks; return g_world; } };
struct CThingCreatureBase : CThing {
    float GetAnimDuration(long animIndex) const
    {
        if (animIndex) {
            CMeshDataBank* bank = PeekWorld().PMainMeshBank;
            return bank->GetAnimation(animIndex)->Duration;
        }
        return 0.0f;
    }
};
int main()
{
    g_world.PMainMeshBank = &g_bank; g_bank.m_infos[1].Duration = 2.5f; g_bank.m_infos[3].Duration = 0.75f;
    CThingCreatureBase c;
    if (c.GetAnimDuration(1) != 2.5f || c.GetAnimDuration(3) != 0.75f || g_worldPeeks != 2) { std::printf("006627f0_TEST FAIL\n"); return 1; }
    if (c.GetAnimDuration(0) != 0.0f || g_worldPeeks != 2) { std::printf("006627f0_TEST FAIL\n"); return 1; }
    std::printf("006627f0_TEST PASS\n");
    return 0;
}