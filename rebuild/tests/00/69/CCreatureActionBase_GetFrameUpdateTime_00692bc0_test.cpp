#include <cstdio>
class CThingCreatureBase { public: float mult; float GetAnimationSpeedMultiplier() const { return mult; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { return m_pItem; } };
long g_frameRate = 25;
class CCreatureActionBase { public: virtual void v00() {} unsigned char _pad04[4]; CBaseIntelligentPointer PCreature;
    float GetFrameUpdateTime() const { float frameTime = 1.0f / (float)g_frameRate; return PCreature.GetPItem()->GetAnimationSpeedMultiplier() * frameTime; } };
int main()
{
    CThingCreatureBase c; c.mult = 2.0f; CCreatureActionBase a; a.PCreature.m_pItem = &c;
    float r = a.GetFrameUpdateTime();
    if (r < 0.0799f || r > 0.0801f) { std::printf("00692bc0_TEST FAIL\n"); return 1; }
    std::printf("00692bc0_TEST PASS\n");
    return 0;
}