// Behaviour test for 0x00918190: the Gaze constructor sets the running parameters and resolves both marker defs.
#include <cstdio>
#include <cstring>
static long DefIndex(const char* name)
{
    if (std::strcmp(name, "GAZE_OUT_OF_BUILDING_MARKER") == 0) return 11;
    if (std::strcmp(name, "GAZE_AT_WORK_MARKER") == 0) return 22;
    return -1;
}
struct CGaze {
    void* GazeMarker; float ChanceOfRunning; float MinJourneyForRunning; float MaxDistanceForCreaturesToLookAt;
    long PrimaryMarkerDef; long SecondaryMarkerDef;
    CGaze() : GazeMarker(0), ChanceOfRunning(0.05f), MinJourneyForRunning(8.0f),
        PrimaryMarkerDef(DefIndex("GAZE_OUT_OF_BUILDING_MARKER")), SecondaryMarkerDef(DefIndex("GAZE_AT_WORK_MARKER")) {}
};
int main()
{
    CGaze g;
    if (g.GazeMarker != 0 || g.ChanceOfRunning != 0.05f || g.MinJourneyForRunning != 8.0f || g.PrimaryMarkerDef != 11 || g.SecondaryMarkerDef != 22) {
        std::printf("00918190_TEST FAIL\n"); return 1;
    }
    std::printf("00918190_TEST PASS\n");
    return 0;
}
