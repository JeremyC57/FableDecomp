// Behaviour test for CGameScriptInterface::GetAllThingsInLevel (0x008A8AF0): resolve the level's map number, force that map
// loaded, collect the things in that map and append the script thing of every living thing whose script handle is not
// null; return the output vector's size (things already in it count).
#include <cstdio>
#include <cstring>
#include <cstdlib>

struct CScriptImp { bool Null; bool IsNull() const { return Null; } };
class CScriptThing { public:
    CScriptImp* Data;
    long        UID;
    CScriptThing() : Data(0), UID(0) {}
    bool IsNull() const { return !Data || Data->IsNull(); }
};
struct CScriptThingVector {
    CScriptThing Items[16];
    unsigned int Size;
    CScriptThingVector() : Size(0) {}
    unsigned int size() const { return Size; }
    void push_back(const CScriptThing& x) { Items[Size++] = x; }
};
class CThing { public:
    long        MapNo;
    bool        Dead;
    CScriptImp* Imp;
    long        UID;
    bool IsAlive() const { return Dead ? false : true; }
    CScriptThing GetScriptThing() const { CScriptThing s; s.Data = Imp; s.UID = UID; return s; }
};
class CWorldMap { public:
    long LastForced;
    CWorldMap() : LastForced(-1) {}
    long GetMapNumberFromScriptName(const char* name) const { return std::strcmp(name, "Oakvale") == 0 ? 3 : 7; }
    void ForceLoad(long mapNumber) { LastForced = mapNumber; }
};
class CThingFilter_InMap { public:
    const CWorldMap* WorldMap;
    long             MapNo;
    CThingFilter_InMap(const CWorldMap& worldMap, long mapNo) : WorldMap(&worldMap), MapNo(mapNo) {}
    bool operator()(const CThing& t) const { return t.MapNo == MapNo; }
};
struct CThingPtrVector {
    CThing** Start;
    CThing** Finish;
    CThingPtrVector() : Start(0), Finish(0) {}
    ~CThingPtrVector() { if (Start) free(Start); }
};
class CWorld { public:
    CWorldMap Map;
    CThing*   Things;
    int       Count;
    CWorldMap& GetWorldMap() { return Map; }
    void GetAllThings(const CThingFilter_InMap& filter, CThingPtrVector& out) const
    {
        out.Start = out.Finish = (CThing**)malloc(sizeof(CThing*) * (Count + 1));
        for (int i = 0; i < Count; ++i)
            if (filter(Things[i])) *out.Finish++ = &Things[i];
    }
};
// The function's logic, as in the source.
long GetAllThingsInLevel(CWorld* World, const char* levelName, CScriptThingVector& things)
{
    long mapNumber = World->GetWorldMap().GetMapNumberFromScriptName(levelName);
    World->GetWorldMap().ForceLoad(mapNumber);
    CThingPtrVector found;
    World->GetAllThings(CThingFilter_InMap(World->GetWorldMap(), mapNumber), found);
    for (CThing** it = found.Start; it != found.Finish; ++it) {
        CThing* thing = *it;
        if (thing->IsAlive()) {
            CScriptThing scriptThing = thing->GetScriptThing();
            if (!scriptThing.IsNull())
                things.push_back(scriptThing);
        }
    }
    return things.size();
}

int main()
{
    CScriptImp live = { false }, nulled = { true };
    CThing things[6] = {
        { 3, false, &live, 1 },    // in map, alive, valid      -> kept
        { 3, true,  &live, 2 },    // dead                      -> skipped
        { 7, false, &live, 3 },    // other map                 -> skipped
        { 3, false, 0,     4 },    // null handle               -> skipped
        { 3, false, &nulled, 5 },  // imp reports null          -> skipped
        { 3, false, &live, 6 },    // kept
    };
    CWorld world;
    world.Things = things;
    world.Count = 6;
    CScriptThingVector out;
    out.push_back(CScriptThing());                       // one already in the vector
    long n = GetAllThingsInLevel(&world, "Oakvale", out);
    bool ok = n == 3 && out.Size == 3 && out.Items[1].UID == 1 && out.Items[2].UID == 6 && world.Map.LastForced == 3;
    CScriptThingVector out2;
    long n2 = GetAllThingsInLevel(&world, "Elsewhere", out2);
    ok = ok && n2 == 1 && out2.Items[0].UID == 3 && world.Map.LastForced == 7;
    if (!ok) { std::printf("008a8af0_TEST FAIL\n"); return 1; }
    std::printf("008a8af0_TEST PASS\n");
    return 0;
}