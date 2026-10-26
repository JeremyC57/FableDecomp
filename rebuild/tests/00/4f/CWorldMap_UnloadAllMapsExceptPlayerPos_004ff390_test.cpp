#ifndef _MSC_VER
// Host fixture adapts pointer widths; the target build uses the shared header.
#define FABLE_ENGINE_CWORLD_H
class CMessageEventManager;
class CWorld { public: CMessageEventManager* PMessageEventManager_Data; };
#endif
// CWorldMap::UnloadAllMapsExceptPlayerPos() -- unload distant loaded maps with a warning event
// The available PDB CWorldMap header has 16-byte containers and puts Maps at +0x24; retail STLport containers are 12 bytes.
// Nearby landed CWorldMap sources establish retail Maps at +0x20, CMapInfo size 0x48, World at +0x08, and
// the message-event pointer at CWorld +0x60. This local retail model avoids using the conflicting PDB offsets.
class CThing;
class CMessageEventManager { public:
    void* AddMessage(long type, unsigned long id, class C3DVector* pos, CThing* a, CThing* b,
                     float f1, float f2, long map); // 0x00687540
};
#include "engine/CWorld.h"
class C3DVector { public: float X,Y,Z; };
class CMapInfo { public: unsigned char bytes[0x48]; };
struct CMapInfoArray {
    CMapInfo* Start; CMapInfo* Finish; CMapInfo* End;
    long size() const { return (long)(Finish - Start); }
};
struct CWorldMap_Methods {
    virtual ~CWorldMap_Methods();
    virtual void DeleteAllLevels(); virtual void DeleteAllRegions(); virtual void LoadFromFile();
    virtual void SaveToFile(); virtual void RemoveLevel(long map); virtual void LoadTopologyFromFile();
    virtual void PostLoadTopology(long map); virtual void ForceLoad(long map); virtual void ForceUnload(long map);
    virtual bool IsMapLoaded(long map) const;
    virtual bool IsMapUsed(long map) const; virtual long GetGranularity() const;
    virtual const void* GetDimensions() const; virtual void* GetMapAt(const void* pos);
    virtual const void* PeekMapAt(const void* pos) const; virtual long GetMapNumberAt(const void* pos) const;
    virtual long GetMapNumberAt3D(const C3DVector& pos) const; virtual void* GetMap(long map);
    virtual const void* PeekMap(long map) const;
    long f4; CWorld* World; void* Component; void* PThingMapManager;
    unsigned long LoadedScriptedMapBrushes[3]; CMapInfoArray Maps;
    long GetMinPlayerDistanceFromMap(long map); // 0x004FCB30
    void UnloadLevel(long map); // 0x004FEDE0
    void UnloadAllMapsExceptPlayerPos();
};
void CWorldMap_Methods::UnloadAllMapsExceptPlayerPos()
{
    for (long i = 0; i < Maps.size(); ++i) {
        long distance = GetMinPlayerDistanceFromMap(i);
        if (IsMapLoaded(i) && distance > 1) {
            C3DVector pos;
            pos.X = 0.0f; pos.Y = 0.0f; pos.Z = 0.0f;
            CMessageEventManager* manager = World->PMessageEventManager_Data;
            manager->AddMessage(0x25, 0x32, &pos, 0, 0, 0.0f, 0.0f, i);
            UnloadLevel(i);
        }
    }
}
#include <cstdio>
static bool loaded[4]; static long distances[4]; static long unloads[4]; static long n_unloads;
static long n_messages; static long message_maps[4]; static bool bad_message;
void* CMessageEventManager::AddMessage(long type, unsigned long id, C3DVector* pos, CThing* a, CThing* b, float f1, float f2, long map)
{
    if (type != 0x25 || id != 0x32 || !pos || pos->X != 0.0f || pos->Y != 0.0f || pos->Z != 0.0f || a || b || f1 != 0.0f || f2 != 0.0f) bad_message = true;
    message_maps[n_messages++] = map; return 0;
}
CWorldMap_Methods::~CWorldMap_Methods() {}
void CWorldMap_Methods::DeleteAllLevels() {}
void CWorldMap_Methods::DeleteAllRegions() {}
void CWorldMap_Methods::LoadFromFile() {}
void CWorldMap_Methods::SaveToFile() {}
void CWorldMap_Methods::RemoveLevel(long) {}
void CWorldMap_Methods::LoadTopologyFromFile() {}
void CWorldMap_Methods::PostLoadTopology(long) {}
void CWorldMap_Methods::ForceLoad(long) {}
void CWorldMap_Methods::ForceUnload(long) {}
bool CWorldMap_Methods::IsMapLoaded(long i) const { return loaded[i]; }
bool CWorldMap_Methods::IsMapUsed(long) const { return false; }
long CWorldMap_Methods::GetGranularity() const { return 0; }
const void* CWorldMap_Methods::GetDimensions() const { return 0; }
void* CWorldMap_Methods::GetMapAt(const void*) { return 0; }
const void* CWorldMap_Methods::PeekMapAt(const void*) const { return 0; }
long CWorldMap_Methods::GetMapNumberAt(const void*) const { return 0; }
long CWorldMap_Methods::GetMapNumberAt3D(const C3DVector&) const { return 0; }
void* CWorldMap_Methods::GetMap(long) { return 0; }
const void* CWorldMap_Methods::PeekMap(long) const { return 0; }
long CWorldMap_Methods::GetMinPlayerDistanceFromMap(long i) { return distances[i]; }
void CWorldMap_Methods::UnloadLevel(long i) { if (n_messages != n_unloads + 1 || message_maps[n_unloads] != i) bad_message = true; loaded[i] = false; unloads[n_unloads++] = i; }
int main()
{
    CMapInfo infos[4]; CMessageEventManager mgr; CWorld world; CWorldMap_Methods map;
    map.World = &world; map.Maps.Start = infos; map.Maps.Finish = infos + 4; map.Maps.End = infos + 4;
    world.PMessageEventManager_Data = &mgr;
    loaded[0]=true; distances[0]=0; loaded[1]=true; distances[1]=1; loaded[2]=false; distances[2]=8; loaded[3]=true; distances[3]=2;
    map.UnloadAllMapsExceptPlayerPos();
    if (n_messages != 1 || n_unloads != 1 || message_maps[0] != 3 || unloads[0] != 3 || bad_message || loaded[0] != true || loaded[1] != true || loaded[2] != false || loaded[3] != false) { std::printf("004ff390_TEST FAIL\n"); return 1; }
    map.Maps.Finish = infos;
    map.UnloadAllMapsExceptPlayerPos();
    if (n_messages != 1 || n_unloads != 1 || bad_message) { std::printf("004ff390_TEST FAIL\n"); return 1; }
    std::printf("004ff390_TEST PASS\n"); return 0;
}