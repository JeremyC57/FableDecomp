// CMusicManager::InternalUpdate(const CState& new_state, bool force_play) -- switch the music to a new state: pick the fade times for the danger / cinematic change, stop the music when paused or no set is active, otherwise play the set's normal or danger track unless it is already playing, then store the state
// Leaf: ?InternalUpdate@CMusicManager@@IAEXABVCState@1@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BD9B0, 676 bytes (+ int3 padding), thiscall, `ret 8`.
// Identity: FableWin.pdb / Ego_r.pdb ?InternalUpdate@CMusicManager@@IAEXABVCState@1@_N@Z (protected); the eight callers are the
//   landed CState setters (EnableDangerMusic 0x006BDE50, SetOverridingMusicSet 0x006BDEA0, ... 0x006BE030), each passing a copy of
//   CurrentState with one member changed. The catalog "CMusicManager_CopyAssign" is a descriptive placeholder.
// Callees: CMusicManager::CState::GetActiveSet() const 0x006BD760 and GetDanger() const 0x006BD780 (inline class bodies, expanded at
//   the first sites and called at the later ones), CMusicManager::PlayMusic(long, const CFadeTimes&, long) 0x006BD900, the sound
//   manager's StopMusic(fadeOutMS) (virtual slot 18, +0x48) on PSoundManager 0x013B8394.
// Data: the five-entry fade-time table 0x0137FAE8 (FadeOutMS / FadeInMS: {2100,2000} normal->normal, {1200,100} normal->danger,
//   {1500,1500} danger->normal, {400,400} danger->danger, {0,0} cinematic); its original name is in no name set on disk
//   (descriptive). Music ID 0x39 is the "no music" track of the sound def's music sets (descriptive).
// Shape: the override test is the inline CState::HasAnyOverride() (out of line at 0x006BD7E0): with the `||` chain written in
//   place, VC7.1 expands GetActiveSet in the override arm (0x006BDB5D, retail calls it) and the copy-and-return blocks merge
//   differently. The 24-byte state copies are the trivial copy assignment (six dwords).
// Header overlay: hdr/engine/CMusicManager.h (new), hdr/engine/CSoundDef.h (FABLE_RETAIL_CSOUNDDEF_MUSICSETENTRIES).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CSOUNDDEF_MUSICSETENTRIES
#define FABLE_RETAIL_CMUSICMANAGER_OVERRIDE_TESTS
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CMusicManager.h"
#include "engine/CASoundManager.h"

static CMusicManager::CFadeTimes MusicFadeTimes[] = {         // 0x0137FAE8 (descriptive name)
    { 2100, 2000 },                                           // normal -> normal
    { 1200, 100 },                                            // normal -> danger
    { 1500, 1500 },                                           // danger -> normal
    { 400, 400 },                                             // danger -> danger
    { 0, 0 },                                                 // cinematic
};

static const long NO_MUSIC_ID = 0x39;                         // descriptive

void CMusicManager::InternalUpdate(const CState& new_state, bool force_play)
{
    long fade_index = 0;
    if (new_state.CinematicOverrideSet) {
        fade_index = 4;
    } else {
        if (CurrentState.GetDanger())
            fade_index = 2;
        if (new_state.GetDanger())
            fade_index |= 1;
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
        const CSoundDef& sound_def = GetSoundDef();
        if (active_set >= sound_def.MusicSetEntries.size())
            return;
        long music_id;
        if (new_state.HasAnyOverride()) {
            music_id = sound_def.MusicSetEntries[new_state.GetActiveSet()].NormalMusicID;
        } else if (new_state.GetDanger()) {
            music_id = sound_def.MusicSetEntries[new_state.GetActiveSet()].DangerMusicID;
            if (!CurrentState.GetDanger()
                && music_id == sound_def.MusicSetEntries[CurrentState.GetActiveSet()].NormalMusicID) {
                CurrentState = new_state;
                return;
            }
        } else {
            music_id = sound_def.MusicSetEntries[new_state.GetActiveSet()].NormalMusicID;
            if (CurrentState.GetDanger()
                && music_id == sound_def.MusicSetEntries[CurrentState.GetActiveSet()].DangerMusicID) {
                CurrentState = new_state;
                return;
            }
        }
        if (music_id == NO_MUSIC_ID)
            PSoundManager->StopMusic(fade_times.FadeOutMS);
        else
            PlayMusic(music_id, fade_times, sound_def.MusicSetEntries[new_state.GetActiveSet()].LoopCount);
    } else {
        PSoundManager->StopMusic(fade_times.FadeOutMS);
    }
    CurrentState = new_state;
}