// behaviour test for 0x0049ca20 CWorld::SetStartTime(double): stores the double; HasInitialisedStartTime and
// TimePlayed are untouched
#include <cstdio>
struct World { double StartTime; double TimePlayed; bool HasInitialisedStartTime; void SetStartTime(double t) { StartTime = t; } };
int main()
{
    int fails = 0;
    World w; w.StartTime = 0.0; w.TimePlayed = 7.5; w.HasInitialisedStartTime = false;
    w.SetStartTime(1234.5);
    if (w.StartTime != 1234.5 || w.TimePlayed != 7.5 || w.HasInitialisedStartTime) fails |= 1;
    if (fails) { std::printf("0049ca20_TEST FAIL %d\n", fails); return 1; }
    std::printf("0049ca20_TEST PASS\n");
    return 0;
}