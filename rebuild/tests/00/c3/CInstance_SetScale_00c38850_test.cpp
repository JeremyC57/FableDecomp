// Test: CInstance::SetScale -- stores the scale and forwards it to the frame streamer when there is one.
#include <cstdio>
struct CStreamer { float s; void SetScale(float x) { s = x; } };
struct CInstance { CStreamer* m_pFrameStreamer; float m_scale;
    void SetScale(float scale) { m_scale = scale; if (m_pFrameStreamer) m_pFrameStreamer->SetScale(scale); } };
int main()
{
    CStreamer st; st.s = 0.0f; CInstance i; i.m_pFrameStreamer = 0; i.SetScale(2.0f);
    bool ok = i.m_scale == 2.0f && st.s == 0.0f;
    i.m_pFrameStreamer = &st; i.SetScale(0.25f); ok = ok && i.m_scale == 0.25f && st.s == 0.25f;
    std::printf(ok ? "00c38850_TEST PASS\n" : "00c38850_TEST FAIL\n");
    return ok ? 0 : 1;
}