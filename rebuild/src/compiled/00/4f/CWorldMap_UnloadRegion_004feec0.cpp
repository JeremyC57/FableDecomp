// CWorldMap::UnloadRegion(long region, bool b) -- unloads every map of a region: tells the quest manager and the thing manager, then for each loaded map clears its active flag, removes it from the engine and the loaded-map list and releases the map; then drops each map from the thing-map manager and the navigator, clears the loaded region and posts the region-unloaded message
// Leaf: ?UnloadRegion@CWorldMap@@IAEXJ_N@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FEEC0, 444 bytes (+ int3 padding), `ret 8`. PDB ?UnloadRegion@CWorldMap@@IAEXJ_N@Z.
// Callees: the quest manager (global 0x013B89FC) OnRegionUnloaded 0x004AFC30 (const CCharString&), CWorld virtual slot 3
//   GetThingManager, CThingManager::OnLevelsUnload(const vector<long>&, bool) 0x00525CD0, CWorldMap virtual slot 10
//   IsMapLoaded, CTBaseSingleton<CEngineManager>::Get 0x00436FB0 (`__fastcall`), CEngineManager::RemoveMap 0x00640430,
//   __find 0x0050E540 (`__fastcall`, landed; same-TU stand-in as in the landed UnloadLevel 0x004FEDE0), memmove
//   0x00BFEAE6, operator delete 0x00BFE9BC, CThingMapManager::RemoveGameMap 0x00638340, CNavigatorManager::
//   IsNavigationMapLoaded 0x00A14E80 / RemoveNavigationMap 0x00A155D0, CMessageEventManager::AddMessage 0x00687540.
// Layout: the retail CWorldMap model of the landed UnloadLevel 0x004FEDE0 (Maps +0x20, Regions +0x2C, LoadedMaps
//   +0x90, LoadedRegion +0x9C, RegionUnloading +0xD4; the PDB header rebuild/include/engine/CWorldMap.h has 16-byte STL
//   containers, so these sit 4..0x24 bytes higher there). CRegion is 0x58 bytes: Maps CArray<long> +0, Name +0x18.
// Fix (chunk 762): the region's map list is bound to a named reference (`CArray<long>& maps = Regions[region].Maps`) and
//   the name is read through Regions[region] again; VC7.1 then keeps the region address in a fresh register (`lea ebx,
//   [eax+ecx]`, the index scaled in eax ahead of the pushes) and the loop maps in ebp, as retail does (the `CRegion& r`
//   form coalesced the scale into ebp). Callee spelling: CQuestManager::OnRegionUnloaded 0x004AFC30 (landed).
#include <string.h>

struct CCharString { void* PStringData; };
struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

