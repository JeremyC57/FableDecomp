// Behaviour test for 0x006B08F0 CTCPhysicsBase::OnSerialise: a copy of the function's logic over a small stand-alone model
// (persist context with the five modes, a world map whose maps are 100-unit strips with a region per pair of maps, a world with a
// save-game marker, a parent thing with type flags and a map number), checked in save mode and in load mode with and without the
// marker move.
#include <cstdio>
class C2DCoordI { public: long X, Y; };
class C3DVector { public: float X, Y, Z;
    bool IsNonZero() const { return X*X+Y*Y+Z*Z > 0.0001f*0.0001f; } };
class CPersistContext { public:
    enum EMode { MODE_LOAD_TEXT, MODE_SAVE_TEXT, MODE_LOAD_BINARY, MODE_SAVE_BINARY, MODE_RESET };
    EMode Mode; float Stored[3]; int Index;
    bool IsSaving() const { return Mode == MODE_SAVE_TEXT || Mode == MODE_SAVE_BINARY; }
    void Transfer(const char*, float& value) { if (IsSaving()) Stored[Index++] = value; else if (Mode == MODE_RESET) value = 0; else value = Stored[Index++]; }
};
class CWorldMap { public:
    long GetMapNumberAt(const C3DVector& p) const { return (long)(p.X / 100.0f); }
    C2DCoordI GetMapPos(long map) const { C2DCoordI c; c.X = map * 100; c.Y = 5; return c; }
    long GetRegionNumberMapIsIn(long map) const { return map / 2; }
};
class CWorld { public: C3DVector Marker; const C3DVector& GetSaveGameMarkerPos() const { return Marker; } };
class CThing { public: unsigned long ThingTypeFlags; short CurrentMapNumber;
    unsigned long GetTypeFlags() const { return ThingTypeFlags; }
    long GetCurrentMapNumber() const { return CurrentMapNumber; }
    void SetCurrentMapNumber(long n) { CurrentMapNumber = (short)n; } };
class CTCPhysicsBase { public:
    CThing* ParentThing; CWorldMap* Map; CWorld* World; long Version; C3DVector Position;
    const CWorldMap& PeekWorldMap() const { return *Map; }
    const CWorld& PeekWorld() const { return *World; }
    long GetSerialiseVersion() const { return Version; }
    const CThing& GetParentThing() const { return *ParentThing; }
    CThing& GetParentThing() { return *ParentThing; }
    void OnSerialise(CPersistContext& context, bool)
    {
        C3DVector pos;
        if (context.IsSaving()) {
            pos = Position;
            C2DCoordI origin = PeekWorldMap().GetMapPos(PeekWorldMap().GetMapNumberAt(pos));
            pos.X -= origin.X;
            pos.Y -= origin.Y;
        }
        context.Transfer("PositionX", pos.X);
        context.Transfer("PositionY", pos.Y);
        context.Transfer("PositionZ", pos.Z);
        if (!context.IsSaving()) {
            Position = pos;
            if (GetSerialiseVersion() > 0) {
                C2DCoordI origin = PeekWorldMap().GetMapPos(ParentThing->GetCurrentMapNumber());
                Position.X += origin.X;
                Position.Y += origin.Y;
            }
            if (ParentThing->GetTypeFlags() & 0x20) {
                if (PeekWorld().GetSaveGameMarkerPos().IsNonZero()) {
                    C3DVector marker = PeekWorld().GetSaveGameMarkerPos();
                    long markerRegion = PeekWorldMap().GetRegionNumberMapIsIn(PeekWorldMap().GetMapNumberAt(marker));
                    const CThing& thing = GetParentThing();
                    if (markerRegion == PeekWorldMap().GetRegionNumberMapIsIn(thing.GetCurrentMapNumber())) {
                        Position = marker;
                        GetParentThing().SetCurrentMapNumber(PeekWorldMap().GetMapNumberAt(Position));
                    }
                }
            }
        }
    }
};
static int fails = 0;
static void check(bool c, const char* what) { if (!c) { std::printf("fail: %s\n", what); ++fails; } }
int main()
{
    CWorldMap map; CWorld world; CThing thing; CTCPhysicsBase tc;
    tc.ParentThing = &thing; tc.Map = &map; tc.World = &world; tc.Version = 1;
    thing.ThingTypeFlags = 0x20; thing.CurrentMapNumber = 2;
    world.Marker.X = 0; world.Marker.Y = 0; world.Marker.Z = 0;
    // save: position relative to the origin of map 2 (200, 5)
    tc.Position.X = 250; tc.Position.Y = 15; tc.Position.Z = 3;
    CPersistContext ctx; ctx.Mode = CPersistContext::MODE_SAVE_BINARY; ctx.Index = 0;
    tc.OnSerialise(ctx, false);
    check(ctx.Stored[0] == 50 && ctx.Stored[1] == 10 && ctx.Stored[2] == 3, "save relative");
    check(tc.Position.X == 250, "save leaves position");
    // load, version 1, zero marker: origin of the thing's map re-added
    tc.Position.X = tc.Position.Y = tc.Position.Z = 0;
    ctx.Mode = CPersistContext::MODE_LOAD_BINARY; ctx.Index = 0;
    tc.OnSerialise(ctx, false);
    check(tc.Position.X == 250 && tc.Position.Y == 15 && tc.Position.Z == 3, "load absolute");
    // load, version 0: stays relative
    tc.Version = 0; ctx.Index = 0;
    tc.OnSerialise(ctx, false);
    check(tc.Position.X == 50 && tc.Position.Y == 10, "load version 0");
    // marker in the same region (map 3 -> region 1, thing map 2 -> region 1): moved, map number updated
    tc.Version = 1; ctx.Index = 0; world.Marker.X = 340; world.Marker.Y = 7; world.Marker.Z = 1;
    tc.OnSerialise(ctx, false);
    check(tc.Position.X == 340 && tc.Position.Y == 7 && tc.Position.Z == 1 && thing.CurrentMapNumber == 3, "marker move");
    // marker in another region (map 5 -> region 2, thing map 3 -> region 1): not moved
    ctx.Index = 0; world.Marker.X = 520;
    tc.OnSerialise(ctx, false);
    check(tc.Position.X == 350 && tc.Position.Y == 15 && thing.CurrentMapNumber == 3, "other region");
    // flag 0x20 clear: never moved
    thing.ThingTypeFlags = 0x10; ctx.Index = 0; world.Marker.X = 330;
    tc.OnSerialise(ctx, false);
    check(tc.Position.X == 350, "flag clear");
    if (fails) { std::printf("006b08f0_TEST FAIL\n"); return 1; }
    std::printf("006b08f0_TEST PASS\n");
    return 0;
}