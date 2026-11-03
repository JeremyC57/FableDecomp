// Behaviour test for 0x00A1E030 CNavigatorAStar::UpdateGoal(const C3DVector& position, C2DVector oldGoal, const C2DVector& newGoal)
#include <stdio.h>
#include <math.h>
struct V2 { float X, Y; };
struct Agent { float Radius, BestDistLeft, BestDistRight; V2 Goal, NextGoal; };
static int g_recalcs;
static void Update(Agent& a, const V2& oldGoal, const V2& newGoal, const V2* route, int routeSize)   // the row's body
{
    float dx = newGoal.X - oldGoal.X, dy = newGoal.Y - oldGoal.Y;
    float distance = (float)sqrt(dx * dx + dy * dy);
    float minWidth = a.Radius * 5.0f;
    if (a.BestDistLeft - distance < minWidth || a.BestDistRight - distance < minWidth)
        ++g_recalcs;                       // CalcWayPointWidths(position)
    else { a.BestDistLeft -= distance; a.BestDistRight -= distance; }
    a.Goal = route[0];
    a.NextGoal = routeSize > 1 ? route[1] : route[0];
}
int main()
{
    Agent a = { 1.0f, 20.0f, 30.0f };
    V2 route[2] = { { 1.0f, 2.0f }, { 3.0f, 4.0f } };
    V2 o = { 0.0f, 0.0f }, n = { 3.0f, 4.0f };
    int ok = 1;
    Update(a, o, n, route, 2);
    ok &= g_recalcs == 0 && a.BestDistLeft == 15.0f && a.BestDistRight == 25.0f && a.NextGoal.X == 3.0f;
    Update(a, o, n, route, 1);            // 15 - 5 = 10 >= 5: still shrinks
    ok &= g_recalcs == 0 && a.BestDistLeft == 10.0f && a.NextGoal.X == 1.0f;
    Update(a, o, n, route, 1);            // 10 - 5 = 5, not < 5
    Update(a, o, n, route, 1);            // 5 - 5 = 0 < 5: recalculates, widths kept
    ok &= g_recalcs == 1 && a.BestDistLeft == 5.0f && a.Goal.Y == 2.0f;
    printf(ok ? "00a1e030_TEST PASS\n" : "00a1e030_TEST FAIL\n");
    return ok ? 0 : 1;
}
