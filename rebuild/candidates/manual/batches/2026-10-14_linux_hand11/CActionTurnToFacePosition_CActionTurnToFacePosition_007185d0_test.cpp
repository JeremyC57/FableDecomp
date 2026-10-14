#include <cstdio>

static int g_base = 0;
struct CThing;
struct C3DVector { long x, y, z; C3DVector(long a, long b, long c) : x(a), y(b), z(c) { } C3DVector(const C3DVector& o) : x(o.x), y(o.y), z(o.z) { } };
struct CBase { virtual void v00() { } CBase() { ++g_base; } };
struct __declspec(novtable) CAction : CBase { CThing* m_pThing; bool m_active; bool m_finished;
    CAction() : CBase(), m_pThing(0), m_active(false), m_finished(false) { } };
struct CActionTurnToFacePosition : CAction { C3DVector m_position; long m_flags;
    virtual void v00() { }
    CActionTurnToFacePosition(const C3DVector& position, long flags) : CAction(), m_position(position), m_flags(flags) { } };
int main()
{
    C3DVector v(1, 2, 3); CActionTurnToFacePosition a(v, 9);
    if (g_base != 1 || a.m_pThing != 0 || a.m_active || a.m_finished || a.m_position.x != 1 || a.m_position.y != 2 || a.m_position.z != 3 || a.m_flags != 9 || sizeof(a) != 0x1c) { std::printf("007185d0_TEST FAIL\n"); return 1; }
    std::printf("007185d0_TEST PASS\n"); return 0;
}
