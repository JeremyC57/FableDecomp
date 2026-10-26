// behaviour test for CThingCreatureBase::AutoPlaceNear (0x00665DF0)
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
static int g_resets = 0;
class CTCCreatureNavigation { public: bool found; C3DVector spot;
    bool GetNearbyWanderPosition(const C3DVector*, float radius, C3DVector* result, bool) { if (found && radius > 0) { *result = spot; return true; } return false; }
    void ResetNavigator() { ++g_resets; } };
class CTCPhysicsBase { public: C3DVector pos; void ResetPosition(const C3DVector& p) { pos = p; } };
class CThingCreatureBase { public: CTCCreatureNavigation* nav; CTCPhysicsBase* PhysicsTC;
    bool AutoPlaceNear(const C3DVector& position, float radius)
    {
        CTCCreatureNavigation* navigation = nav;
        C3DVector placed = position;
        if (navigation->GetNearbyWanderPosition(&position, radius, &placed, false)) {
            navigation->ResetNavigator(); PhysicsTC->ResetPosition(placed); navigation->ResetNavigator(); return true;
        }
        return false;
    } };
int main()
{
    CTCCreatureNavigation n; n.found = true; n.spot.X = 5; n.spot.Y = 6; n.spot.Z = 7;
    CTCPhysicsBase p; p.pos.X = 0; CThingCreatureBase c; c.nav = &n; c.PhysicsTC = &p;
    C3DVector at; at.X = 1; at.Y = 2; at.Z = 3;
    int bad = !c.AutoPlaceNear(at, 2.0f) || p.pos.X != 5 || p.pos.Z != 7 || g_resets != 2;
    n.found = false; p.pos.X = 0; if (c.AutoPlaceNear(at, 2.0f) || p.pos.X != 0 || g_resets != 2) bad = 1;
    if (bad) { std::printf("00665df0_TEST FAIL\n"); return 1; }
    std::printf("00665df0_TEST PASS\n");
    return 0;
}