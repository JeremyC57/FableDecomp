#include <cstdio>
// Behaviour model of CAIStateGroup_GoToPub::PayAtBar @ 0x0094AAC0.
struct C3DVector { float X, Y, Z; }; struct CRightHandedSet { C3DVector X, Y, Z; };
static bool g_pay = true, g_walk = true, g_money = true; static int g_terminated = 0; static C3DVector* g_walkedTo = 0;
struct CAIStateGroupBase { virtual void v00() {} void Terminate() { g_terminated++; } };
struct CAIStateGroup_GoToPub : CAIStateGroupBase { unsigned char _pub[0x64]; C3DVector m_payPoint; CRightHandedSet m_payOrientation;
    bool GetPayPoint() { return g_pay; } bool WalkToPoint(C3DVector* p, CRightHandedSet*) { g_walkedTo = p; return g_walk; } bool PayMoney() { return g_money; }
    bool PayAtBar() { if (!GetPayPoint()) { Terminate(); return false; } if (!WalkToPoint(&m_payPoint, &m_payOrientation)) { Terminate(); return false; } if (!PayMoney()) { Terminate(); return false; } return true; } };
int main()
{
    CAIStateGroup_GoToPub g;
    bool ok = g.PayAtBar() == true && g_terminated == 0 && g_walkedTo == &g.m_payPoint;
    g_money = false; ok = ok && g.PayAtBar() == false && g_terminated == 1;
    g_walk = false; g_walkedTo = 0; ok = ok && g.PayAtBar() == false && g_terminated == 2 && g_walkedTo == &g.m_payPoint;
    g_pay = false; g_walkedTo = 0; ok = ok && g.PayAtBar() == false && g_terminated == 3 && g_walkedTo == 0;
    if (!ok) { std::printf("0094aac0_TEST FAIL\n"); return 1; }
    std::printf("0094aac0_TEST PASS\n");
    return 0;
}