// Behaviour test for 0x00984B40 (label IsOverrideableByActionOfSamePriority): target present, alive, interface 81, farther than 3 m.
#include <cstdio>
#include <cmath>
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    float Length() const { return (float)std::sqrt(X * X + Y * Y + Z * Z); }
};
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { C3DVector r; r.X = a.X - b.X; r.Y = a.Y - b.Y; r.Z = a.Z - b.Z; return r; }
struct CThing {
    unsigned int m_presentBits[9]; unsigned char m_flags; C3DVector pos;
    CThing() : m_flags(0) { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; pos.X = pos.Y = pos.Z = 0; }
    bool IsDead() const { return (m_flags & 1) != 0; }
    const C3DVector& GetPosition() const { return pos; }
};
struct CReactionOwner { CThing* m_pCreature; };
struct CRecord {
    CReactionOwner* m_pOwner; CThing* target;
    bool IsTargetValidAndDistant()
    {
        if (target != 0) {
            if (!target->IsDead()) {
                if (target->m_presentBits[2] & 0x20000) {
                    CThing* creature = m_pOwner->m_pCreature;
                    return (target->GetPosition() - creature->GetPosition()).Length() > 3.0f;
                }
            }
        }
        return false;
    }
};
int main()
{
    CThing me, them; CReactionOwner owner = { &me };
    them.m_presentBits[2] = 0x20000; them.pos.X = 4.0f;
    CRecord r; r.m_pOwner = &owner; r.target = &them;
    if (!r.IsTargetValidAndDistant()) { std::printf("00984b40_TEST FAIL\n"); return 1; }
    them.pos.X = 2.0f; if (r.IsTargetValidAndDistant()) { std::printf("00984b40_TEST FAIL\n"); return 1; }
    them.pos.X = 5.0f; them.m_flags = 1; if (r.IsTargetValidAndDistant()) { std::printf("00984b40_TEST FAIL\n"); return 1; }
    them.m_flags = 0; them.m_presentBits[2] = 0; if (r.IsTargetValidAndDistant()) { std::printf("00984b40_TEST FAIL\n"); return 1; }
    r.target = 0; if (r.IsTargetValidAndDistant()) { std::printf("00984b40_TEST FAIL\n"); return 1; }
    std::printf("00984b40_TEST PASS\n");
    return 0;
}