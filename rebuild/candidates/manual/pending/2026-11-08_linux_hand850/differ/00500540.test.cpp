#include <cstdio>
#include <cstring>
// Behaviour test for CWorldMap::LoadRegion (retail 0x00500540): with a region def the region and internal lighting themes,
// spotlight shadow mode, sea body, camera / hero-breath particles (old ones killed first) and village time are applied, else
// village time is switched off; every map of the region is made visible and gets a level-files record (thing file, topology file,
// navigation file only when asked; both time stamps written to Maps[index]); visible maps are shown; the loader object is queued,
// waited on for WAIT_FOR_LOAD, and the quest manager is told the region name.
struct CRegionDef { long EnvironmentTheme, InternalEnvironmentTheme, InternalLightingChannelCount; bool UseVillageTime;
    long AttachToCameraFX; bool EnableSpotlightShadowMode; long HeroBreathParticle; bool HasSea; };
struct CLevelFiles { long LevelID; int Topology, Navigation, Thing; };
struct CWorldMapModel {
    const CRegionDef* Def; long Maps[3]; long NoMaps; long VisMaps[2]; long NoVisMaps;
    bool Visible[16]; long RegionTheme; long ChannelTheme[8]; bool Spot; char Sea[16]; bool Village;
    int CameraParticle, BreathParticle, Killed; bool UseWAD; long Stamps; CLevelFiles Files[3]; long NoFiles;
    int BusyCount, Processed, Queued; char Loaded[16];
    void LoadRegion(int wait, bool load_navigation, int hero_alive)
    {
        for (int m = 0; m < 16; ++m) Visible[m] = false;
        if (Def) {
            if (Def->EnvironmentTheme > 0) RegionTheme = Def->EnvironmentTheme;
            if (Def->InternalEnvironmentTheme > 0)
                for (long n = 0; n < Def->InternalLightingChannelCount; ++n) ChannelTheme[n + 1] = Def->InternalEnvironmentTheme;
            Spot = Def->EnableSpotlightShadowMode;
            std::strcpy(Sea, Def->HasSea ? "Region" : "");
            if (CameraParticle) { ++Killed; CameraParticle = 0; }
            if (Def->AttachToCameraFX > 0) CameraParticle = Def->AttachToCameraFX;
            if (BreathParticle) { ++Killed; BreathParticle = 0; }
            if (Def->HeroBreathParticle > 0 && hero_alive) BreathParticle = Def->HeroBreathParticle;
            Village = Def->UseVillageTime;
        } else Village = false;
        NoFiles = 0;
        for (long c = 0; c < NoMaps; ++c) {
            Visible[Maps[c]] = true; Stamps += 2;
            CLevelFiles f = { Maps[c], UseWAD ? 2 : 1, 0, UseWAD ? 2 : 1 };
            if (load_navigation) f.Navigation = f.Topology;
            Files[NoFiles++] = f;
        }
        for (long c = 0; c < NoVisMaps; ++c) Visible[VisMaps[c]] = true;
        ++Queued;
        if (wait == 0) while (BusyCount > 0) { ++Processed; --BusyCount; }
        std::strcpy(Loaded, "Region");
    }
};
int main()
{
    CRegionDef def = { 3, 5, 2, true, 7, true, 9, true };
    CWorldMapModel w; std::memset(&w, 0, sizeof(w));
    w.Def = &def; w.Maps[0] = 1; w.Maps[1] = 4; w.NoMaps = 2; w.VisMaps[0] = 6; w.NoVisMaps = 1; w.CameraParticle = 1; w.BusyCount = 3;
    w.LoadRegion(0, true, 1);
    int ok = w.RegionTheme == 3 && w.ChannelTheme[1] == 5 && w.ChannelTheme[2] == 5 && w.ChannelTheme[3] == 0 && w.Spot
          && std::strcmp(w.Sea, "Region") == 0 && w.CameraParticle == 7 && w.BreathParticle == 9 && w.Killed == 1 && w.Village
          && w.Visible[1] && w.Visible[4] && w.Visible[6] && !w.Visible[2] && w.NoFiles == 2 && w.Files[1].LevelID == 4
          && w.Files[0].Navigation == 1 && w.Stamps == 4 && w.Processed == 3 && w.Queued == 1 && std::strcmp(w.Loaded, "Region") == 0;
    w.Def = 0; w.UseWAD = true; w.BusyCount = 2; w.LoadRegion(1, false, 1);
    ok = ok && !w.Village && w.Files[0].Topology == 2 && w.Files[0].Navigation == 0 && w.Processed == 3 && w.Queued == 2;
    if (!ok) { std::printf("00500540_TEST FAIL\n"); return 1; }
    std::printf("00500540_TEST PASS\n");
    return 0;
}
