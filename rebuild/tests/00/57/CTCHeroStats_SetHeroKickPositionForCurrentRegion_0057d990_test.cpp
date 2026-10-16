#include <cstdio>
// Behaviour model of CTCHeroStats::SetHeroKickPositionForCurrentRegion (0x0057D990): the current region's mirrored
// criminal status records the exit position, angle, teleporter flag and the world frame.
struct Vec { float X, Y, Z; };
struct Status { float EntrancePosX, EntrancePosY, EntrancePosZ, EntranceAngle; bool ExitedUsingTeleporter; long LastTimeExited; };
struct Stats {
    Status regions[3]; int currentRegion; long frame;
    void Set(const Vec& p, float angle, bool tele)
    {
        Status& s = regions[currentRegion];
        s.EntrancePosX = p.X; s.EntrancePosY = p.Y; s.EntrancePosZ = p.Z;
        s.EntranceAngle = angle; s.ExitedUsingTeleporter = tele; s.LastTimeExited = frame;
    }
};
int main()
{
    Stats st = {};
    st.currentRegion = 2; st.frame = 77;
    Vec v = { 1.0f, 2.0f, 3.0f };
    st.Set(v, 0.5f, true);
    const Status& s = st.regions[2];
    if (s.EntrancePosX != 1.0f || s.EntrancePosY != 2.0f || s.EntrancePosZ != 3.0f || s.EntranceAngle != 0.5f
        || !s.ExitedUsingTeleporter || s.LastTimeExited != 77 || st.regions[0].LastTimeExited != 0) { std::printf("0057d990_TEST FAIL\n"); return 1; }
    std::printf("0057d990_TEST PASS\n");
    return 0;
}