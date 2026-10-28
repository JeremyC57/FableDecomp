// CMusicManager::ClearUIOverridingMusicSet() -- apply a copy of the current music state with the selected setting.
// Leaf: ?ClearUIOverridingMusicSet@CMusicManager@@QAEXXZ
// Identity: proven; own state store matches Ego_r CState::CinematicOverrideSet; FableWin.pdb exact method; InternalUpdate(const CState&,bool)0x006BD9B0; current state24bytes.
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
    void ClearUIOverridingMusicSet();
protected:
    void InternalUpdate(const CState& new_state, bool force_play);   // 0x006BD9B0
};

void CMusicManager::ClearUIOverridingMusicSet()
{
    CState new_state = CurrentState;
    new_state.CinematicOverrideSet = 0;
    InternalUpdate(new_state, false);
}