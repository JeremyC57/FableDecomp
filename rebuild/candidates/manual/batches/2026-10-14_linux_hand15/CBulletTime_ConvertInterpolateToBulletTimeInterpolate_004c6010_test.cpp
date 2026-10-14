#include <stdio.h>
static long g_frames;
long GFGetFrameCount() { return g_frames; }
const float g_bulletTimeRatioNumerator = 2.0f;
struct CBulletTime {
    float m_scale;
    float GetTimeScale() { return m_scale; }
    float ConvertInterpolateToBulletTimeInterpolate(float interpolate);
};
float CBulletTime::ConvertInterpolateToBulletTimeInterpolate(float interpolate)
{
    float ratio; long divisor; long frames;
    { float scale = GetTimeScale(); ratio = g_bulletTimeRatioNumerator / scale; frames = GFGetFrameCount(); divisor = (long)scale; }
    long rem = (frames - 1) % divisor;
    float carried = ratio * (float)rem;
    float stepped = interpolate * ratio;
    return carried + stepped;
}
int main()
{
    CBulletTime bt; bt.m_scale = 4.0f; g_frames = 10;      // ratio 0.5, rem = 9 % 4 = 1
    float r0 = bt.ConvertInterpolateToBulletTimeInterpolate(3.0f);   // 0.5 + 1.5
    g_frames = 13;                                            // rem = 12 % 4 = 0
    float r1 = bt.ConvertInterpolateToBulletTimeInterpolate(3.0f);   // 1.5
    bool ok = r0 == 2.0f && r1 == 1.5f;
    if (!ok) { printf("004c6010_TEST FAIL\n"); return 1; }
    printf("004c6010_TEST PASS\n"); return 0;
}
