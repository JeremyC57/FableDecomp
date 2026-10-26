#include <cstdio>
class C3DVector { public: float X, Y, Z; };
struct CTCPhysicsNavigator {
    bool m_accept;
    bool Moving : 1;
    bool OldMoving : 1;
    C3DVector PositionToNavigateTo;
    virtual ~CTCPhysicsNavigator() {}
    virtual bool vfunc_41() { return m_accept; }
    void SetPositionToNavigateTo(const C3DVector& pos)
    {
        if (vfunc_41()) {
            PositionToNavigateTo = pos;
            Moving = true;
        }
    }
};
int main()
{
    CTCPhysicsNavigator n;
    n.Moving = false; n.OldMoving = true;
    n.PositionToNavigateTo.X = n.PositionToNavigateTo.Y = n.PositionToNavigateTo.Z = 0.0f;
    C3DVector p; p.X = 1.5f; p.Y = -2.0f; p.Z = 3.0f;
    n.m_accept = false; n.SetPositionToNavigateTo(p);
    if (n.Moving || n.PositionToNavigateTo.X != 0.0f) { std::printf("00726920_TEST FAIL\n"); return 1; }
    n.m_accept = true; n.SetPositionToNavigateTo(p);
    if (!n.Moving || !n.OldMoving || n.PositionToNavigateTo.X != 1.5f || n.PositionToNavigateTo.Y != -2.0f ||
        n.PositionToNavigateTo.Z != 3.0f) { std::printf("00726920_TEST FAIL\n"); return 1; }
    std::printf("00726920_TEST PASS\n");
    return 0;
}