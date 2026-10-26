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