// Behaviour test for 0x006B4900: the CEnvironment ctor seeds lightning/sunlight values from the environment defs, sets the wind defaults, replaces both spell-effect pointers, builds the themes, then per channel pushes a 5-entry stack, resets entry 0 and blends the day theme (12h) into both tick states.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ","); }
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CTheme { int Id; };
struct CTickState {
    CTheme BlendedTheme[6];
    float Time, WindSpeed; C3DVector WindDirection; float ThemeWindSpeed; C3DVector ThemeWindDirection;
    bool WindOverridden; C3DVector OverrideWindDirection; int ThunderState;
};
struct CEntry { int Theme; void Reset(int t) { Theme = t; Log("reset"); } };
struct CStack {
    std::vector<CEntry> Entries;
    explicit CStack(int n) : Entries(n) { CEntry e = { -1 }; for (int i = 0; i < n; ++i) Entries[i] = e; }
};
struct CDay {
    int Calls;
    void GetBlendedTheme(CTheme* theme, float hour) { theme->Id = (int)hour + Calls++; }
};
struct CEnvironment;
struct CIface { CEnvironment* Env; CIface(CEnvironment* e) : Env(e) { Log("iface"); } };
struct CInterp { CIface* Iface; CEnvironment* Env; CInterp(CIface* i, CEnvironment* e) : Iface(i), Env(e) { Log("interp"); } };
struct CDefs { float Diffuse, FadeStart, FadeEnd, FadeIn, FadeOut, Flash, RainThreshold; int Theme; };
struct CDefMgr { CDefs* Environment; };
struct CEnvironment {
    CDefMgr* DefinitionManager;
    float FadeStart, FadeEnd; int RegionTheme; double LightningStartTime;
    float FadeIn, FadeOut, Flash, RainThreshold; int LightningTheme; float FlashChance; unsigned int Seed;
    bool ForcedColours, ForcedNonColourValues, ForcedEnvironmentTheme; float Diffuse; bool PassedFirstUpdate, RainEnabled;
    std::vector<CStack> Channels;
    CTickState GameTickState, PrevGameTickState;
    CDay Day; CDay* DayPtr;
    CIface* SpellEffectInterface; CInterp* SpellEffectInterpolator;
    void BuildThemes() { Log("build"); DayPtr = &Day; Day.Calls = 0; }
    CEnvironment(CDefMgr* defs) : DefinitionManager(defs), DayPtr(0), SpellEffectInterface(0), SpellEffectInterpolator(0)
    {
        LightningStartTime = -1.0;
        GameTickState.WindDirection = C3DVector(1.0f, 0.0f, 0.0f);
        GameTickState.ThemeWindDirection = C3DVector(1.0f, 0.0f, 0.0f);
        GameTickState.WindSpeed = 0.2f;
        GameTickState.ThemeWindSpeed = 0.2f;
        GameTickState.Time = 0.0f;
        GameTickState.WindOverridden = false;
        GameTickState.ThunderState = 0;
        RegionTheme = 0;
        Seed = 123;
        FlashChance = 0.0f;
        GameTickState.OverrideWindDirection = C3DVector(1.0f, 0.0f, 0.0f);
        CDefs* environment = DefinitionManager->Environment;
        Diffuse = environment->Diffuse;
        FadeStart = environment->FadeStart;
        FadeEnd = environment->FadeEnd;
        FadeIn = environment->FadeIn;
        FadeOut = environment->FadeOut;
        Flash = environment->Flash;
        RainThreshold = environment->RainThreshold;
        LightningTheme = environment->Theme;
        ForcedColours = false;
        ForcedNonColourValues = false;
        ForcedEnvironmentTheme = false;
        PassedFirstUpdate = false;
        RainEnabled = true;
        {
            CIface* created = new CIface(this);
            CIface* old = SpellEffectInterface;
            SpellEffectInterface = created;
            delete old;
        }
        {
            CInterp* created = new CInterp(SpellEffectInterface, this);
            CInterp* old = SpellEffectInterpolator;
            SpellEffectInterpolator = created;
            delete old;
        }
        BuildThemes();
        for (int i = 0; i < 6; ++i) {
            Channels.push_back(CStack(5));
            Channels[i].Entries[0].Reset(0);
            DayPtr->GetBlendedTheme(&GameTickState.BlendedTheme[i], 12.0f);
            DayPtr->GetBlendedTheme(&PrevGameTickState.BlendedTheme[i], 12.0f);
        }
    }
};
int main()
{
    CDefs defs = { 0.5f, 10.0f, 20.0f, 0.1f, 0.2f, 0.3f, 0.4f, 7 };
    CDefMgr mgr = { &defs };
    CEnvironment env(&mgr);
    int ok = 1;
    ok &= std::strcmp(g_log, "iface,interp,build,reset,reset,reset,reset,reset,reset,") == 0;
    ok &= env.LightningStartTime == -1.0 && env.Seed == 123 && env.RainEnabled && !env.ForcedColours;
    ok &= env.Diffuse == 0.5f && env.FadeStart == 10.0f && env.FadeEnd == 20.0f && env.LightningTheme == 7 && env.RainThreshold == 0.4f;
    ok &= env.GameTickState.WindSpeed == 0.2f && env.GameTickState.WindDirection.X == 1.0f && env.GameTickState.OverrideWindDirection.Y == 0.0f;
    ok &= env.Channels.size() == 6 && env.Channels[5].Entries.size() == 5 && env.Channels[3].Entries[0].Theme == 0 && env.Channels[3].Entries[1].Theme == -1;
    ok &= env.GameTickState.BlendedTheme[0].Id == 12 && env.PrevGameTickState.BlendedTheme[0].Id == 13 && env.PrevGameTickState.BlendedTheme[5].Id == 23;
    ok &= env.SpellEffectInterpolator->Iface == env.SpellEffectInterface && env.SpellEffectInterface->Env == &env;
    if (!ok)
        std::printf("log %s\n", g_log);
    std::printf(ok ? "006b4900_TEST PASS\n" : "006b4900_TEST FAIL\n");
    return ok ? 0 : 1;
}
