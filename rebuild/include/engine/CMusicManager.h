#ifndef FABLE_ENGINE_CMUSICMANAGER_H
#define FABLE_ENGINE_CMUSICMANAGER_H
// CMusicManager (new shared header, chunk 1026, row 0x006BD9B0 InternalUpdate). Ego_r.pdb members (struct_layouts_egor.tsv,
// sizeof 0x44 with a 16-byte Dinkumware vector) at the retail offsets of the landed TU (ctor 0x006BE300, UpdateCurrentMusicOffset
// 0x006BD8A0, the CState setters 0x006BDE50..0x006BE030): CurrentState +0x00, PSoundDef +0x18, RegisteredMusic +0x1C (CVectorMap,
// 12-byte STLport vector, Compare +0x28, Dirty +0x29), CurrentMusicID +0x2C, PendingMusicID +0x30, PPendingFadeTimes +0x34,
// PendingLoopCount +0x38, Paused +0x3C; sizeof 0x40.
// Nested classes (Ego_r.pdb): CState (five longs + Paused / Danger, 0x18), CFadeTimes (FadeOutMS / FadeInMS, 8),
// CRegisteredMusicEntry (SrcFileName, DstFileName, Gain, Offset, CacheToHD; 0x14).
// Member functions: the FableWin.pdb / Ego_r.pdb name set (CMusicManager 0x006FA030..0x006FA160 in Ego_r), declarations only,
// except the two CState accessors, which retail keeps out of line (GetActiveSet 0x006BD760, GetDanger 0x006BD780) and also
// expands (0x006BD9CA, 0x006BD9F8 in InternalUpdate): inline bodies in the class.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CVectorMap.h"
#include "engine/CSoundDef.h"                                        // GetSoundDef dereferences the handle
class CMusicManager {
public:
    class CState {                                                          // 0x18 bytes
    public:
        CState();                                                           // ??0CState@CMusicManager@@QAE@XZ (zeroes every member)
        // ?GetActiveSet@CState@CMusicManager@@QBEJXZ (0x006BD760): the first non-zero set in priority order.
        long GetActiveSet() const
        {
            if (UIOverrideSet)
                return UIOverrideSet;
            if (CinematicOverrideSet)
                return CinematicOverrideSet;
            if (OverrideSet)
                return OverrideSet;
            if (MapCellSet)
                return MapCellSet;
            return RegionSet;
        }
        // ?GetDanger@CState@CMusicManager@@QBE_NXZ (0x006BD780; the landed body).
        bool GetDanger() const
        {
            if (GetActiveSet() == 0)
                return false;
            return Danger;
        }
#ifdef FABLE_RETAIL_CMUSICMANAGER_OVERRIDE_TESTS
        // Retail opt-in (chunk 1026, row 0x006BD9B0): the override tests of the CState TU cluster, out of line (unreferenced
        // COMDATs) at 0x006BD7B0 (`UIOverrideSet != 0`), 0x006BD7C0 (cinematic), 0x006BD7D0 (override) and 0x006BD7E0 (any of
        // the three, `mov eax,1` / `xor eax,eax`). Their original names are in no name set on disk (descriptive).
        bool HasUIOverride() const { return UIOverrideSet != 0; }
        bool HasCinematicOverride() const { return CinematicOverrideSet != 0; }
        bool HasOverride() const { return OverrideSet != 0; }
        bool HasAnyOverride() const { return HasUIOverride() || HasCinematicOverride() || HasOverride(); }
#endif
        long RegionSet;                                                     // +0x00
        long OverrideSet;                                                   // +0x04
        long UIOverrideSet;                                                 // +0x08
        long CinematicOverrideSet;                                          // +0x0C
        long MapCellSet;                                                    // +0x10
        bool Paused;                                                        // +0x14
        bool Danger;                                                        // +0x15
    };
    class CFadeTimes {                                                      // 8 bytes
    public:
        unsigned long FadeOutMS;                                            // +0x00
        unsigned long FadeInMS;                                             // +0x04
    };
    class CRegisteredMusicEntry {                                           // 0x14 bytes
    public:
        CWideString SrcFileName;                                            // +0x00
        CWideString DstFileName;                                            // +0x04
        float Gain;                                                         // +0x08
        float Offset;                                                       // +0x0C
        bool CacheToHD;                                                     // +0x10
    };

    CMusicManager();                                                        // 0x006BE300
    void PauseMusic(bool pause);
    void Update();
    void EnableDangerMusic(bool enable);                                    // 0x006BDE50
    bool IsDangerMusicEnabled() const;                                      // 0x006BD840
    void SetOverridingMusicSet(long music_set, bool force_play);            // 0x006BDEA0
    void ClearOverridingMusicSet();                                         // 0x006BDEF0
    void SetCinematicOverridingMusicSet(long music_set);                    // 0x006BDF40
    void ClearCinematicOverridingMusicSet();                                // 0x006BDF90
    void CacheMusicSet(long music_set);
    void SetUIOverridingMusicSet(long music_set);                           // 0x006BDFE0
    void ClearUIOverridingMusicSet();                                       // 0x006BE030
    void ResetMusicEntriesOffsets();                                        // 0x006BD870
protected:
    void UpdateCurrentMusicOffset();                                        // 0x006BD8A0
    void InternalUpdate(const CState& new_state, bool force_play);          // 0x006BD9B0
    void CacheMusic(long music_id);
    void PlayMusic(long music_id, const CFadeTimes& fade_times, long loop_count);   // 0x006BD900
    void RegisterMusic();                                                   // 0x006BE080
    const CSoundDef& GetSoundDef() const { return *PSoundDef; }             // ?GetSoundDef@CMusicManager@@IBEABVCSoundDef@@XZ (FableWin)

    CState CurrentState;                                                    // +0x00
    CDefPointer<const CSoundDef> PSoundDef;                                 // +0x18
    CVectorMap<long, CRegisteredMusicEntry> RegisteredMusic;                // +0x1C
    long CurrentMusicID;                                                    // +0x2C
    long PendingMusicID;                                                    // +0x30
    CFadeTimes* PPendingFadeTimes;                                          // +0x34
    long PendingLoopCount;                                                  // +0x38
    bool Paused;                                                            // +0x3C
};
#endif
