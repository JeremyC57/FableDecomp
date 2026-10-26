#include <cstdio>
#include <cstring>
#include <math.h>

class C3DVector { public: float X, Y, Z; };
class C3DAnimationInfo { public: float Duration; C3DVector MovementVector; };
class CMeshDataBank { public: C3DAnimationInfo m_infos[4]; C3DAnimationInfo* GetAnimation(long index) { return &m_infos[index]; } };
class CWorld { public: CMeshDataBank* PMainMeshBank; };
static CMeshDataBank g_bank; static CWorld g_world;
static int g_worldPeeks = 0;

class CThing { public: const CWorld& PeekWorld() const { ++g_worldPeeks; return g_world; } };
class CThingCreatureBase : public CThing { public:
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