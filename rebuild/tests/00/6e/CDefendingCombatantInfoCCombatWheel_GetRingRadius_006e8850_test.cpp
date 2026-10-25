// Test for 0x006E8850 CDefendingCombatantInfo::CCombatWheel::GetRingRadius: the source object must return the sum of
// RingThickness over rings 0..ring (28-byte retail rings), and 0 for a negative ring.
#include <stdio.h>
template <class T> struct CRetailVector { T* first; T* last; T* end_of_storage; };
class CDefendingCombatantInfo {
public:
    class CProng { public: bool Blocked; float Prong[3]; long ProngBlockageTimer; unsigned char POccupier[8]; };
    class CCombatRing {
    public:
        float RotationAngle; float DeltaRotation; long Index; float RingThickness; CRetailVector<CProng> Prongs;
    };
    class CCombatWheel {
    public:
        CRetailVector<CCombatRing> CombatRings;
        float GetRingRadius(long ring) const;
    };
};
int main()
{
    if (sizeof(CDefendingCombatantInfo::CCombatRing) != 28) { puts("006e8850_TEST FAIL size"); return 1; }
    CDefendingCombatantInfo::CCombatRing rings[4];
    for (int k = 0; k < 4; ++k) { rings[k].RotationAngle = 9.0f; rings[k].Index = k; rings[k].RingThickness = 1.5f * (k + 1); }
    CDefendingCombatantInfo::CCombatWheel wheel;
    wheel.CombatRings.first = rings; wheel.CombatRings.last = rings + 4; wheel.CombatRings.end_of_storage = rings + 4;
    bool ok = wheel.GetRingRadius(0) == 1.5f && wheel.GetRingRadius(1) == 4.5f && wheel.GetRingRadius(3) == 15.0f
        && wheel.GetRingRadius(-1) == 0.0f;
    if (!ok) { puts("006e8850_TEST FAIL"); return 1; }
    puts("006e8850_TEST PASS");
    return 0;
}
