// CWorldMap::UnloadLevel(long) -- unload one map: mark it inactive, drop it from the loaded-map list, remove it from the engine, the thing manager, the map's own pointer, the thing-map manager and the navigator
// Leaf: UnloadLevel
// Retail: Fable.exe @ 0x004FEDE0, 222 bytes, `ret 4`. PDB ?UnloadLevel@CWorldMap@@IAEXJ@Z (protected)
// Callees: SGI __find<long*>(first, last, const long&, random_access_iterator_tag) 0x0050E540 (`__fastcall`, landed
// global_Find0050e540), memmove 0x00BFEAE6, CTBaseSingleton<CEngineManager>::Get 0x00436FB0 (`__fastcall`; manifest
// "CGameJoystickManager Get" is a byte twin), CEngineManager::RemoveMap(long) 0x00640430, CWorld virtual slot 3
// GetThingManager (0x0049E1B0), CThingManager::OnLevelUnload(long, bool) 0x00525D90 (manifest
// "CLandscapeGPUUpdateQueue::AddTask"), operator delete 0x00BFE9BC, NThingMap::CThingMapManager::RemoveGameMap(long)
// 0x00638340, CNavigatorManager::IsNavigationMapLoaded(long) const 0x00A14E80, RemoveNavigationMap(long) 0x00A155D0.
// Retail offsets: CWorldMap World +0x08, PThingMapManager +0x10, Maps +0x20 (CMapInfo 0x48 bytes: PMap +0x10,
// Activated +0x26), LoadedMaps (CArray<long>) +0x90 (PDB +0xAC); CWorld NavigatorManager +0x48.
// Shape: the find body sits above the caller (auto_inline off) and takes its empty iterator tag by const reference
// (user ctor): VC7.1 then places the tag at the live `map` slot and keeps `map` in edi across the call; the inline single-element erase copies
// the tail down when `pos + 1` is not the end; the map pointer is cleared by an inline release (count re-read after
// the decrement) followed by zero stores.
#include <string.h>

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

// The landed 0x0050E540 body (same TU), defined above the caller under auto_inline(off) so VC7.1 knows it does not
// write `value` (retail keeps `map` in edi across the call).
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
template <class T> struct CArray {
    T* Start;                                                // +0x00
    T* Finish;                                               // +0x04
    T* EndOfStorage;                                         // +0x08
    T& operator[](unsigned long i) { return Start[i]; }
};

struct CEngineManager {
    static CEngineManager& __fastcall Get();                 // 0x00436FB0 CTBaseSingleton<CEngineManager>::Get
    void RemoveMap(long map);                                // 0x00640430
};
struct CThingManager {
    void OnLevelUnload(long map, bool b);                    // 0x00525D90
};
struct CThingMapManager {
    void RemoveGameMap(long map);                            // 0x00638340
};
struct CNavigatorManager {
    bool IsNavigationMapLoaded(long map) const;              // 0x00A14E80
    void RemoveNavigationMap(long map);                      // 0x00A155D0
};
struct CWorld {
    virtual void _slot0();
    virtual void _slot1();
    virtual void _slot2();
    virtual CThingManager* GetThingManager();                // slot 3 (0x0049E1B0)
    unsigned char      _pad04[0x44];
    CNavigatorManager* NavigatorManager;                     // +0x48
};

struct CWorldMap {
    void*             _vptr;                                 // +0x00
    long              f4;                                    // +0x04
    CWorld*           World;                                 // +0x08
    void*             Component;                             // +0x0C
    CThingMapManager* PThingMapManager;                      // +0x10
    unsigned char     _pad14[0x0c];
    CArray<CMapInfo>  Maps;                                  // +0x20
    unsigned char     _pad2c[0x64];
    CLongArray        LoadedMaps;                            // +0x90 (PDB +0xAC)
    void UnloadLevel(long map);
};

void CWorldMap::UnloadLevel(long map)
{
    Maps[map].Activated = false;
    LoadedMaps.Erase(Find(LoadedMaps.Start, LoadedMaps.Finish, map, CIteratorTag()));
    CEngineManager::Get().RemoveMap(map);
    World->GetThingManager()->OnLevelUnload(map, true);
    Maps[map].PMap.Clear();
    PThingMapManager->RemoveGameMap(map);
    if (World->NavigatorManager->IsNavigationMapLoaded(map))
        World->NavigatorManager->RemoveNavigationMap(map);
}