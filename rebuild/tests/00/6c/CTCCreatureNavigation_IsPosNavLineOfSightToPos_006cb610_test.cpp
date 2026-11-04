// Behaviour test for CTCCreatureNavigation::IsPosNavLineOfSightToPos 0x006CB610: the answer is whatever the active navigator's
// IsNavLineOfSightClear (slot 10) says for the same two positions, passed through unchanged and in order.
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CNavigatorBase {
    virtual ~CNavigatorBase() { }
    virtual bool IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const = 0;
};
struct CWallNavigator : CNavigatorBase {               // blocked whenever the segment crosses x = 0
    mutable const C3DVector* lastFrom;
    mutable const C3DVector* lastTo;
    CWallNavigator() : lastFrom(0), lastTo(0) { }
    bool IsNavLineOfSightClear(const C3DVector& from, const C3DVector& to) const
    {
        lastFrom = &from; lastTo = &to;
        return (from.X < 0) == (to.X < 0);
    }
};
struct CTCCreatureNavigation {
    CNavigatorBase* PNavigator;
    bool IsPosNavLineOfSightToPos(const C3DVector& from, const C3DVector& to) const { return PNavigator->IsNavLineOfSightClear(from, to); }
};
int main()
{
    CWallNavigator nav;
    CTCCreatureNavigation tc;
    tc.PNavigator = &nav;
    C3DVector a = { -1, 0, 0 }, b = { 2, 0, 0 }, c = { -5, 3, 0 };
    if (tc.IsPosNavLineOfSightToPos(a, b) || !tc.IsPosNavLineOfSightToPos(a, c) || nav.lastFrom != &a || nav.lastTo != &c) {
        std::printf("006cb610_TEST FAIL\n"); return 1;
    }
    std::printf("006cb610_TEST PASS\n");
    return 0;
}