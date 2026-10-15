// Behaviour test for CMusicManager::CMusicManager @ 0x006BE300
#include <cstdio>
#include <cstring>
static const char* g_lookedUp = 0;
static int g_registered = 0;
class CCharString { public: const char* m_text; CCharString(const char* text, long) : m_text(text) {} ~CCharString() {} };
struct CSoundDef { int id; };
static CSoundDef g_soundDef = { 42 };
template <class T> struct CDefPointer { T* Object; CDefPointer() : Object(0) {} };
struct CGameDefinitionManager {
    static CGameDefinitionManager& Get() { static CGameDefinitionManager m; return m; }
    bool GetDef(const CCharString& name, CDefPointer<const CSoundDef>& out) { g_lookedUp = name.m_text; out.Object = &g_soundDef; return true; }
};
template <class T> struct CRetailVector { T* m_start; T* m_finish; T* m_end; CRetailVector() : m_start(0), m_finish(0), m_end(0) {} };
struct CMusicState {
    long RegionSet, OverrideSet, UIOverrideSet, CinematicOverrideSet, MapCellSet; bool Paused, Danger;
    CMusicState() : RegionSet(0), OverrideSet(0), UIOverrideSet(0), CinematicOverrideSet(0), MapCellSet(0), Paused(false), Danger(false) {}
};
struct CRegisteredMusicMap { CRetailVector<long> m_entries; bool Dirty; CRegisteredMusicMap() : Dirty(false) {} };
struct CMusicManager {
    CMusicState CurrentState; CDefPointer<const CSoundDef> PSoundDef; CRegisteredMusicMap RegisteredMusic;
    long CurrentMusicID, PendingMusicID; void* PPendingFadeTimes; long PendingLoopCount; bool Paused;
    void RegisterMusic() { ++g_registered; Paused = true; }
    CMusicManager() : CurrentMusicID(0), PendingMusicID(0), PPendingFadeTimes(0)
    {
        { CCharString name("MAIN_SOUND_SETUP", -1); CGameDefinitionManager::Get().GetDef(name, PSoundDef); }
        RegisterMusic();
        Paused = false;
    }
};
int main()
{
    CMusicManager m;
    if (!g_lookedUp || std::strcmp(g_lookedUp, "MAIN_SOUND_SETUP") != 0 || m.PSoundDef.Object != &g_soundDef || g_registered != 1 ||
        m.Paused || m.CurrentState.RegionSet || m.CurrentState.Danger || m.RegisteredMusic.m_entries.m_start || m.RegisteredMusic.Dirty ||
        m.CurrentMusicID || m.PendingMusicID || m.PPendingFadeTimes) {
        std::printf("006be300_TEST FAIL\n"); return 1;
    }
    std::printf("006be300_TEST PASS\n");
    return 0;
}
