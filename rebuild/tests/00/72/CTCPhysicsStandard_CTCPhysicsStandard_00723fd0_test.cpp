#include <cstdio>
#include <cstring>

struct CThing { int id; };
struct CThingPhysicsMeshInfo;
struct CTCPhysicsBase { CThing* ParentThing; CTCPhysicsBase(CThing& o) : ParentThing(&o) { } };
struct CTCPhysicsStandard : CTCPhysicsBase {
    CThingPhysicsMeshInfo* PPhysicsMeshInfo; long RHSetLastFrameSet;
    bool AlwaysFacingMovementDirection : 1; bool AngledWithGround : 1; bool FeelsGravity : 1; bool FeelsAirResistance : 1;
    bool OnGround : 1; bool OnMesh : 1; bool InteractsWithThings : 1; bool CollidesWithThings : 1;
    bool CollidesWithThingsOneWay : 1; bool CollidesWithLandscape : 1; bool PhysicsEnabled : 1; bool Immoveable : 1;
    bool NavigationLayers : 1; bool PushableByHero : 1; bool ExcludingCreatures : 1; bool Impassable : 1;
    CTCPhysicsStandard(CThing& owner)
        : CTCPhysicsBase(owner), PPhysicsMeshInfo(0), RHSetLastFrameSet(0),
          AlwaysFacingMovementDirection(false), AngledWithGround(false), FeelsGravity(true), FeelsAirResistance(false),
          OnGround(false), OnMesh(false), InteractsWithThings(false), CollidesWithThings(false),
          CollidesWithThingsOneWay(false), CollidesWithLandscape(false), PhysicsEnabled(true), Immoveable(false),
          NavigationLayers(false), PushableByHero(false), ExcludingCreatures(false), Impassable(false) { }
};
int main()
{
    CThing t; t.id = 1;
    CTCPhysicsStandard p(t);
    if (p.ParentThing != &t || p.PPhysicsMeshInfo != 0 || p.RHSetLastFrameSet != 0 || !p.FeelsGravity || !p.PhysicsEnabled) { std::printf("00723fd0_TEST FAIL\n"); return 1; }
    if (p.AlwaysFacingMovementDirection || p.AngledWithGround || p.FeelsAirResistance || p.OnGround || p.OnMesh || p.InteractsWithThings || p.CollidesWithThings) { std::printf("00723fd0_TEST FAIL\n"); return 1; }
    if (p.CollidesWithThingsOneWay || p.CollidesWithLandscape || p.Immoveable || p.NavigationLayers || p.PushableByHero || p.ExcludingCreatures || p.Impassable) { std::printf("00723fd0_TEST FAIL\n"); return 1; }
    std::printf("00723fd0_TEST PASS\n");
    return 0;
}