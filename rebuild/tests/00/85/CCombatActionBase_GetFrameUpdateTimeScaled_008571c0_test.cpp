#include <cstdio>

// Model of CCombatActionBase::GetFrameUpdateTime @ 0x008571C0
struct CSpeedVariation { unsigned char _pad[8]; float Multiplier; };
struct CSpeedVariationHandle { const CSpeedVariation* m_pVariation; CSpeedVariationHandle(const CSpeedVariation* p) : m_pVariation(p) { } };
static CSpeedVariation g_var; static int g_handleCalls = 0;
struct CAnimationEntry { CSpeedVariationHandle GetSpeedVariation() { ++g_handleCalls; return CSpeedVariationHandle(&g_var); } };
struct CCombatActionBase {
    virtual void v00() {} unsigned char _pad04[0x40 - 4]; float RunningTimeSeconds; unsigned char _pad44[0x74 - 0x44]; CAnimationEntry* PAnimEntry; unsigned char _pad78[0xd0 - 0x78]; float StrikeSpeedVariationStartTime; float StrikeSpeedVariationEndTime; unsigned char _padd8[0x114 - 0xd8];
    float GetFrameUpdateTime() const { return 0.5f; }
    float GetFrameUpdateTimeScaled() const {
        float frameTime = CCombatActionBase::GetFrameUpdateTime();
        if (PAnimEntry != 0) { CSpeedVariationHandle v = PAnimEntry->GetSpeedVariation(); float t = RunningTimeSeconds; if (t > StrikeSpeedVariationStartTime && t < StrikeSpeedVariationEndTime) return frameTime * v.m_pVariation->Multiplier; }
        return frameTime;
    }
};
int main()
{
    g_var.Multiplier = 3.0f; CAnimationEntry e; CCombatActionBase a; a.PAnimEntry = 0; a.RunningTimeSeconds = 2.0f; a.StrikeSpeedVariationStartTime = 1.0f; a.StrikeSpeedVariationEndTime = 3.0f;
    if (a.GetFrameUpdateTimeScaled() != 0.5f || g_handleCalls != 0) { std::printf("008571c0_TEST FAIL\n"); return 1; }
    a.PAnimEntry = &e;
    if (a.GetFrameUpdateTimeScaled() != 1.5f || g_handleCalls != 1) { std::printf("008571c0_TEST FAIL\n"); return 1; }
    a.RunningTimeSeconds = 1.0f; if (a.GetFrameUpdateTimeScaled() != 0.5f) { std::printf("008571c0_TEST FAIL\n"); return 1; }
    a.RunningTimeSeconds = 3.0f; if (a.GetFrameUpdateTimeScaled() != 0.5f) { std::printf("008571c0_TEST FAIL\n"); return 1; }
    std::printf("008571c0_TEST PASS\n"); return 0;
}