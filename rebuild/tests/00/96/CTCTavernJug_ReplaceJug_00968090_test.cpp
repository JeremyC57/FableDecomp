#include <cstdio>
// Behaviour test for CTCTavernJug::ReplaceJug (0x00968090).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up, Forward; };
struct CTCPhysicsBase {
    int calls; int order[3]; const C3DVector* pos; const CRightHandedSet* rh; int active;
    virtual void ResetPosition(const C3DVector& p) { order[calls++] = 1; pos = &p; }
    virtual void ResetOrientation(const CRightHandedSet& r) { order[calls++] = 2; rh = &r; }
    virtual void SetPhysicsActive(bool a) { order[calls++] = 3; active = a ? 1 : 0; }
    virtual ~CTCPhysicsBase() {}
};
struct CThing { CTCPhysicsBase* m_pPhysics; };
struct CTCTavernJug {
    CThing* m_pParentThing;
    CRightHandedSet RHSet;
    C3DVector Pos;
    void ReplaceJug();
};
void CTCTavernJug::ReplaceJug()
{
    CTCPhysicsBase* physics = m_pParentThing->m_pPhysics;
    if (physics) {
        physics->ResetPosition(Pos);
        physics->ResetOrientation(RHSet);
        physics->SetPhysicsActive(false);
    }
}
int main()
{
    CTCPhysicsBase src; src.calls = 0; src.active = 7;
    CThing thing; thing.m_pPhysics = &src;
    CThing none; none.m_pPhysics = 0;
    CTCTavernJug o; o.m_pParentThing = &thing;
    o.ReplaceJug();
    if (src.calls != 3 || src.order[0] != 1 || src.order[1] != 2 || src.order[2] != 3 || src.pos != &o.Pos
        || src.rh != &o.RHSet || src.active != 0) { std::printf("00968090_TEST FAIL\n"); return 1; }
    src.calls = 0;
    o.m_pParentThing = &none;
    o.ReplaceJug();   // no physics component: nothing happens
    if (src.calls != 0) { std::printf("00968090_TEST FAIL\n"); return 1; }
    std::printf("00968090_TEST PASS\n");
    return 0;
}