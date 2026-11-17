// Behaviour test for NCameraModes::CStringAndCageBase::UpdateXYVars (0x00871FD0): the current string length becomes the rolling
// average (current * 3 + desired) / 4; the cage radius eases toward the desired cage radius, or toward the desired string length
// while TimerForExpansion > 0, which then counts down by one (a zero or negative timer is left alone).
#include <cstdio>
struct Cam {
    float DesiredStringLength, DesiredCageRadius, CurrentStringLength, CurrentCageRadius; long TimerForExpansion; int tools_calls;
    float Roll(const float& cur, const float& nv, float w) { ++tools_calls; return ((cur * w + nv) / (w + 1.0f)); }
    void UpdateXYVars()
    {
        CurrentStringLength = Roll(CurrentStringLength, DesiredStringLength, 3.0f);
        float new_cage_radius = DesiredCageRadius;
        if (TimerForExpansion > 0) {
            new_cage_radius = DesiredStringLength;
            --TimerForExpansion;
        }
        CurrentCageRadius = Roll(CurrentCageRadius, new_cage_radius, 3.0f);
    }
};
static int fail(int n) { std::printf("00871fd0_TEST FAIL %d\n", n); return 1; }
int main()
{
    Cam c = { 8.0f, 4.0f, 0.0f, 0.0f, 2, 0 };
    c.UpdateXYVars();                                   // timer running: cage toward the string length 8
    if (c.CurrentStringLength != 2.0f || c.CurrentCageRadius != 2.0f || c.TimerForExpansion != 1 || c.tools_calls != 2) return fail(1);
    c.UpdateXYVars();                                   // (2*3+8)/4 = 3.5 for both
    if (c.CurrentStringLength != 3.5f || c.CurrentCageRadius != 3.5f || c.TimerForExpansion != 0) return fail(2);
    c.UpdateXYVars();                                   // timer zero: cage toward 4 -> (3.5*3+4)/4 = 3.625, timer stays 0
    if (c.CurrentStringLength != 4.625f || c.CurrentCageRadius != 3.625f || c.TimerForExpansion != 0) return fail(3);
    c.TimerForExpansion = -3; c.CurrentCageRadius = 0.0f;
    c.UpdateXYVars();                                   // negative timer: left alone, cage toward 4 -> 1
    if (c.CurrentCageRadius != 1.0f || c.TimerForExpansion != -3) return fail(4);
    std::printf("00871fd0_TEST PASS\n");
    return 0;
}
