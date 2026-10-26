#include <cstdio>
// behaviour test: CEnvironmentThemeSetDay::GetBlendedTheme picks the theme pair around time*24 (wrapping) and blends
struct CBlendedEnvironmentTheme;
static const CBlendedEnvironmentTheme* g_from = 0; static const CBlendedEnvironmentTheme* g_to = 0;
static float g_t = -1.0f; static CBlendedEnvironmentTheme* g_out = 0;
struct CBlendedEnvironmentTheme {
    float TimeOfDay;
    float Pad[124];
    void Blend(CBlendedEnvironmentTheme& out, const CBlendedEnvironmentTheme& other, float t) const
    { g_from = this; g_to = &other; g_t = t; g_out = &out; }
};
template <class T> class CArray { public:
    T* First; T* Last; T* Capacity;
    long Size() const { return (long)(Last - First); }
    const T& operator[](long i) const { return First[i]; }
};
struct CEnvironmentThemeSetDay {
    CArray<CBlendedEnvironmentTheme> Themes;
    void GetBlendedTheme(CBlendedEnvironmentTheme& out, float time) const;
};
void CEnvironmentThemeSetDay::GetBlendedTheme(CBlendedEnvironmentTheme& out, float time) const
{
    unsigned long count = Themes.Size();
    long i;
    for (i = 0; i < (long)count; ++i)
        if (Themes[i].TimeOfDay > time * 24.0f) break;
    unsigned long next = i % Themes.Size();
    unsigned long prev = (Themes.Size() + next - 1) % Themes.Size();
    float prevTime = Themes[prev].TimeOfDay * (1.0f / 24.0f);
    float nextTime = Themes[next].TimeOfDay * (1.0f / 24.0f);
    if (prevTime > nextTime) nextTime += 1.0f;
    Themes[prev].Blend(out, Themes[next], (time - prevTime) / (nextTime - prevTime));
}
static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
int main()
{
    CBlendedEnvironmentTheme t[3]; t[0].TimeOfDay = 6.0f; t[1].TimeOfDay = 12.0f; t[2].TimeOfDay = 18.0f;
    CEnvironmentThemeSetDay set; set.Themes.First = t; set.Themes.Last = t + 3; set.Themes.Capacity = t + 3;
    CBlendedEnvironmentTheme out;
    set.GetBlendedTheme(out, 9.0f / 24.0f);                 // between 6 and 12
    bool ok = g_from == &t[0] && g_to == &t[1] && near(g_t, 0.5f) && g_out == &out;
    set.GetBlendedTheme(out, 21.0f / 24.0f);                // after 18: wraps to 6 next day
    ok = ok && g_from == &t[2] && g_to == &t[0] && near(g_t, 0.25f);
    set.GetBlendedTheme(out, 3.0f / 24.0f);                 // before 6: blend 18 -> 6
    ok = ok && g_from == &t[2] && g_to == &t[0] && near(g_t, (3.0f / 24.0f - 0.75f) / 0.5f);
    if (!ok) { std::printf("00888260_TEST FAIL\n"); return 1; }
    std::printf("00888260_TEST PASS\n");
    return 0;
}