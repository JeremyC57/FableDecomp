// Behaviour test for CTCPhysicsStandard::SetInteractionFlags (0x007234C0): each flag-word bit lands in its PDB bool:1 flag, the
// kept flags are untouched, and only an Immoveable 1 -> 0 transition calls SetPhysicsEnabled(true).
#include <cstdio>
static int g_enableCalls = 0; static int g_enableArg = -1;
struct CTCPhysicsStandard {
    virtual void SetPhysicsEnabled(bool enabled) { ++g_enableCalls; g_enableArg = enabled ? 1 : 0; }
    bool AlwaysFacingMovementDirection : 1;
    bool AngledWithGround : 1;
    bool FeelsGravity : 1;
    bool FeelsAirResistance : 1;
    bool OnGround : 1;
    bool OnMesh : 1;
    bool InteractsWithThings : 1;
    bool CollidesWithThings : 1;
    bool CollidesWithThingsOneWay : 1;
    bool CollidesWithLandscape : 1;
    bool PhysicsEnabled : 1;
    bool Immoveable : 1;
    bool NavigationLayers : 1;
    bool PushableByHero : 1;
    bool ExcludingCreatures : 1;
    bool Impassable : 1;
    void SetInteractionFlags(unsigned long flags);
};

void CTCPhysicsStandard::SetInteractionFlags(unsigned long flags)
{
    AlwaysFacingMovementDirection = (flags >> 6) & 1;
    AngledWithGround = (flags >> 8) & 1;
    InteractsWithThings = (flags >> 4) & 1;
    CollidesWithThings = (flags >> 2) & 1;
    CollidesWithThingsOneWay = (flags >> 3) & 1;
    CollidesWithLandscape = (flags >> 1) & 1;
    ExcludingCreatures = (flags >> 10) & 1;
    Impassable = (flags >> 11) & 1;
    bool wasImmoveable = Immoveable;
    Immoveable = (flags >> 5) & 1;
    if (wasImmoveable && !Immoveable)
        SetPhysicsEnabled(true);
}

int main()
{
    int bad = 0;
    CTCPhysicsStandard p;
    p.AlwaysFacingMovementDirection = p.AngledWithGround = p.InteractsWithThings = p.CollidesWithThings = false;
    p.CollidesWithThingsOneWay = p.CollidesWithLandscape = p.ExcludingCreatures = p.Impassable = false;
    p.FeelsGravity = true; p.FeelsAirResistance = false; p.OnGround = true; p.OnMesh = false;
    p.PhysicsEnabled = true; p.NavigationLayers = true; p.PushableByHero = false; p.Immoveable = false;
    p.SetInteractionFlags(0xFFFu);
    if (!p.AlwaysFacingMovementDirection || !p.AngledWithGround || !p.InteractsWithThings || !p.CollidesWithThings) bad |= 1;
    if (!p.CollidesWithThingsOneWay || !p.CollidesWithLandscape || !p.ExcludingCreatures || !p.Impassable || !p.Immoveable) bad |= 2;
    if (!p.FeelsGravity || p.FeelsAirResistance || !p.OnGround || p.OnMesh || !p.PhysicsEnabled || !p.NavigationLayers) bad |= 4;
    if (g_enableCalls != 0) bad |= 8;                  // 0 -> 1 does not call
    p.SetInteractionFlags(0x0004u);                    // only CollidesWithThings; Immoveable 1 -> 0
    if (p.AlwaysFacingMovementDirection || p.AngledWithGround || p.InteractsWithThings || !p.CollidesWithThings) bad |= 16;
    if (p.CollidesWithThingsOneWay || p.CollidesWithLandscape || p.ExcludingCreatures || p.Impassable || p.Immoveable) bad |= 32;
    if (g_enableCalls != 1 || g_enableArg != 1) bad |= 64;
    p.SetInteractionFlags(0x0040u | 0x0100u);          // facing + angled; Immoveable stays 0
    if (!p.AlwaysFacingMovementDirection || !p.AngledWithGround || p.CollidesWithThings || g_enableCalls != 1) bad |= 128;
    p.SetInteractionFlags(0x0020u); p.SetInteractionFlags(0x0020u);   // stays immoveable: no call
    if (!p.Immoveable || g_enableCalls != 1) bad |= 256;
    if (bad) { std::printf("007234c0_TEST FAIL\n"); return 1; }
    std::printf("007234c0_TEST PASS\n");
    return 0;
}
