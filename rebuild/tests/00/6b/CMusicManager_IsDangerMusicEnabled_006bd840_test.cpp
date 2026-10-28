// Behaviour test for 0x006BD840 CMusicManager::IsDangerMusicEnabled: the current state's GetDanger (standalone copy of the logic)
#include <cstdio>
struct State { long RegionSet, OverrideSet, UIOverrideSet, CinematicOverrideSet, MapCellSet; bool Paused, Danger;
    long GetActiveSet() const { if (UIOverrideSet) return UIOverrideSet; if (CinematicOverrideSet) return CinematicOverrideSet; if (OverrideSet) return OverrideSet; if (MapCellSet) return MapCellSet; return RegionSet; }
    bool GetDanger() const { if (GetActiveSet() == 0) return false; return Danger; } };
struct Manager { State CurrentState; bool IsDangerMusicEnabled() const { return CurrentState.GetDanger(); } };

int main()
{
    Manager m; m.CurrentState.RegionSet = 1; m.CurrentState.OverrideSet = m.CurrentState.UIOverrideSet = m.CurrentState.CinematicOverrideSet = m.CurrentState.MapCellSet = 0;
    m.CurrentState.Danger = true; if (!m.IsDangerMusicEnabled()) { std::printf("006bd840_TEST FAIL\n"); return 1; }
    m.CurrentState.RegionSet = 0; if (m.IsDangerMusicEnabled()) { std::printf("006bd840_TEST FAIL\n"); return 1; }
    std::printf("006bd840_TEST PASS\n");
    return 0;
}