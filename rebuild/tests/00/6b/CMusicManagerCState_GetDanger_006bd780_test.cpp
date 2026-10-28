// Behaviour test for 0x006BD780 CMusicManager::CState::GetDanger: danger only while some music set is active (standalone copy of the logic)
#include <cstdio>
struct State { long RegionSet, OverrideSet, UIOverrideSet, CinematicOverrideSet, MapCellSet; bool Paused, Danger;
    long GetActiveSet() const { if (UIOverrideSet) return UIOverrideSet; if (CinematicOverrideSet) return CinematicOverrideSet; if (OverrideSet) return OverrideSet; if (MapCellSet) return MapCellSet; return RegionSet; }
    bool GetDanger() const { if (GetActiveSet() == 0) return false; return Danger; } };

int main()
{
    State s; s.RegionSet = s.OverrideSet = s.UIOverrideSet = s.CinematicOverrideSet = s.MapCellSet = 0; s.Danger = true;
    if (s.GetDanger()) { std::printf("006bd780_TEST FAIL\n"); return 1; }
    s.MapCellSet = 4; if (!s.GetDanger()) { std::printf("006bd780_TEST FAIL\n"); return 1; }
    s.Danger = false; if (s.GetDanger()) { std::printf("006bd780_TEST FAIL\n"); return 1; }
    s.Danger = true; s.MapCellSet = 0; s.UIOverrideSet = 2; if (s.GetActiveSet() != 2 || !s.GetDanger()) { std::printf("006bd780_TEST FAIL\n"); return 1; }
    std::printf("006bd780_TEST PASS\n");
    return 0;
}