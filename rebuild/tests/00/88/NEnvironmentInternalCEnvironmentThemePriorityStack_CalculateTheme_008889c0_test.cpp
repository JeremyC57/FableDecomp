// Behaviour test for 0x008889c0 CEnvironmentThemePriorityStack::CalculateTheme
#include <cstdio>
struct CBlendedEnvironmentTheme { float m_value; int m_blends; };
struct CPallette { float m_scale; };
struct CEntry {
    float BaseWeight; float m_level;
    void GetLevelTheme(CBlendedEnvironmentTheme* theme, float time, CPallette* p) { theme->m_value = m_level * p->m_scale + time; theme->m_blends = 0; }
    void BlendInLevel(CBlendedEnvironmentTheme* out, const CBlendedEnvironmentTheme* in, float time, CPallette* p)
    { out->m_value = in->m_value + m_level; out->m_blends = in->m_blends + 1; }
};
template <class T> class CArray { public: T* m_pBegin; T* m_pEnd; T* m_pCapacity;
    unsigned long Size() const { return m_pEnd - m_pBegin; } T& operator[](unsigned long i) { return m_pBegin[i]; } };
struct CStack {
    CArray<CEntry> Ents;
    void CalculateTheme(CBlendedEnvironmentTheme* theme, float time, CPallette* p)
    {
        Ents[0].GetLevelTheme(theme, time, p);
        for (unsigned long i = 1; i < Ents.Size(); ++i) {
            CEntry& entry = Ents[i];
            if (1.0f - entry.BaseWeight > 0.0009765625f) entry.BlendInLevel(theme, theme, time, p);
        }
    }
};
int main()
{
    CEntry e[4] = { { 0.0f, 1.0f }, { 0.5f, 10.0f }, { 1.0f, 100.0f }, { 0.0f, 1000.0f } };   // entry 2 is fully weighted: skipped
    CStack s; s.Ents.m_pBegin = e; s.Ents.m_pEnd = s.Ents.m_pCapacity = e + 4; CPallette p = { 2.0f }; CBlendedEnvironmentTheme t;
    s.CalculateTheme(&t, 0.5f, &p);
    bool ok = t.m_value == 1012.5f && t.m_blends == 2;
    if (!ok) { std::printf("008889c0_TEST FAIL\n"); return 1; }
    std::printf("008889c0_TEST PASS\n");
    return 0;
}