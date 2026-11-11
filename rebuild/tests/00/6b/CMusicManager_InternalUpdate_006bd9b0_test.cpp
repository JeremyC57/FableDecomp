// Behaviour test for 0x006BD9B0 CMusicManager::InternalUpdate(const CState& new_state, bool force_play)
#include <stdio.h>
#include <vector>

class CMusicSetEntry { public: long NormalMusicID; long DangerMusicID; long LoopCount; };
static std::vector<CMusicSetEntry> g_sets;
static long g_played = -1, g_playedLoops = -1, g_stopped = -1;
static unsigned long g_fadeOut = 0;

class CMusicManager {
public:
    class CState {
    public:
        long GetActiveSet() const
        {
            if (UIOverrideSet) return UIOverrideSet;
            if (CinematicOverrideSet) return CinematicOverrideSet;
            if (OverrideSet) return OverrideSet;
            if (MapCellSet) return MapCellSet;
            return RegionSet;
        }
        bool GetDanger() const { if (GetActiveSet() == 0) return false; return Danger; }
        bool HasAnyOverride() const { return UIOverrideSet != 0 || CinematicOverrideSet != 0 || OverrideSet != 0; }
        long RegionSet, OverrideSet, UIOverrideSet, CinematicOverrideSet, MapCellSet;
        bool Paused, Danger;
    };
    class CFadeTimes { public: unsigned long FadeOutMS, FadeInMS; };
    CState CurrentState;
    void PlayMusic(long id, const CFadeTimes& f, long loops) { g_played = id; g_fadeOut = f.FadeOutMS; g_playedLoops = loops; }
    void StopMusic(unsigned long fade) { g_stopped = 1; g_fadeOut = fade; }
    void InternalUpdate(const CState& new_state, bool force_play);
};
static CMusicManager::CFadeTimes MusicFadeTimes[] = { { 2100, 2000 }, { 1200, 100 }, { 1500, 1500 }, { 400, 400 }, { 0, 0 } };
static const long NO_MUSIC_ID = 0x39;

void CMusicManager::InternalUpdate(const CState& new_state, bool force_play)   // the row's body
{
    long fade_index = 0;
    if (new_state.CinematicOverrideSet) {
        fade_index = 4;
    } else {
        if (CurrentState.GetDanger()) fade_index = 2;
        if (new_state.GetDanger()) fade_index |= 1;
    }
    const CFadeTimes& fade_times = MusicFadeTimes[fade_index];
    if (!force_play) {
        if (CurrentState.GetActiveSet() == new_state.GetActiveSet()
            && (CurrentState.HasAnyOverride() || CurrentState.Danger == new_state.Danger)
            && CurrentState.Paused == new_state.Paused) {
            CurrentState = new_state;
            return;
        }
    }
    if (!new_state.Paused && new_state.GetActiveSet() != 0) {
        long active_set = new_state.GetActiveSet();
        if ((unsigned long)active_set >= g_sets.size())
            return;
        long music_id;
        if (new_state.HasAnyOverride()) {
            music_id = g_sets[new_state.GetActiveSet()].NormalMusicID;
        } else if (new_state.GetDanger()) {
            music_id = g_sets[new_state.GetActiveSet()].DangerMusicID;
            if (!CurrentState.GetDanger() && music_id == g_sets[CurrentState.GetActiveSet()].NormalMusicID) {
                CurrentState = new_state;
                return;
            }
        } else {
            music_id = g_sets[new_state.GetActiveSet()].NormalMusicID;
            if (CurrentState.GetDanger() && music_id == g_sets[CurrentState.GetActiveSet()].DangerMusicID) {
                CurrentState = new_state;
                return;
            }
        }
        if (music_id == NO_MUSIC_ID) StopMusic(fade_times.FadeOutMS);
        else PlayMusic(music_id, fade_times, g_sets[new_state.GetActiveSet()].LoopCount);
    } else {
        StopMusic(fade_times.FadeOutMS);
    }
    CurrentState = new_state;
}

static void Reset() { g_played = -1; g_playedLoops = -1; g_stopped = -1; g_fadeOut = 0; }

int main()
{
    int ok = 1;
    CMusicSetEntry e0 = { 0, 0, 0 }, e1 = { 10, 11, 2 }, e2 = { 20, 21, 3 }, e3 = { 0x39, 30, 0 }, e4 = { 40, 10, 1 };
    g_sets.push_back(e0); g_sets.push_back(e1); g_sets.push_back(e2); g_sets.push_back(e3); g_sets.push_back(e4);
    CMusicManager m = { { 0, 0, 0, 0, 0, false, false } };
    CMusicManager::CState s = m.CurrentState;
    s.RegionSet = 1;                                        // region set 1: play its normal track, normal->normal fade
    Reset(); m.InternalUpdate(s, false);
    ok &= g_played == 10 && g_playedLoops == 2 && g_fadeOut == 2100 && m.CurrentState.RegionSet == 1;
    Reset(); m.InternalUpdate(s, false);                    // unchanged: nothing played
    ok &= g_played == -1 && g_stopped == -1;
    Reset(); m.InternalUpdate(s, true);                     // forced: replayed
    ok &= g_played == 10;
    s.Danger = true;                                        // danger: danger track, normal->danger fade
    Reset(); m.InternalUpdate(s, false);
    ok &= g_played == 11 && g_fadeOut == 1200 && m.CurrentState.Danger;
    s.OverrideSet = 2;                                      // override: its normal track even in danger
    Reset(); m.InternalUpdate(s, false);
    ok &= g_played == 20 && g_fadeOut == 400;
    s.OverrideSet = 0; s.RegionSet = 3; s.Danger = false;   // set 3's normal track is "no music": stop
    Reset(); m.InternalUpdate(s, false);
    ok &= g_stopped == 1 && g_played == -1 && m.CurrentState.RegionSet == 3;
    s.RegionSet = 9;                                        // out of range: nothing, state unchanged
    Reset(); m.InternalUpdate(s, false);
    ok &= g_played == -1 && g_stopped == -1 && m.CurrentState.RegionSet == 3;
    s.RegionSet = 1; m.CurrentState = s; m.CurrentState.Danger = true;   // in danger on set 1 (track 11) ...
    s.RegionSet = 4;                                        // ... set 4's normal track is 11's twin? (10): differs from 11
    Reset(); m.InternalUpdate(s, false);
    ok &= g_played == 40 && g_fadeOut == 1500;
    m.CurrentState.RegionSet = 1; m.CurrentState.Danger = false;   // normal on set 1 (track 10); new set 4 in danger -> 10: same
    s.RegionSet = 4; s.Danger = true;
    Reset(); m.InternalUpdate(s, false);
    ok &= g_played == -1 && g_stopped == -1 && m.CurrentState.RegionSet == 4 && m.CurrentState.Danger;
    s.CinematicOverrideSet = 2; s.Paused = true;            // paused: stop with the cinematic (zero) fade
    Reset(); m.InternalUpdate(s, false);
    ok &= g_stopped == 1 && g_fadeOut == 0 && m.CurrentState.Paused;
    printf(ok ? "006bd9b0_TEST PASS\n" : "006bd9b0_TEST FAIL\n");
    return ok ? 0 : 1;
}