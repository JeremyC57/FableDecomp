#include <cstdio>
struct CThing { unsigned long m_properties; };
struct CTCWeaponTrail {
    CThing* ParentThing; long NoTrails; long NoProjectileTrails;
    bool HasWeaponTrailPoints() const { if (ParentThing->m_properties & 0x00800000) return NoProjectileTrails == 1; return NoTrails > 0; }
};
int main()
{
    CThing t; t.m_properties = 0; CTCWeaponTrail w; w.ParentThing = &t; w.NoTrails = 0; w.NoProjectileTrails = 1;
    if (w.HasWeaponTrailPoints()) { std::printf("0076e4a0_TEST FAIL\n"); return 1; }
    w.NoTrails = 2; if (!w.HasWeaponTrailPoints()) { std::printf("0076e4a0_TEST FAIL\n"); return 1; }
    w.NoTrails = -1; if (w.HasWeaponTrailPoints()) { std::printf("0076e4a0_TEST FAIL\n"); return 1; }
    t.m_properties = 0x00800001; if (!w.HasWeaponTrailPoints()) { std::printf("0076e4a0_TEST FAIL\n"); return 1; }
    w.NoProjectileTrails = 2; if (w.HasWeaponTrailPoints()) { std::printf("0076e4a0_TEST FAIL\n"); return 1; }
    std::printf("0076e4a0_TEST PASS\n");
    return 0;
}