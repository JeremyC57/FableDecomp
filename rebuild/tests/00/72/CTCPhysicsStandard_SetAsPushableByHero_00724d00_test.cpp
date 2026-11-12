#include <cstdio>
struct CTCPhysicsStandard {
    bool CollidesWithThingsOneWay : 1;
    bool CollidesWithLandscape : 1;
    bool PhysicsEnabled : 1;
    bool Immoveable : 1;
    bool NavigationLayers : 1;
    bool PushableByHero : 1;
    bool ExcludingCreatures : 1;
    bool Impassable : 1;
    void vfunc_84(bool value) { PushableByHero = value; }
};
int main()
{
    CTCPhysicsStandard p;
    p.CollidesWithThingsOneWay = true; p.CollidesWithLandscape = false; p.PhysicsEnabled = true; p.Immoveable = false;
    p.NavigationLayers = true; p.PushableByHero = false; p.ExcludingCreatures = true; p.Impassable = false;
    p.vfunc_84(true);
    if (!p.PushableByHero || !p.CollidesWithThingsOneWay || p.CollidesWithLandscape || !p.PhysicsEnabled || p.Immoveable ||
        !p.NavigationLayers || !p.ExcludingCreatures || p.Impassable) { std::printf("00724d00_TEST FAIL\n"); return 1; }
    p.vfunc_84(false);
    if (p.PushableByHero || !p.ExcludingCreatures || !p.NavigationLayers) { std::printf("00724d00_TEST FAIL\n"); return 1; }
    std::printf("00724d00_TEST PASS\n");
    return 0;
}