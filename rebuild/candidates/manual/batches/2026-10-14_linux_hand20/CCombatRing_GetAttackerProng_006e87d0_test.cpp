#include <cstdio>
struct CThing { int x; };
struct CBaseIntelligentPointer { virtual void v00() { } CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CProng { bool Blocked; float Prong_X, Prong_Y, Prong_Z; long ProngBlockageTimer; CBaseIntelligentPointer POccupier; };
struct CCombatRing {
    float RotationAngle, DeltaRotation; long Index; float RingThickness; CProng* first; CProng* last; CProng* end_of_storage;
    CProng* GetAttackerProng(CThing* attacker) { for (CProng* p = first; p != last; ++p) if (p->POccupier.GetPItem() == attacker) return p; return 0; }
};
int main()
{
    CThing a, b, c; CProng prongs[3]; prongs[0].POccupier.m_pItem = &a; prongs[1].POccupier.m_pItem = &b; prongs[2].POccupier.m_pItem = 0;
    CCombatRing ring; ring.first = prongs; ring.last = prongs + 3;
    if (ring.GetAttackerProng(&b) != &prongs[1] || ring.GetAttackerProng(&a) != &prongs[0] || ring.GetAttackerProng(&c) != 0) { std::printf("006e87d0_TEST FAIL\n"); return 1; }
    ring.last = prongs;
    if (ring.GetAttackerProng(&a) != 0) { std::printf("006e87d0_TEST FAIL\n"); return 1; }
    std::printf("006e87d0_TEST PASS\n");
    return 0;
}
