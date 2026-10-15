#include <cstdio>
// behaviour test for CThingBodyReorienter::UpdateBodyReorienation (0x00863E10)
long g_BodyReorientSlopePeriod = 3;
static int g_fill = 0, g_check = 0, g_gradual = 0, g_slopes = 0;
static bool g_checkFinishes = false;
struct CThingBodyReorienter {
    bool BodyReorientationDone;
    bool ReorientingBody;
    long ReorientationCount;
    bool StillCheckingForNewBodyReorientationAngle;
    bool HaveFinalReorientationAngle;
    float FinalReorientationAngleXY;
    long RotationDirection;
    bool HasPossibleIntersectingThings;
    void FillBodyReorientationPossibleIntersecting() { ++g_fill; HasPossibleIntersectingThings = true; }
    void CheckForNewBodyReorientationAngle() { ++g_check; if (g_checkFinishes) BodyReorientationDone = true; }
    void GradualReorientBody() { ++g_gradual; }
    void ReorientBodyYZForSlopes() { ++g_slopes; }
    void UpdateBodyReorienation();
};
void CThingBodyReorienter::UpdateBodyReorienation()
{
    if (BodyReorientationDone)
        return;
    if (!HasPossibleIntersectingThings)
        FillBodyReorientationPossibleIntersecting();
    if (StillCheckingForNewBodyReorientationAngle)
        CheckForNewBodyReorientationAngle();
    if (!BodyReorientationDone)
        GradualReorientBody();
    ++ReorientationCount;
    if (StillCheckingForNewBodyReorientationAngle && ReorientationCount % g_BodyReorientSlopePeriod == 0)
        ReorientBodyYZForSlopes();
}
int main()
{
    CThingBodyReorienter r;
    r.BodyReorientationDone = true; r.ReorientationCount = 0; r.StillCheckingForNewBodyReorientationAngle = true; r.HasPossibleIntersectingThings = false;
    r.UpdateBodyReorienation();
    if (g_fill || g_check || g_gradual || r.ReorientationCount != 0) { std::printf("00863e10_TEST FAIL\n"); return 1; }
    r.BodyReorientationDone = false;
    r.UpdateBodyReorienation(); r.UpdateBodyReorienation(); r.UpdateBodyReorienation();
    if (g_fill != 1 || g_check != 3 || g_gradual != 3 || r.ReorientationCount != 3 || g_slopes != 1) { std::printf("00863e10_TEST FAIL\n"); return 1; }
    g_checkFinishes = true;
    r.UpdateBodyReorienation();
    if (g_check != 4 || g_gradual != 3 || r.ReorientationCount != 4 || g_slopes != 1) { std::printf("00863e10_TEST FAIL\n"); return 1; }
    std::printf("00863e10_TEST PASS\n");
    return 0;
}