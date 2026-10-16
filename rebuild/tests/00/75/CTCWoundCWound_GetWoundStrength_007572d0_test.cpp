#include <cstdio>
struct CEntry { int FadeToScarDuration; int TotalDuration; float PermanentScarStrength; };
struct CWound { CEntry Desc; unsigned int Duration; float GetWoundStrength() const; };
float CWound::GetWoundStrength() const
{
    if (Duration < (unsigned int)Desc.TotalDuration)
    {
        if (Desc.FadeToScarDuration > 0)
        {
            if (Duration < (unsigned int)Desc.FadeToScarDuration)
                return 1.0f - (float)Duration / Desc.FadeToScarDuration;
        }
        else
            return 1.0f - (float)Duration / Desc.TotalDuration;
    }
    return 0.0f;
}
static bool Near(float a, float b) { return a - b < 1e-5f && b - a < 1e-5f; }
int main()
{
    CWound w; w.Desc.FadeToScarDuration = 100; w.Desc.TotalDuration = 400; w.Desc.PermanentScarStrength = 0.25f;
    w.Duration = 25;  if (!Near(w.GetWoundStrength(), 0.75f)) { std::printf("007572d0_TEST FAIL\n"); return 1; }
    w.Duration = 150; if (w.GetWoundStrength() != 0.0f) { std::printf("007572d0_TEST FAIL\n"); return 1; }
    w.Duration = 400; if (w.GetWoundStrength() != 0.0f) { std::printf("007572d0_TEST FAIL\n"); return 1; }
    w.Desc.FadeToScarDuration = 0; w.Duration = 100;
    if (!Near(w.GetWoundStrength(), 0.75f)) { std::printf("007572d0_TEST FAIL\n"); return 1; }
    std::printf("007572d0_TEST PASS\n");
    return 0;
}