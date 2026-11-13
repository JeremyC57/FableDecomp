// Behaviour test for CEnvironment::Update 0x006B3FF0: a copy of the per-channel blend and the rain-splash keep-alive logic.
#include <cstdio>
#include <cstring>
#include <vector>
struct Theme { float Colours[19*4]; float Scalers[30]; std::vector<long> Tex[3]; std::vector<float> Blends[3]; };
struct Thing { bool dead; int killed; };
struct Env {
    bool ForcedColours, ForcedNonColourValues, RainEnabled, PassedFirstUpdate;
    Theme Tick[6]; float Time; Thing* splash; int created, attached, sunmoon;
    Thing pool;
    float GetRainStrength() const { return Tick[0].Scalers[15]; }
    void Update(float time, const Theme& src) {
        Time = time;
        for (int i = 0; i < 6; ++i) {
            if (!ForcedColours) std::memcpy(Tick[i].Colours, src.Colours, sizeof(src.Colours));
            if (!ForcedNonColourValues) {
                for (int s = 0; s < 30; ++s) Tick[i].Scalers[s] = src.Scalers[s];
                for (int t = 0; t < 3; ++t) { Tick[i].Tex[t] = src.Tex[t]; Tick[i].Blends[t] = src.Blends[t]; }
            }
        }
        const float thr = 0.0001f;
        float rain = GetRainStrength();
        if (!RainEnabled) rain = 0.0f;
        bool alive = splash && !splash->dead;
        if (rain > thr && !alive) {
            pool.dead = false; pool.killed = 0; splash = &pool; ++created;
            if (splash && !splash->dead) ++attached;
        } else if (rain <= thr && alive) {
            splash->killed++; splash->dead = true;
        }
        ++sunmoon;
        if (!PassedFirstUpdate) PassedFirstUpdate = true;
    }
};
int main()
{
    static Env e; std::memset(&e.ForcedColours, 0, 4); e.splash = 0; e.created = e.attached = e.sunmoon = 0; e.Time = 0;
    static Theme src; for (int s = 0; s < 30; ++s) src.Scalers[s] = 0.0f; src.Scalers[15] = 0.5f; src.Colours[0] = 2.0f;
    src.Tex[1].push_back(7); src.Blends[1].push_back(0.25f);
    e.RainEnabled = true;
    e.Update(3.0f, src);
    if (e.Time != 3.0f || e.Tick[5].Scalers[15] != 0.5f || e.Tick[3].Colours[0] != 2.0f || e.Tick[2].Tex[1].size() != 1) { std::printf("006b3ff0_TEST FAIL\n"); return 1; }
    if (e.created != 1 || e.attached != 1 || !e.PassedFirstUpdate) { std::printf("006b3ff0_TEST FAIL\n"); return 1; }
    e.Update(4.0f, src);                                  // still raining, splash alive: nothing new
    if (e.created != 1 || e.pool.killed != 0) { std::printf("006b3ff0_TEST FAIL\n"); return 1; }
    e.RainEnabled = false;                                // rain disabled -> strength 0 -> kill
    e.Update(5.0f, src);
    if (e.pool.killed != 1 || !e.pool.dead) { std::printf("006b3ff0_TEST FAIL\n"); return 1; }
    e.ForcedColours = true; src.Colours[0] = 9.0f; e.Update(6.0f, src);
    if (e.Tick[0].Colours[0] != 2.0f || e.created != 1 || e.sunmoon != 4) { std::printf("006b3ff0_TEST FAIL\n"); return 1; }
    std::printf("006b3ff0_TEST PASS\n");
    return 0;
}