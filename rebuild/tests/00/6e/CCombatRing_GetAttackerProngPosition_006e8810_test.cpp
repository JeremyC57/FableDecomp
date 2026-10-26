#include <cstdio>
#include <cstdlib>
#include <cstring>
// behaviour test for 0x006E8810: exercises the same logic through a copy of the model (compiled separately from the source)
static bool near(float a, float b) { float d = a - b; if (d < 0) d = -d; return d < 1e-4f; }
class CThing { public: int id; };
class C3DVector { public: float x, y, z; C3DVector(float a, float b, float c) : x(a), y(b), z(c) {} };
struct CProng {
    CThing* m_pOccupier; float m_px, m_py, m_pz;
    C3DVector GetRotatedPosition(float angle) { return C3DVector(m_px + angle, m_py + angle, m_pz); }   // stand-in for the out-of-line rotation
};
struct CCombatRing {
    float RotationAngle; CProng m_prongs[3];
    CProng* GetAttackerProng(CThing* attacker) { for (int i = 0; i < 3; ++i) if (m_prongs[i].m_pOccupier == attacker) return &m_prongs[i]; return 0; }
    C3DVector GetAttackerProngPosition(CThing* attacker)
    {
        CProng* prong = GetAttackerProng(attacker);
        if (prong) return prong->GetRotatedPosition(RotationAngle);
        return C3DVector(1.0f, 0.0f, 0.0f);
    }
};
static bool Run()
{
    CThing a, b, c; CCombatRing ring; ring.RotationAngle = 0.5f;
    ring.m_prongs[0].m_pOccupier = &a; ring.m_prongs[1].m_pOccupier = &b; ring.m_prongs[2].m_pOccupier = 0;
    ring.m_prongs[1].m_px = 1.0f; ring.m_prongs[1].m_py = 2.0f; ring.m_prongs[1].m_pz = 3.0f;
    C3DVector p = ring.GetAttackerProngPosition(&b);
    if (!near(p.x, 1.5f) || !near(p.y, 2.5f) || !near(p.z, 3.0f)) return false;
    C3DVector q = ring.GetAttackerProngPosition(&c);
    return near(q.x, 1.0f) && near(q.y, 0.0f) && near(q.z, 0.0f);
}
int main()
{
    if (!Run()) { std::printf("006e8810_TEST FAIL\n"); return 1; }
    std::printf("006e8810_TEST PASS\n");
    return 0;
}