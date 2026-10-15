// Behaviour test for CEngineLandscapeRenderer::CalcForegroundFadeFactors @ 0x00B671E0.
#include <cstdio>
#include <cmath>
struct C4DVector { float x, y, z, w; void Set(float a, float b, float c, float d) { x = a; y = b; z = c; w = d; } };
struct CEngineLandscapeRenderer {
    float m_foregroundFadeStart, m_foregroundFadeEnd;
    C4DVector m_fadeInFactors, m_fadeOutFactors;
    void CalcForegroundFadeFactors()
    {
        m_fadeOutFactors.Set(1.0f / (m_foregroundFadeEnd - m_foregroundFadeStart), 0.0f, 0.0f,
                             m_foregroundFadeEnd / (m_foregroundFadeStart - m_foregroundFadeEnd) + 1.0f);
        m_fadeInFactors.Set(1.0f / (m_foregroundFadeStart - m_foregroundFadeEnd), 0.0f, 0.0f,
                            m_foregroundFadeStart / (m_foregroundFadeEnd - m_foregroundFadeStart) + 1.0f);
    }
};
static int Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }
int main()
{
    CEngineLandscapeRenderer r; r.m_foregroundFadeStart = 10.0f; r.m_foregroundFadeEnd = 20.0f;
    r.m_fadeInFactors.y = 5; r.m_fadeOutFactors.z = 7;
    r.CalcForegroundFadeFactors();
    const C4DVector& o = r.m_fadeOutFactors; const C4DVector& i = r.m_fadeInFactors;
    // fade-out ramp: z*x + w is 0 at start (10) and 1 at end (20)
    int ok = Near(o.x * 10 + o.w, 0.0f) && Near(o.x * 20 + o.w, 1.0f) && o.y == 0 && o.z == 0;
    // fade-in ramp: 1 at start, 0 at end
    ok = ok && Near(i.x * 10 + i.w, 1.0f) && Near(i.x * 20 + i.w, 0.0f) && i.y == 0 && i.z == 0;
    ok = ok && Near(o.x, 0.1f) && Near(o.w, -1.0f) && Near(i.x, -0.1f) && Near(i.w, 2.0f);
    if (!ok) { std::printf("00b671e0_TEST FAIL\n"); return 1; }
    std::printf("00b671e0_TEST PASS\n");
    return 0;
}
