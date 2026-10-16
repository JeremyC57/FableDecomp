#include <cstdio>
struct CEntry { int FadeToScarDuration; int TotalDuration; float PermanentScarStrength; };
struct CWound { CEntry Desc; unsigned int Duration; float GetScarStrength() const; };
float CWound::GetScarStrength() const
{
    if (Duration < (unsigned int)Desc.TotalDuration && Desc.FadeToScarDuration > 0)
    {
        if (Duration > (unsigned int)Desc.FadeToScarDuration)
        {
            float sinceFade = (float)(Duration - Desc.FadeToScarDuration);
            float scarTime = (float)(Desc.TotalDuration - Desc.FadeToScarDuration);
            float t = sinceFade / scarTime;
            return (1.0f - t) * (1.0f - Desc.PermanentScarStrength) + Desc.PermanentScarStrength;
        }
        return (float)Duration / Desc.FadeToScarDuration;
    }
    return Desc.PermanentScarStrength;
}
static bool Near(float a, float b) { return a - b < 1e-5f && b - a < 1e-5f; }
int main()
{
    CWound w; w.Desc.FadeToScarDuration = 100; w.Desc.TotalDuration = 300; w.Desc.PermanentScarStrength = 0.2f;
    w.Duration = 50;  if (!Near(w.GetScarStrength(), 0.5f)) { std::printf("00757340_TEST FAIL\n"); return 1; }
    w.Duration = 200; if (!Near(w.GetScarStrength(), 0.6f)) { std::printf("00757340_TEST FAIL\n"); return 1; }
    w.Duration = 300; if (!Near(w.GetScarStrength(), 0.2f)) { std::printf("00757340_TEST FAIL\n"); return 1; }
    w.Desc.FadeToScarDuration = 0; w.Duration = 10;
    if (!Near(w.GetScarStrength(), 0.2f)) { std::printf("00757340_TEST FAIL\n"); return 1; }
    std::printf("00757340_TEST PASS\n");
    return 0;
}