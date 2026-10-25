// CMusicManager::SetOverridingMusicSet(long music_set, bool force_play) -- copy the current music state, replace its override set and apply it
// Leaf: ?SetOverridingMusicSet@CMusicManager@@QAEXJ_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BDEA0, 74 bytes, thiscall, `ret 8`.
// Identity: FableWin.pdb ?SetOverridingMusicSet@CMusicManager@@QAEXJ_N@Z; the only caller is CGameScriptInterface::
//   OverrideMusic (0x00891294: `OverrideMusic(type, false, force)` reaches world +0x6C, the music manager). Ego_r.pdb:
//   CMusicManager::CurrentState (CState, 24 bytes: RegionSet, OverrideSet +4, UIOverrideSet, CinematicOverrideSet,
//   MapCellSet, Paused, Danger) at +0; InternalUpdate(const CState&, bool) is protected. The sibling 0x006BDF40 has the
//   same shape for another state slot with a constant flag. Catalog "vector<EWeaponClass>::_Umove<EWeaponClass*>" is
//   propagated.
// Callees: CMusicManager::InternalUpdate(const CState&, bool) @ 0x006BD9B0 (manifest "CMusicManager_CopyAssign").
// Shape: `CState new_state = CurrentState; new_state.OverrideSet = music_set; InternalUpdate(new_state, force_play);`
//   CState declares only a default constructor, so the copy is the trivial bitwise one (six dwords through edx/eax, the
//   two bools in the last dword); the bool argument is forwarded as its whole stack dword.
class CSoundDef;
class CMusicManager {
public:
    class CState {                              // 24 bytes
    public:
        long RegionSet;                         // +0x00
        long OverrideSet;                       // +0x04
        long UIOverrideSet;                     // +0x08
        long CinematicOverrideSet;              // +0x0C
        long MapCellSet;                        // +0x10
        bool Paused;                            // +0x14
        bool Danger;                            // +0x15
        CState();
    };
    class CFadeTimes;
    CState CurrentState;                        // +0x00
    void SetOverridingMusicSet(long music_set, bool force_play);
protected:
    void InternalUpdate(const CState& new_state, bool force_play);   // 0x006BD9B0
};

void CMusicManager::SetOverridingMusicSet(long music_set, bool force_play)
{
    CState new_state = CurrentState;
    new_state.OverrideSet = music_set;
    InternalUpdate(new_state, force_play);
}