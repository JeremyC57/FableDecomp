// Behaviour test for CMusicManager::CState::GetActiveSet() const (0x006BD760): the first non-zero of the UI override,
// cinematic override, override and map-cell sets, else the region set. Standalone model of the CState members it reads.
#include <cstdio>

class CState { public:
    long RegionSet;                 // +0x00
    long OverrideSet;               // +0x04
    long UIOverrideSet;             // +0x08
    long CinematicOverrideSet;      // +0x0C
    long MapCellSet;                // +0x10
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
};

static long Active(long region, long over, long ui, long cine, long cell)
{
    CState s;
    s.RegionSet = region; s.OverrideSet = over; s.UIOverrideSet = ui; s.CinematicOverrideSet = cine; s.MapCellSet = cell;
    return s.GetActiveSet();
}

int main()
{
    int ok = 1;
    ok &= Active(5, 2, 1, 3, 4) == 1;   // UI override wins
    ok &= Active(5, 2, 0, 3, 4) == 3;   // then cinematic
    ok &= Active(5, 2, 0, 0, 4) == 2;   // then override
    ok &= Active(5, 0, 0, 0, 4) == 4;   // then map cell
    ok &= Active(5, 0, 0, 0, 0) == 5;   // else region
    ok &= Active(0, 0, 0, 0, 0) == 0;   // nothing active
    if (ok) printf("BEHAVIOUR_PASS_006bd760\n");
    else    printf("BEHAVIOUR_FAIL\n");
    return 0;
}
