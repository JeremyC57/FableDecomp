// Behaviour test for CAnimateThing::GetInterpolatedMeshAnim @ 0x0070e650: UpdateCachedAnims first, then out->SetInterpolate(start, end, t).
#include <cstdio>
static int g_updated = 0;
struct C3DAnimationBlendState {
    long m_anim; float m_time;
    void SetInterpolate(const C3DAnimationBlendState& start, const C3DAnimationBlendState& end, float t)
    { if (g_updated != 1) { m_anim = -1; return; } m_anim = start.m_anim * 100 + end.m_anim; m_time = start.m_time + (end.m_time - start.m_time) * t; }
};
struct CAnimateThing {
    unsigned char _pad[0x68]; C3DAnimationBlendState StartAnimState; C3DAnimationBlendState EndAnimState;
    void UpdateCachedAnims() { ++g_updated; }
    void GetInterpolatedMeshAnim(C3DAnimationBlendState* pOut, float t) { UpdateCachedAnims(); pOut->SetInterpolate(StartAnimState, EndAnimState, t); }
};
int main()
{
    CAnimateThing a; a.StartAnimState.m_anim = 3; a.StartAnimState.m_time = 1.0f; a.EndAnimState.m_anim = 4; a.EndAnimState.m_time = 3.0f;
    C3DAnimationBlendState out; out.m_anim = 0; out.m_time = 0;
    a.GetInterpolatedMeshAnim(&out, 0.5f);
    bool ok = g_updated == 1 && out.m_anim == 304 && out.m_time == 2.0f;
    if (!ok) { std::printf("0070e650_TEST FAIL\n"); return 1; }
    std::printf("0070e650_TEST PASS\n");
    return 0;
}