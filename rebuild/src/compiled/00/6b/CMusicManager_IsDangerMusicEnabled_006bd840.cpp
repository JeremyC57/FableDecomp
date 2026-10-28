// CMusicManager::IsDangerMusicEnabled() const -- whether danger music is on: the current state's danger flag while some music set is active
// Leaf: ?IsDangerMusicEnabled@CMusicManager@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BD840, 41 bytes, thiscall, `ret`.
// Identity (proven): the only reference is the tail jump of CGameScriptInterface::IsDangerMusicEnabled 0x00891330
//   (`mov ecx,[[this+4]+6Ch] ; jmp 006BD840`, the script interface's music manager); Ego_r.pdb
//   ?IsDangerMusicEnabled@CMusicManager@@QBE_NXZ. Ego_r.pdb CMusicManager::CurrentState (CState) is the first member, so
//   the inlined CState::GetDanger (out of line at 0x006BD780) reads the same offsets.
// Shape: CState::GetDanger with GetActiveSet inline (see 0x006BD780).
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
        bool GetDanger() const                          // 0x006BD780 out of line
        {
            if (GetActiveSet() == 0)
                return false;
            return Danger;
        }
    };
    CState CurrentState;                                // +0x00
    bool IsDangerMusicEnabled() const;
};

bool CMusicManager::IsDangerMusicEnabled() const
{
    return CurrentState.GetDanger();
}