#include <cstdio>
// behaviour test: OnInitialActivate -- always starts the camera earthquake when a camera manager exists; a live hero gets a
// controller rumble of level scale*Strength (scale 1 for a global rumble, else the distance falloff), smoothness 0.5,
// attack 0.2*Length, decay 0.8*Length, and the life timer becomes FPS*Length; no hero / a dead hero sets the timer to -1
struct Init { float MaxLevel, Smoothness, AttackTime, DecayTime; bool Cumulative; };
struct Dummy {
    float Strength, Length; bool Global; long LifeTimer;
    bool HasCamera, HasHero, HeroDead; float Falloff; long Fps;
    int Quakes; int Rumbles; Init Last;
    void OnInitialActivate()
    {
        if (HasCamera) ++Quakes;
        float decayTime = Length * 0.8f;
        float attackTime = Length * 0.2f;
        if (HasHero && !HeroDead) {
            float scale = 1.0f;
            if (!Global) scale = Falloff;
            Init init = { scale * Strength, 0.5f, attackTime, decayTime, false };
            Last = init; ++Rumbles;
            LifeTimer = (long)(Fps * Length);
            return;
        }
        LifeTimer = -1;
    }
};
static bool near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }
int main()
{
    Dummy d = { 2.0f, 5.0f, false, 0, true, true, false, 0.25f, 30, 0, 0 };
    d.OnInitialActivate();
    bool ok = d.Quakes == 1 && d.Rumbles == 1 && near(d.Last.MaxLevel, 0.5f) && near(d.Last.Smoothness, 0.5f)
              && near(d.Last.AttackTime, 1.0f) && near(d.Last.DecayTime, 4.0f) && !d.Last.Cumulative && d.LifeTimer == 150;
    d.Global = true; d.OnInitialActivate();
    ok = ok && near(d.Last.MaxLevel, 2.0f) && d.Rumbles == 2;
    d.HeroDead = true; d.HasCamera = false; d.OnInitialActivate();
    ok = ok && d.LifeTimer == -1 && d.Rumbles == 2 && d.Quakes == 2;
    d.HeroDead = false; d.HasHero = false; d.LifeTimer = 7; d.OnInitialActivate();
    ok = ok && d.LifeTimer == -1;
    if (!ok) { std::printf("007f1930_TEST FAIL\n"); return 1; }
    std::printf("007f1930_TEST PASS\n");
    return 0;
}