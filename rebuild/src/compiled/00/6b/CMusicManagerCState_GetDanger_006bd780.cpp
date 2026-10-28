// CMusicManager::CState::GetDanger() const -- the music state's danger flag, but only while some music set (UI, cinematic, override, map-cell or region) is active
// Leaf: ?GetDanger@CState@CMusicManager@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BD780, 41 bytes, thiscall, `ret`.
// Identity (probable): FableWin.pdb names ?GetDanger@CState@CMusicManager@@QBE_NXZ and ?GetActiveSet@CState@CMusicManager@@QBEJXZ;
//   the body reads only Ego_r.pdb CMusicManager::CState members (RegionSet +0x00, OverrideSet +0x04, UIOverrideSet +0x08,
//   CinematicOverrideSet +0x0C, MapCellSet +0x10, Danger +0x15). Its byte twin 0x006BD840 is CMusicManager::
//   IsDangerMusicEnabled (the tail jump of CGameScriptInterface::IsDangerMusicEnabled 0x00891330 lands there, and the
//   manager's CurrentState sits at +0); this copy is called three times by 0x006BD9B0, which compares states. The catalog
//   label "IsDangerMusicEnabled" on this address is propagated from the twin.
// Shape: GetActiveSet inline (the first non-zero set in priority order UI, cinematic, override, map cell, region), so
//   every test loads into eax (the would-be return register) -- including the last one, which a plain `||` chain turns
//   into `cmp [ecx],0`.
class CMusicManager {
public:
    class CState {
    public:
        long RegionSet;                                 // +0x00
        long OverrideSet;                               // +0x04
        long UIOverrideSet;                             // +0x08
        long CinematicOverrideSet;                      // +0x0C
        long MapCellSet;                                // +0x10
        bool Paused;                                    // +0x14
        bool Danger;                                    // +0x15
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
        bool GetDanger() const;
    };
};

bool CMusicManager::CState::GetDanger() const
{
    if (GetActiveSet() == 0)
        return false;
    return Danger;
}