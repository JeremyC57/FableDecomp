#include <cstdio>
struct CAIStateGroup_Gaze { virtual void v00() {} unsigned char _pad_0x04[0x10]; unsigned char GazeMarker[8]; float ChanceOfRunning; float MinJourneyForRunning; float MaxDistanceForCreaturesToLookAt; long PrimaryMarkerDef; long SecondaryMarkerDef;
    CAIStateGroup_Gaze() : ChanceOfRunning(1.0f), MinJourneyForRunning(0.0f), MaxDistanceForCreaturesToLookAt(99.0f) {} };
struct CAIStateGroup_GazeAtHome : CAIStateGroup_Gaze { virtual void v00() {} CAIStateGroup_GazeAtHome() : CAIStateGroup_Gaze() { ChanceOfRunning = 0.05f; MinJourneyForRunning = 10.0f; } };

int main()
{
    CAIStateGroup_GazeAtHome g;
    if (g.ChanceOfRunning != 0.05f || g.MinJourneyForRunning != 10.0f || g.MaxDistanceForCreaturesToLookAt != 99.0f) { std::printf("009182b0_TEST FAIL\n"); return 1; }
    if ((unsigned char*)&g.ChanceOfRunning - (unsigned char*)&g != 0x1c) { std::printf("009182b0_TEST FAIL\n"); return 1; }
    std::printf("009182b0_TEST PASS\n");
    return 0;
}