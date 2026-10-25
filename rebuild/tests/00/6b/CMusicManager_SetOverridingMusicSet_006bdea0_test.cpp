// Behaviour test for 0x006BDEA0: CMusicManager::SetOverridingMusicSet copies CurrentState, replaces OverrideSet and hands the copy to InternalUpdate
#include <cstdio>

class CMusicManager {
public:
    class CState {
    public:
        long RegionSet;
        long OverrideSet;
        long UIOverrideSet;
        long CinematicOverrideSet;
        long MapCellSet;
        bool Paused;
        bool Danger;
        CState() : RegionSet(0), OverrideSet(0), UIOverrideSet(0), CinematicOverrideSet(0), MapCellSet(0), Paused(false), Danger(false) { }
    };
    CState CurrentState;
    CState Applied;                                 // test probes: what InternalUpdate received
    const CState* AppliedFrom;
    bool AppliedForce;
    int Updates;
    CMusicManager() : AppliedFrom(0), AppliedForce(false), Updates(0) { }
    void SetOverridingMusicSet(long music_set, bool force_play)     // the reconstructed body
    {
        CState new_state = CurrentState;
        new_state.OverrideSet = music_set;
        InternalUpdate(new_state, force_play);
    }
    void InternalUpdate(const CState& new_state, bool force_play)
    {
        Applied = new_state; AppliedFrom = &new_state; AppliedForce = force_play; ++Updates;
    }
};

int main()
{
    CMusicManager m;
    m.CurrentState.RegionSet = 3;
    m.CurrentState.OverrideSet = 4;
    m.CurrentState.UIOverrideSet = 5;
    m.CurrentState.CinematicOverrideSet = 6;
    m.CurrentState.MapCellSet = 7;
    m.CurrentState.Paused = true;
    m.CurrentState.Danger = true;
    m.SetOverridingMusicSet(11, true);
    if (m.Updates != 1 || m.Applied.OverrideSet != 11 || m.Applied.RegionSet != 3 || m.Applied.UIOverrideSet != 5
        || m.Applied.CinematicOverrideSet != 6 || m.Applied.MapCellSet != 7 || !m.Applied.Paused || !m.Applied.Danger
        || !m.AppliedForce || m.AppliedFrom == &m.CurrentState || m.CurrentState.OverrideSet != 4) {
        std::printf("006bdea0_TEST FAIL\n");
        return 1;
    }
    m.SetOverridingMusicSet(-1, false);
    if (m.Updates != 2 || m.Applied.OverrideSet != -1 || m.AppliedForce || m.CurrentState.OverrideSet != 4) {
        std::printf("006bdea0_TEST FAIL\n");
        return 1;
    }
    std::printf("006bdea0_TEST PASS\n");
    return 0;
}