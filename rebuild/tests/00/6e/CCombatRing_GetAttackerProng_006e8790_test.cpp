#include <cstdio>

// Model of CCombatRing::GetAttackerProng @ 0x006E8790
struct CThing { int id; };
struct CBaseIntelligentPointer { virtual void v00() {} CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
struct CProng { unsigned char _pad[0x14]; CBaseIntelligentPointer PAttacker; };
struct CProngVector { CProng* m_pFirst; CProng* m_pLast; CProng* m_pEnd; };
struct CCombatRing { float RotationAngle; float DeltaRotation; long Index; float RingThickness; CProngVector Prongs;
    CProng* GetAttackerProng(const CThing* attacker) { for (CProng* p = Prongs.m_pFirst; p != Prongs.m_pLast; ++p) { if (p->PAttacker.GetPItem() == attacker) return p; } return 0; } };
int main()
{
    CThing t0, t1, t2; CProng prongs[3]; prongs[0].PAttacker.m_pItem = &t0; prongs[1].PAttacker.m_pItem = &t1; prongs[2].PAttacker.m_pItem = 0;
    CCombatRing ring; ring.Prongs.m_pFirst = prongs; ring.Prongs.m_pLast = prongs + 3; ring.Prongs.m_pEnd = prongs + 3;
    if (ring.GetAttackerProng(&t1) != &prongs[1] || ring.GetAttackerProng(&t0) != &prongs[0] || ring.GetAttackerProng(&t2) != 0 || (sizeof(void*) == 4 && sizeof(CProng) != 0x1c) || (sizeof(void*) == 4 && (char*)&ring.Prongs - (char*)&ring != 0x10)) { std::printf("006e8790_TEST FAIL\n"); return 1; }
    ring.Prongs.m_pLast = prongs; if (ring.GetAttackerProng(&t0) != 0) { std::printf("006e8790_TEST FAIL\n"); return 1; }
    std::printf("006e8790_TEST PASS\n"); return 0;
}