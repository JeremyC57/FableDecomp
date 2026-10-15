#include <cstdio>
// Behaviour test for CTCTavernTankard::ReplaceTankard (0x00947C30).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CTCPhysicsBase {
    int calls; int order[3]; const C3DVector* pos; const CRightHandedSet* rh; int active;
    virtual void ResetPosition(const C3DVector& p) { order[calls++] = 1; pos = &p; }
    virtual void SetOrientation(const CRightHandedSet& r) { order[calls++] = 2; rh = &r; }
    virtual void SetPhysicsActive(bool a) { order[calls++] = 3; active = a ? 1 : 0; }
    virtual ~CTCPhysicsBase() {}
};
struct CThing { CTCPhysicsBase* m_pPhysics; };
struct CTCTavernTankard {
    CThing* m_pParentThing;
    CRightHandedSet RHSet;
    C3DVector Pos;
    void ReplaceTankard();
};
void CTCTavernTankard::ReplaceTankard()
{
    CTCPhysicsBase* physics = m_pParentThing->m_pPhysics;
    physics->ResetPosition(Pos);
    physics->SetOrientation(RHSet);
    physics->SetPhysicsActive(false);
}
int main()
{
    CTCPhysicsBase src; src.calls = 0; src.active = 7;
    CThing thing; thing.m_pPhysics = &src;
    CThing none; none.m_pPhysics = 0;
    CTCTavernTankard o; o.m_pParentThing = &thing;
    o.ReplaceTankard();
    if (src.calls != 3 || src.order[0] != 1 || src.order[1] != 2 || src.order[2] != 3 || src.pos != &o.Pos
        || src.rh != &o.RHSet || src.active != 0) { std::printf("00947c30_TEST FAIL\n"); return 1; }
    src.calls = 0;

    std::printf("00947c30_TEST PASS\n");
    return 0;
}