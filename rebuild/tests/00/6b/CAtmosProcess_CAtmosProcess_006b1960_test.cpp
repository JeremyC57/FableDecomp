// behaviour test for CAtmosProcess::CAtmosProcess (0x006B1960): member defaults, sound-system hand-over with the def damping, loaders, def release
#include <cstdio>
#include <cstring>
static int g_destroyed = 0, g_themes = 0, g_main = 0, g_lookups = 0;
static char g_order[8]; static int g_n = 0;
struct CCharString { const char* m_text; CCharString(const char* t, long) : m_text(t) {} ~CCharString() {} };
struct CDefPointeeBase { long RefCount; virtual void Destroy() { ++g_destroyed; } virtual ~CDefPointeeBase() {} };
template <class T> struct CDefPointer { T* Object; CDefPointer() : Object(0) {} };
struct CSoundDef : CDefPointeeBase { float AtmosListenerDampingMin, AtmosListenerDampingRange; };
static CSoundDef g_def;
struct CDefinitionManager {
    static CDefinitionManager& Get() { static CDefinitionManager m; return m; }
    template <class T> bool GetDef(const CCharString& name, CDefPointer<T>& out)
    { ++g_lookups; if (std::strcmp(name.m_text, "MAIN_SOUND_SETUP") != 0) return false; ++g_def.RefCount; out.Object = &g_def; return true; }
};
struct CMyAtmosMap { virtual long GetBlockValue() { return 0; } };
struct CSoundSystem { void* m_map; float m_min, m_range; unsigned long m_flags;
    void SetAtmosMap_slot36(CMyAtmosMap* map, float mn, float rg, unsigned long f) { m_map = map; m_min = mn; m_range = rg; m_flags = f; g_order[g_n++] = 'S'; } };
static CSoundSystem* g_pSoundSystem = 0;
struct CIntelligentPointer { void* m_pItem; CIntelligentPointer() : m_pItem(0) {} };
class CAtmosProcess { public:
    long DayNightState; CMyAtmosMap AtmosMap; bool RainCurrentlyPlaying; bool AtmosSoundsMuted; CIntelligentPointer PRainPlayer;
    long InsideBuilding; long ReverbType; float ReverbLevel;
    void LoadThemes_006b16b0() { ++g_themes; g_order[g_n++] = 'T'; }
    void LoadMainTheme_006b1600() { ++g_main; g_order[g_n++] = 'M'; }
    CAtmosProcess()
        : DayNightState(0), RainCurrentlyPlaying(false), AtmosSoundsMuted(false), InsideBuilding(-1), ReverbType(0), ReverbLevel(0)
    {
        if (g_pSoundSystem) {
            CDefPointer<const CSoundDef> soundDef;
            { CCharString name("MAIN_SOUND_SETUP", -1); CDefinitionManager::Get().GetDef(name, soundDef); }
            CSoundDef* def = const_cast<CSoundDef*>(soundDef.Object);
            g_pSoundSystem->SetAtmosMap_slot36(&AtmosMap, def->AtmosListenerDampingMin, def->AtmosListenerDampingRange, 0x8010);
            LoadThemes_006b16b0(); LoadMainTheme_006b1600();
            if (--def->RefCount == 0) def->Destroy();
        }
    } };
int main()
{
    int bad = 0;
    CAtmosProcess quiet;                                 // no sound system: members only
    if (quiet.DayNightState != 0 || quiet.RainCurrentlyPlaying || quiet.AtmosSoundsMuted || quiet.PRainPlayer.m_pItem != 0) bad = 1;
    if (quiet.InsideBuilding != -1 || quiet.ReverbType != 0 || quiet.ReverbLevel != 0.0f || g_lookups != 0) bad = 1;
    CSoundSystem sound; g_pSoundSystem = &sound;
    g_def.RefCount = 0; g_def.AtmosListenerDampingMin = 0.25f; g_def.AtmosListenerDampingRange = 3.5f;
    CAtmosProcess p;                                     // looked up (+1), released (-1 -> 0): destroyed
    if (g_lookups != 1 || sound.m_map != &p.AtmosMap || sound.m_min != 0.25f || sound.m_range != 3.5f || sound.m_flags != 0x8010) bad = 1;
    if (g_themes != 1 || g_main != 1 || g_order[0] != 'S' || g_order[1] != 'T' || g_order[2] != 'M' || g_destroyed != 1) bad = 1;
    g_def.RefCount = 1; CAtmosProcess q;                 // def held elsewhere: not destroyed
    if (g_destroyed != 1 || g_def.RefCount != 1) bad = 1;
    if (bad) { std::printf("006b1960_TEST FAIL\n"); return 1; }
    std::printf("006b1960_TEST PASS\n");
    return 0;
}