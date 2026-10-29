// Behaviour test for CWorld::UpdateSaveGameLoad @ 0x004A2F10: the four-step queued save-game load (fade out + pause, wait
// for the fade, load the save and its hero state, then region name / progress / display / sound restore + unpause).
#include <stdio.h>
#include <string.h>
class CWideString { public:
    char Text[64];
    CWideString() { Text[0] = 0; }
};
CWideString operator+(const CWideString& left, const CWideString& right)
{
    CWideString r; strcpy(r.Text, left.Text); strcat(r.Text, right.Text); return r;
}
class CAFile { public:
    static void SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name,
                          CWideString* extension)
    {   // "dir\name.ext" -> name, ".ext"
        const char* slash = strrchr(path.Text, '\\');
        const char* base = slash ? slash + 1 : path.Text;
        const char* dot = strrchr(base, '.');
        if (drive || directory) return;
        if (name) { size_t n = dot ? (size_t)(dot - base) : strlen(base); memcpy(name->Text, base, n); name->Text[n] = 0; }
        if (extension) strcpy(extension->Text, dot ? dot : "");
    }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
static int g_fadeStarts = 0, g_fadeEnds = 0, g_scenes = 0, g_progressStops = 0, g_areaNames = 0;
static float g_sceneTime = 0, g_fadeHold = 0;
static unsigned int g_groups[3]; static float g_volumes[3]; static int g_volumeCalls = 0;
class CIEngine { public:
    virtual void StartNewScene(unsigned int scene, float time) { g_scenes++; g_sceneTime = time; (void)scene; }
};
class CDisplayEngine { public:
    CIEngine* Engine3D;
    bool FadedOut;
    void StartScreenFadeOut(float, float, CRGBColour colour, float holdTime)
    { g_fadeStarts++; g_fadeHold = holdTime; if (colour.A != 255 || colour.R != 0) g_fadeStarts += 100; }
    void EndScreenFadeOut() { g_fadeEnds++; }
    bool IsScreenFullyFadedOut() const { return FadedOut; }
};
class CMainGameComponent { public:
    CDisplayEngine* PDisplayEngine;
    CDisplayEngine& GetDisplayEngine() { return *PDisplayEngine; }
};
class CCharString { public: const char* Text; };
class CRegion { public:
    CCharString DisplayName;
    const CCharString& GetDisplayName() const { return DisplayName; }
};
class CWorldMap { public:
    CRegion Regions[3]; int Loaded;
    int GetLoadedRegion() const { return Loaded; }
    const CRegion& GetRegion(int region) const { return Regions[region]; }
};
static const char* g_areaName = 0;
class CTCRegionDisplay { public:
    void SetCurrentAreaName(const CCharString& name, float, bool, bool, bool) { g_areaNames++; g_areaName = name.Text; }
};
class CThingPlayerCreature { public:
    CTCRegionDisplay* Display;
    bool GetTC(CTCRegionDisplay** out) { *out = Display; return true; }
};
class CPlayer { public:
    CThingPlayerCreature* Creature;
    CThingPlayerCreature* GetPControlledCreature() const { return Creature; }
};
class CPlayerManager { public:
    CPlayer Main;
    CPlayer& GetMainPlayer() { return Main; }
};
namespace NProgressDisplay { void StopProgress(bool, bool) { g_progressStops++; } }
class CUserProfileManager { public:
    bool UseNewProfileValues; float SoundVolume, DialogueVolume, SoundVolumeNewProfile, DialogueVolumeNewProfile;
    static CUserProfileManager& Get() { static CUserProfileManager m; return m; }
    float GetSoundVolume() const { return UseNewProfileValues ? SoundVolumeNewProfile : SoundVolume; }
    float GetDialogueVolume() const { return UseNewProfileValues ? DialogueVolumeNewProfile : DialogueVolume; }
};
class CSoundManagerLH { public:
    virtual void SetGroupVolume(unsigned int groupMask, float volume, float)
    { if (g_volumeCalls < 3) { g_groups[g_volumeCalls] = groupMask; g_volumes[g_volumeCalls] = volume; } g_volumeCalls++; }
};
CSoundManagerLH* g_pSoundSystem = 0;
bool g_ProgressDebugDisplay = true;
bool g_ProgressDebugDisplaySaved = false;
static char g_heroChecked[64], g_heroLoaded[64]; static int g_gameLoads = 0; static bool g_gameLoadResult = true;
class CWorld { public:
    CMainGameComponent* Component; CPlayerManager* PlayerManager; bool Paused; int SaveGameLoadStatus;
    CWideString SaveGamePathName; bool SerialisingAboutToLoadHeroState; bool DisplayMiniMapOnRegionLoad;
    CWorldMap Map; bool SerialisingSeenDuringLoad;
    const CWorldMap& PeekWorldMap() const { return Map; }
    bool IsHeroStateFileValid(const CWideString& name) const { strcpy(g_heroChecked, name.Text); return true; }
    bool LoadGameStateInternal(const CWideString&, bool) { g_gameLoads++; SerialisingSeenDuringLoad = SerialisingAboutToLoadHeroState; return g_gameLoadResult; }
    bool LoadHeroStateInternal(const CWideString& name) { strcpy(g_heroLoaded, name.Text); return true; }
    void SetAsPaused(bool paused) { if (Paused != paused) Paused = paused; }
    void UpdateSaveGameLoad()
    {
        switch (SaveGameLoadStatus) {
        case 1:
            DisplayMiniMapOnRegionLoad = false;
            Component->GetDisplayEngine().StartScreenFadeOut(1.0f, 1.0f, CRGBColour(0, 0, 0, 255), -1.0f);
            SetAsPaused(true);
            SaveGameLoadStatus = 2;
            break;
        case 2:
            if (Component->GetDisplayEngine().IsScreenFullyFadedOut()) {
                g_ProgressDebugDisplaySaved = g_ProgressDebugDisplay;
                g_ProgressDebugDisplay = false;
                SaveGameLoadStatus = 3;
            }
            break;
        case 3: {
            CWideString name;
            CWideString extension;
            CAFile::SplitPath(SaveGamePathName, 0, 0, &name, &extension);
            SerialisingAboutToLoadHeroState = IsHeroStateFileValid(name + extension);
            bool loaded = LoadGameStateInternal(SaveGamePathName, true);
            SerialisingAboutToLoadHeroState = false;
            if (loaded)
                LoadHeroStateInternal(name + extension);
            Component->GetDisplayEngine().Engine3D->StartNewScene(0, 20.0f);
            SaveGameLoadStatus = 4;
            break;
        }
        case 4: {
            const CRegion& region = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion());
            CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
            CTCRegionDisplay* regionDisplay;
            hero->GetTC(&regionDisplay);
            regionDisplay->SetCurrentAreaName(region.GetDisplayName(), -1.0f, false, false, true);
            NProgressDisplay::StopProgress(true, false);
            g_ProgressDebugDisplay = g_ProgressDebugDisplaySaved;
            Component->GetDisplayEngine().EndScreenFadeOut();
            DisplayMiniMapOnRegionLoad = true;
            if (g_pSoundSystem) {
                g_pSoundSystem->SetGroupVolume(0x10, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
                g_pSoundSystem->SetGroupVolume(0x40, CUserProfileManager::Get().GetDialogueVolume(), 0.0f);
                g_pSoundSystem->SetGroupVolume(0x80, CUserProfileManager::Get().GetSoundVolume(), 0.0f);
            }
            SetAsPaused(false);
            SaveGameLoadStatus = 0;
            break;
        }
        }
    }
};
static int fail(int line) { printf("004a2f10_TEST FAIL\n"); printf("line %d\n", line); return 1; }
int main()
{
    CIEngine engine; CDisplayEngine display; display.Engine3D = &engine; display.FadedOut = false;
    CMainGameComponent component; component.PDisplayEngine = &display;
    CTCRegionDisplay regionDisplay; CThingPlayerCreature hero; hero.Display = &regionDisplay;
    CPlayerManager players; players.Main.Creature = &hero;
    CSoundManagerLH sound; g_pSoundSystem = &sound;
    CUserProfileManager& profile = CUserProfileManager::Get();
    profile.UseNewProfileValues = true; profile.SoundVolume = 0.1f; profile.DialogueVolume = 0.2f;
    profile.SoundVolumeNewProfile = 0.7f; profile.DialogueVolumeNewProfile = 0.6f;
    CWorld w; w.Component = &component; w.PlayerManager = &players; w.Paused = false; w.SaveGameLoadStatus = 0;
    strcpy(w.SaveGamePathName.Text, "saves\\Hero1.sav"); w.SerialisingAboutToLoadHeroState = false;
    w.DisplayMiniMapOnRegionLoad = true; w.SerialisingSeenDuringLoad = false;
    w.Map.Loaded = 2; w.Map.Regions[2].DisplayName.Text = "Bowerstone";
    w.UpdateSaveGameLoad();                                   // idle: nothing
    if (g_fadeStarts != 0 || w.SaveGameLoadStatus != 0) return fail(__LINE__);
    w.SaveGameLoadStatus = 1; w.UpdateSaveGameLoad();
    if (g_fadeStarts != 1 || g_fadeHold != -1.0f || !w.Paused || w.DisplayMiniMapOnRegionLoad || w.SaveGameLoadStatus != 2)
        return fail(__LINE__);
    w.UpdateSaveGameLoad();                                   // fade not finished: wait
    if (w.SaveGameLoadStatus != 2 || !g_ProgressDebugDisplay) return fail(__LINE__);
    display.FadedOut = true; w.UpdateSaveGameLoad();
    if (w.SaveGameLoadStatus != 3 || g_ProgressDebugDisplay || !g_ProgressDebugDisplaySaved) return fail(__LINE__);
    w.UpdateSaveGameLoad();
    if (w.SaveGameLoadStatus != 4 || g_gameLoads != 1 || !w.SerialisingSeenDuringLoad || w.SerialisingAboutToLoadHeroState)
        return fail(__LINE__);
    if (strcmp(g_heroChecked, "Hero1.sav") != 0 || strcmp(g_heroLoaded, "Hero1.sav") != 0) return fail(__LINE__);
    if (g_scenes != 1 || g_sceneTime != 20.0f) return fail(__LINE__);
    w.UpdateSaveGameLoad();
    if (w.SaveGameLoadStatus != 0 || w.Paused || !w.DisplayMiniMapOnRegionLoad || g_fadeEnds != 1 || g_progressStops != 1)
        return fail(__LINE__);
    if (g_areaNames != 1 || strcmp(g_areaName, "Bowerstone") != 0 || !g_ProgressDebugDisplay) return fail(__LINE__);
    if (g_volumeCalls != 3 || g_groups[0] != 0x10 || g_groups[1] != 0x40 || g_groups[2] != 0x80) return fail(__LINE__);
    if (g_volumes[0] != 0.7f || g_volumes[1] != 0.6f || g_volumes[2] != 0.7f) return fail(__LINE__);
    // a failed game-state load skips the hero state
    g_heroLoaded[0] = 0; g_gameLoadResult = false; g_pSoundSystem = 0;
    w.SaveGameLoadStatus = 3; w.UpdateSaveGameLoad();
    if (g_heroLoaded[0] != 0 || w.SaveGameLoadStatus != 4 || g_scenes != 2) return fail(__LINE__);
    w.UpdateSaveGameLoad();
    if (g_volumeCalls != 3 || w.SaveGameLoadStatus != 0) return fail(__LINE__);
    printf("004a2f10_TEST PASS\n");
    return 0;
}