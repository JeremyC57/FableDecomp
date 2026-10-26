// Behaviour test (0x0049e5e0): CWorld::PrepareAtmosBanksForLoadedRegion mutes atmos sounds when the loaded region has sound themes off
#include <cstdio>
#include <cmath>
class CRegion { public:
    unsigned char _pad00[0x56];
    bool          SoundThemesEnabled;
    unsigned char _pad57;
};
class CWorldMap { public:
    CRegion Regions[3];
    long    Loaded;
    long GetLoadedRegion() const { return Loaded; }
    const CRegion& GetRegion(long region) const { return Regions[region]; }
};
class CAtmosProcess { public:
    int   Calls;
    bool  Muted;
    float Fade;
    void SetAtmosSoundsAsMuted(bool muted, float fadeTime) { ++Calls; Muted = muted; Fade = fadeTime; }
};
class CWorld { public:
    CWorldMap      Map;
    CAtmosProcess* PAtmosProcessor;
    virtual const CWorldMap& PeekWorldMap() const { return Map; }
    virtual ~CWorld() { }
    void PrepareAtmosBanksForLoadedRegion()
    {
        if (PAtmosProcessor)
        {
            const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion());
            PAtmosProcessor->SetAtmosSoundsAsMuted(!region.SoundThemesEnabled, 1000.0f);
        }
    }
};
int main()
{
    CWorld w;
    w.Map.Regions[0].SoundThemesEnabled = true;
    w.Map.Regions[1].SoundThemesEnabled = false;
    w.Map.Regions[2].SoundThemesEnabled = true;
    w.PAtmosProcessor = 0;
    w.Map.Loaded = 1;
    w.PrepareAtmosBanksForLoadedRegion();                        // no processor: nothing to do
    CAtmosProcess atmos; atmos.Calls = 0; atmos.Muted = false; atmos.Fade = 0.0f;
    w.PAtmosProcessor = &atmos;
    w.PrepareAtmosBanksForLoadedRegion();
    if (atmos.Calls != 1 || !atmos.Muted || std::fabs(atmos.Fade - 1000.0f) > 1e-3f) { std::printf("0049e5e0_TEST FAIL\n"); return 1; }
    w.Map.Loaded = 2;
    w.PrepareAtmosBanksForLoadedRegion();
    if (atmos.Calls != 2 || atmos.Muted) { std::printf("0049e5e0_TEST FAIL\n"); return 1; }
    std::printf("0049e5e0_TEST PASS\n");
    return 0;
}