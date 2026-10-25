// Behaviour test for CAnimationEntry::GetRotationInAnimation @ 0x005dbfd0: the negated rotation of the main mesh bank's animation named by BankIndex.
#include <cstdio>
class C3DAnimationInfo { public: unsigned char _before_Rotation[0x24]; float Rotation; };
static C3DAnimationInfo g_animations[3];
class CMeshDataBank { public: const C3DAnimationInfo* PeekAnimation(unsigned long index) const { return &g_animations[index]; } };
class CWorld { public: unsigned char _before_PMainMeshBank[0x3c]; CMeshDataBank* PMainMeshBank; };
class CMainGameComponent { public: unsigned char _before_PWorld[0x24]; CWorld* PWorld; };
static CMainGameComponent* g_pMainGameComponent;
class CAnimationEntry {
public:
    long BankIndex;
    void GetRotationInAnimation(float& rotation) const
    { rotation = -g_pMainGameComponent->PWorld->PMainMeshBank->PeekAnimation(BankIndex)->Rotation; }
};
int main()
{
    CMeshDataBank bank; CWorld world; world.PMainMeshBank = &bank; CMainGameComponent game; game.PWorld = &world; g_pMainGameComponent = &game;
    g_animations[0].Rotation = 1.0f; g_animations[1].Rotation = -2.5f; g_animations[2].Rotation = 0.0f;
    CAnimationEntry e; float r = 99.0f;
    e.BankIndex = 1; e.GetRotationInAnimation(r); bool ok = r == 2.5f;
    e.BankIndex = 0; e.GetRotationInAnimation(r); if (r != -1.0f) ok = false;
    e.BankIndex = 2; e.GetRotationInAnimation(r); if (r != 0.0f) ok = false;
    if (!ok) { std::printf("005dbfd0_TEST FAIL\n"); return 1; }
    std::printf("005dbfd0_TEST PASS\n");
    return 0;
}
