// Behaviour test for CHeightSource::Evaluate @ 0x00C2A390 (the provider's value, 0 when disabled or without a provider).
#include <cstdio>

struct SPosition2D { float x; float y; };

struct CHeightProvider {
    virtual ~CHeightProvider() { }
    virtual float Evaluate(SPosition2D position) { return position.x * 2.0f + position.y; }
};

struct CHeightSource {
    bool             m_bDisabled;
    CHeightProvider* m_pProvider;
    float Evaluate(const SPosition2D& position)
    {
        if (m_pProvider && !m_bDisabled)
            return m_pProvider->Evaluate(position);
        return 0.0f;
    }
};

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    CHeightProvider provider;
    SPosition2D p; p.x = 1.5f; p.y = 0.25f;
    CHeightSource s;
    s.m_bDisabled = false; s.m_pProvider = &provider;
    bool ok = Near(s.Evaluate(p), 3.25f);
    s.m_bDisabled = true;
    ok = ok && Near(s.Evaluate(p), 0.0f);
    s.m_bDisabled = false; s.m_pProvider = 0;
    ok = ok && Near(s.Evaluate(p), 0.0f);
    if (!ok) { std::printf("00c2a390_TEST FAIL\n"); return 1; }
    std::printf("00c2a390_TEST PASS\n");
    return 0;
}