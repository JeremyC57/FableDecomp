#include <cstdio>
// behaviour test: CTCVillage::FrameUpdate runs the def's task updaters, every 30 frames flips mismatched lights and recounts working villagers, and announces a curfew / cook-time change
static int g_torch = 0, g_sweep = 0, g_horn = 0, g_played = 0; static const char* g_text = 0;
struct CLight { bool OnDuringDay; bool Active; };
struct CVillage {
    long Tasks[4]; int NumTasks; CLight Lights[3]; int NumLights; long States[4]; int NumPeople;
    long NumWorking; bool WasCurfew, WasCook, CurfewNow, CookNow, IsDay;
    void FrameUpdate(long frame)
    {
        bool torchesUpdated = false;
        for (int i = 0; i < NumTasks; ++i) {
            switch (Tasks[i]) {
            case 0: ++g_torch; torchesUpdated = true; break;
            case 3: ++g_sweep; break;
            case 4: ++g_horn; break;
            }
        }
        if (frame % 30 == 0) {
            if (!torchesUpdated)
                for (int i = 0; i < NumLights; ++i)
                    if (Lights[i].OnDuringDay == IsDay) Lights[i].Active = !IsDay;
            NumWorking = 0;
            for (int i = 0; i < NumPeople; ++i) if (States[i] == 3) ++NumWorking;
        }
        const char* text = "";
        bool was = WasCurfew;
        if (was != CurfewNow) text = was ? "CURFEW_START" : "CURFEW_END";
        was = WasCook;
        if (was != CookNow) text = was ? "COOK_START" : "COOK_END";
        if (text[0] != 0) { WasCurfew = CurfewNow; WasCook = CookNow; g_text = text; ++g_played; }
    }
};
int main()
{
    CVillage v; v.Tasks[0] = 3; v.Tasks[1] = 4; v.Tasks[2] = 9; v.NumTasks = 3;
    v.Lights[0].OnDuringDay = true; v.Lights[0].Active = true; v.Lights[1].OnDuringDay = false; v.Lights[1].Active = true; v.NumLights = 2;
    v.States[0] = 3; v.States[1] = 1; v.States[2] = 3; v.NumPeople = 3; v.NumWorking = 7;
    v.WasCurfew = false; v.WasCook = false; v.CurfewNow = true; v.CookNow = false; v.IsDay = true;
    v.FrameUpdate(60);
    bool ok = g_sweep == 1 && g_horn == 1 && g_torch == 0 && !v.Lights[0].Active && v.Lights[1].Active && v.NumWorking == 2
        && g_played == 1 && g_text[7] == 'E' && v.WasCurfew;
    v.FrameUpdate(61);
    ok = ok && g_played == 1 && g_sweep == 2;
    if (!ok) { std::printf("0065c781_TEST FAIL\n"); return 1; }
    std::printf("0065c781_TEST PASS\n");
    return 0;
}