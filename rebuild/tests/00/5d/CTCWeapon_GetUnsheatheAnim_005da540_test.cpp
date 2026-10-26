#include <cstdio>
class CCharString { public: const char* m_text; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetLengthSquared() const { return X * X + Y * Y + Z * Z; }
};
class CRightHandedSet { public: C3DVector m_position; C3DVector m_lastPosition; CRightHandedSet() : m_position(0, 0, 0), m_lastPosition(0, 0, 0) { } };
class CThing { public: CRightHandedSet* m_pRHSet; const C3DVector& GetPosition() const { return m_pRHSet->m_position; } };
class CWeaponDef { public: CCharString UnsheatheToCombatAnim, UnsheatheToNormalAnim, SheatheFromCombatAnim, SheatheFromNormalAnim; };
static float g_MovingEpsilon = 1e-4f;
class CTCWeapon { public:
    CThing* ParentThing; const CWeaponDef* Def;
    const CCharString& GetUnsheatheAnim() const
    {
        CThing* thing = ParentThing;
        const C3DVector& last = thing->m_pRHSet->m_lastPosition;
        if ((thing->GetPosition() - last).GetLengthSquared() > g_MovingEpsilon * g_MovingEpsilon) return Def->UnsheatheToNormalAnim;
        return Def->UnsheatheToCombatAnim;
    }
};
int main()
{
    CWeaponDef d; CRightHandedSet rh; CThing t; t.m_pRHSet = &rh; CTCWeapon w; w.ParentThing = &t; w.Def = &d;
    if (&w.GetUnsheatheAnim() != &d.UnsheatheToCombatAnim) { std::printf("005da540_TEST FAIL\n"); return 1; }
    rh.m_position.X = 0.5f; rh.m_lastPosition.Z = -0.25f;
    if (&w.GetUnsheatheAnim() != &d.UnsheatheToNormalAnim) { std::printf("005da540_TEST FAIL\n"); return 1; }
    rh.m_position.X = 0.00001f; rh.m_lastPosition.Z = 0;
    if (&w.GetUnsheatheAnim() != &d.UnsheatheToCombatAnim) { std::printf("005da540_TEST FAIL\n"); return 1; }
    std::printf("005da540_TEST PASS\n");
    return 0;
}