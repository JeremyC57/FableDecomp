// CMap::ScriptedMapBrushMoveThings(const CScriptedMapBrush&) -- behaviour test: live things listed by the brush get the brush
// position (converted to world-map coordinates for the map's number) and the three facing angles; null and dead things are skipped.
#include <stdio.h>
#include <list>

struct Vec { float X, Y, Z; };
struct Angles { float AngleXY, AngleYZ, AngleXZ; };
struct PosAndOrientation { Vec Position; Angles FacingAngles; };

struct Physics {
    Vec Pos; float XY, YZ, XZ; int Calls;
    Physics() : XY(0), YZ(0), XZ(0), Calls(0) { Pos.X = Pos.Y = Pos.Z = 0; }
    virtual void ResetPosition(const Vec& p) { Pos = p; ++Calls; }
    virtual void SetFacingAngleXY(float a) { XY = a; ++Calls; }
    virtual void SetFacingAngleYZ(float a) { YZ = a; ++Calls; }
    virtual void SetFacingAngleXZ(float a) { XZ = a; ++Calls; }
    virtual ~Physics() { }
};
struct Thing { Physics* PhysicsTC; bool Dead; };
struct Brush {
    PosAndOrientation Moves[4];
    void GetThingsToMove(std::list<unsigned long>& out) const { out.push_back(0); out.push_back(1); out.push_back(2); out.push_back(3); }
    const PosAndOrientation& GetThingsMovement(unsigned long uid) const { return Moves[uid]; }
};
struct ThingManager {
    Thing* Things[4];
    Thing* GetPThingWithUID(unsigned long uid) { return Things[uid]; }
};
struct WorldMap {
    long MapNumberSeen;
    long GetMapNumberAt(long x, long y) const { return x * 10 + y; }
    Vec ConvertLocalCoordsToWorldMapCoords(long map, const Vec& p) { MapNumberSeen = map; Vec r; r.X = p.X + 100.0f; r.Y = p.Y + 200.0f; r.Z = p.Z; return r; }
};
struct World { ThingManager Manager; WorldMap Map; };
struct Map {
    World* PWorld; long WorldPosX, WorldPosY;
    void ScriptedMapBrushMoveThings(const Brush& brush)
    {
        std::list<unsigned long> things;
        brush.GetThingsToMove(things);
        long mapNumber = PWorld->Map.GetMapNumberAt(WorldPosX, WorldPosY);
        for (std::list<unsigned long>::iterator it = things.begin(); it != things.end(); ++it) {
            Thing* thing = PWorld->Manager.GetPThingWithUID(*it);
            if (thing && !thing->Dead) {
                PosAndOrientation movement = brush.GetThingsMovement(*it);
                Physics* physics = thing->PhysicsTC;
                physics->ResetPosition(PWorld->Map.ConvertLocalCoordsToWorldMapCoords(mapNumber, movement.Position));
                thing->PhysicsTC->SetFacingAngleXY(movement.FacingAngles.AngleXY);
                thing->PhysicsTC->SetFacingAngleYZ(movement.FacingAngles.AngleYZ);
                thing->PhysicsTC->SetFacingAngleXZ(movement.FacingAngles.AngleXZ);
            }
        }
    }
};

int main()
{
    Physics p0, p2, p3;
    Thing t0 = { &p0, false }, t2 = { &p2, true }, t3 = { &p3, false };
    World w;
    w.Manager.Things[0] = &t0; w.Manager.Things[1] = 0; w.Manager.Things[2] = &t2; w.Manager.Things[3] = &t3;
    w.Map.MapNumberSeen = -1;
    Brush b;
    for (int i = 0; i < 4; ++i) {
        b.Moves[i].Position.X = (float)i; b.Moves[i].Position.Y = 2.0f * i; b.Moves[i].Position.Z = 3.0f * i;
        b.Moves[i].FacingAngles.AngleXY = 0.5f * i; b.Moves[i].FacingAngles.AngleYZ = 0.25f * i; b.Moves[i].FacingAngles.AngleXZ = 0.125f * i;
    }
    Map m; m.PWorld = &w; m.WorldPosX = 3; m.WorldPosY = 4;
    m.ScriptedMapBrushMoveThings(b);
    bool ok = p0.Calls == 4 && p0.Pos.X == 100.0f && p0.Pos.Y == 200.0f && p0.Pos.Z == 0.0f
        && p2.Calls == 0
        && p3.Calls == 4 && p3.Pos.X == 103.0f && p3.Pos.Y == 206.0f && p3.Pos.Z == 9.0f
        && p3.XY == 1.5f && p3.YZ == 0.75f && p3.XZ == 0.375f
        && w.Map.MapNumberSeen == 34;
    printf(ok ? "00820840_TEST PASS\n" : "00820840_TEST FAIL\n");
    return ok ? 0 : 1;
}