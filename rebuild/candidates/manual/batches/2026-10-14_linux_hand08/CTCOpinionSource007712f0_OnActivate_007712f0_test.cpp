#include <cstdio>
static long g_frame = 100; static long g_framesPerSecond = 15;
long GFGetCurrentFrame() { return g_frame; }
struct CTCOpinionSource_007712f0 { unsigned char _pad[0x80]; long m_sourceId; long m_activations; long m_expiryFrame; void OnActivate(long sourceId, float duration); };
void CTCOpinionSource_007712f0::OnActivate(long sourceId, float duration) { ++m_activations; m_sourceId = sourceId; long framesPerSecond = g_framesPerSecond; m_expiryFrame = (long)((float)GFGetCurrentFrame() + (float)framesPerSecond * duration); }
int main()
{
    CTCOpinionSource_007712f0 o; o.m_sourceId = 0; o.m_activations = 1; o.m_expiryFrame = 0;
    o.OnActivate(7, 2.0f);
    if (o.m_sourceId != 7 || o.m_activations != 2 || o.m_expiryFrame != 130) { std::printf("007712f0_TEST FAIL\n"); return 1; }
    std::printf("007712f0_TEST PASS\n"); return 0;
}
