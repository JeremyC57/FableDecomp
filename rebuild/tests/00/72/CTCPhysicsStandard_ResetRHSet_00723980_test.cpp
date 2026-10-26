#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
struct CTCPhysicsStandard {
    CRightHandedSet RHSet, OldRHSet;
    int m_setCalls;
    virtual ~CTCPhysicsStandard() {}
    virtual void SetFacingAnglesFromRHSet(const CRightHandedSet& set) { ++m_setCalls; RHSet = set; }
    void ResetRHSet(const CRightHandedSet& set)
    {
        SetFacingAnglesFromRHSet(set);
        OldRHSet = set;
    }
};
int main()
{
    CTCPhysicsStandard p; p.m_setCalls = 0;
    p.OldRHSet.Up.X = 5.0f;
    CRightHandedSet s; s.Up.X = 0.0f; s.Up.Y = 1.0f; s.Up.Z = 0.0f; s.Forward.X = 0.5f; s.Forward.Y = 0.0f; s.Forward.Z = -0.5f;
    p.ResetRHSet(s);
    if (p.m_setCalls != 1 || p.RHSet.Up.Y != 1.0f || p.OldRHSet.Up.X != 0.0f || p.OldRHSet.Up.Y != 1.0f ||
        p.OldRHSet.Forward.X != 0.5f || p.OldRHSet.Forward.Z != -0.5f) { std::printf("00723980_TEST FAIL\n"); return 1; }
    std::printf("00723980_TEST PASS\n");
    return 0;
}