struct CCPPointerInfo {
    long  Count;                                             // +0
    void (__fastcall* Destroy)(void* data);                  // +4
    void* Data;                                              // +8
};
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    void Clear()
    {
        if (Info)
        {
            --Info->Count;
            if (Info->Count == 0)
            {
                Info->Destroy(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
};

struct CIteratorTag { CIteratorTag() { } };

#pragma auto_inline(off)
long* __fastcall Find(long* first, long* last, const long& value, const CIteratorTag& tag)
{
    long count;
    for (count = (last - first) >> 2; 0 < count; --count) {
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
        if (*first == value) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (*first == value) return first;
        ++first;
    case 2:
        if (*first == value) return first;
        ++first;
    case 1:
        if (*first == value) return first;
    case 0:
    default:
        break;
    }
    return last;
}
#pragma auto_inline(on)

struct CLongArray {                                          // CArray<long> (12 bytes in retail)
    long* Start;
    long* Finish;
    long* EndOfStorage;
    unsigned int size() const { return Finish - Start; }
    long& operator[](unsigned int i) { return Start[i]; }
    long* Erase(long* position)
    {
        long* next = position + 1;
        if (Finish != next)
            memmove(position, next, (char*)Finish - (char*)next);
        --Finish;
        return position;
    }
};

struct C2DBoxI { long TLX, TLY, BRX, BRY; };
struct CMap;
struct CMapInfo {                                            // 0x48 bytes
    C2DBoxI               Area;                              // +0x00
    CCountedPointer<CMap> PMap;                              // +0x10
    unsigned char         _pad18[0x0e];
    bool                  Activated;                         // +0x26
    unsigned char         _rest[0x21];                       // +0x27 .. +0x48
};
struct CRegion {                                             // 0x58 bytes
    CLongArray    Maps;                                      // +0x00
    unsigned char _pad0c[0x0c];
    CCharString   Name;                                      // +0x18
    unsigned char _rest[0x58 - 0x1c];
};
#include "engine/CArray.h"

struct CQuestManager {
    void OnRegionUnloaded(const CCharString& region);      // 0x004AFC30
};
extern CQuestManager* g_pQuestManager;                       // 0x013B89FC
struct CEngineManager {
    static CEngineManager& __fastcall Get();                 // 0x00436FB0 CTBaseSingleton<CEngineManager>::Get
    void RemoveMap(long map);                                // 0x00640430
};
struct CThingManager {
    void OnLevelsUnload(const CLongArray& maps, bool b);     // 0x00525CD0
};
struct CThingMapManager {
    void RemoveGameMap(long map);                            // 0x00638340
};
struct CNavigatorManager {
    bool IsNavigationMapLoaded(long map) const;              // 0x00A14E80
    void RemoveNavigationMap(long map);                      // 0x00A155D0
};
struct CMessageEventManager {
    void* AddMessage(long type, unsigned long distance, C3DVector* pos, CThing* source, CThing* extra, float a, float b, long c);   // 0x00687540
};
struct CWorld {
    virtual void _slot0();
    virtual void _slot1();
    virtual void _slot2();
    virtual CThingManager* GetThingManager();                // slot 3 (0x0049E1B0)
    unsigned char         _pad04[0x44];
    CNavigatorManager*    NavigatorManager;                  // +0x48
    unsigned char         _pad4c[0x14];
    CMessageEventManager* MessageEventManager;               // +0x60
};

class CWorldMap { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;                // slot 10 (+0x28) 0x004FC050
    long              f4;                                    // +0x04
    CWorld*           World;                                 // +0x08
    void*             Component;                             // +0x0C
    CThingMapManager* PThingMapManager;                      // +0x10
    unsigned char     _pad14[0x0c];
    CArray<CMapInfo>  Maps;                                  // +0x20
    CArray<CRegion>   Regions;                               // +0x2C
    unsigned char     _pad38[0x58];
    CLongArray        LoadedMaps;                            // +0x90 (PDB +0xAC)
    long              LoadedRegion;                          // +0x9C (PDB +0xBC)
    unsigned char     _padA0[0x34];
    bool              RegionUnloading;                       // +0xD4 (PDB +0xF8)
protected:
    void UnloadRegion(long region, bool b);
public:
    CRegion& GetRegion(long index) { return Regions[index]; }
};

void CWorldMap::UnloadRegion(long region, bool b)
{
    CLongArray& maps = Regions[region].Maps;
    RegionUnloading = true;
    g_pQuestManager->OnRegionUnloaded(Regions[region].Name);
    World->GetThingManager()->OnLevelsUnload(maps, b);
    for (unsigned int i = 0; i < maps.size(); ++i) {
        long map = maps[i];
        if (IsMapLoaded(map)) {
            Maps[map].Activated = false;
            CEngineManager::Get().RemoveMap(map);
            LoadedMaps.Erase(Find(LoadedMaps.Start, LoadedMaps.Finish, map, CIteratorTag()));
            Maps[map].PMap.Clear();
        }
    }
    for (unsigned int j = 0; j < maps.size(); ++j) {
        long map = maps[j];
        PThingMapManager->RemoveGameMap(map);
        if (World->NavigatorManager->IsNavigationMapLoaded(map))
            World->NavigatorManager->RemoveNavigationMap(map);
    }
    LoadedRegion = 0;
    World->MessageEventManager->AddMessage(0x27, 0x32, &C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, region);
    RegionUnloading = false;
}