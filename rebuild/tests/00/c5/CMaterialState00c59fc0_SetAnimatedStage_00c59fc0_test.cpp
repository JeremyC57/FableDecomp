#include <cstdio>
// Behaviour test for CMaterialState_00c59fc0::SetAnimatedStage (0x00C59FC0).
struct SStageDesc { long m_frames; };
static long ConvertStageFrames(long frames) { return frames * 2 + 1; }
static int g_applied = 0;
struct CMaterialState_00c59fc0 {
    unsigned char _pad[0x48];
    long m_stagePixels;
    void ApplyStageTransform(SStageDesc*) { ++g_applied; }
    void SetAnimatedStage(SStageDesc* desc);
};
void CMaterialState_00c59fc0::SetAnimatedStage(SStageDesc* desc)
{
    ApplyStageTransform(desc);
    if (desc)
        m_stagePixels = ConvertStageFrames(desc->m_frames);
}
int main()
{
    static CMaterialState_00c59fc0 m;
    m.m_stagePixels = -7;
    m.SetAnimatedStage(0);
    if (g_applied != 1 || m.m_stagePixels != -7) { std::printf("00c59fc0_TEST FAIL\n"); return 1; }
    SStageDesc d; d.m_frames = 10;
    m.SetAnimatedStage(&d);
    if (g_applied != 2 || m.m_stagePixels != 21) { std::printf("00c59fc0_TEST FAIL\n"); return 1; }
    std::printf("00c59fc0_TEST PASS\n");
    return 0;
}