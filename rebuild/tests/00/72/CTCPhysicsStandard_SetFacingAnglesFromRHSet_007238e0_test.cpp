#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
static int g_order = 0, g_updateAt = 0, g_movingAt = 0;
struct CTCPhysicsStandard {
    CRightHandedSet RHSet, OldRHSet;
    bool m_moving;
    virtual ~CTCPhysicsStandard() {}
    virtual void UpdateOldRHSet() { g_updateAt = ++g_order; OldRHSet = RHSet; }
    void SetAsMoving(bool moving) { g_movingAt = ++g_order; m_moving = moving; }
    void SetFacingAnglesFromRHSet(const CRightHandedSet& set)
    {
        UpdateOldRHSet();
        RHSet = set;
        SetAsMoving(true);
    }
};
int main()
{
    CTCPhysicsStandard p;
    p.m_moving = false;
    p.RHSet.Up.X = 1.0f; p.RHSet.Up.Y = 0.0f; p.RHSet.Up.Z = 0.0f; p.RHSet.Forward.X = 0.0f; p.RHSet.Forward.Y = 1.0f; p.RHSet.Forward.Z = 0.0f;
    CRightHandedSet s; s.Up.X = 0.0f; s.Up.Y = 0.0f; s.Up.Z = 1.0f; s.Forward.X = 1.0f; s.Forward.Y = 0.0f; s.Forward.Z = 0.0f;
    p.SetFacingAnglesFromRHSet(s);
    if (g_updateAt != 1 || g_movingAt != 2 || !p.m_moving || p.OldRHSet.Up.X != 1.0f || p.OldRHSet.Forward.Y != 1.0f ||
        p.RHSet.Up.Z != 1.0f || p.RHSet.Forward.X != 1.0f || p.RHSet.Forward.Y != 0.0f) { std::printf("007238e0_TEST FAIL\n"); return 1; }
    std::printf("007238e0_TEST PASS\n");
    return 0;
}