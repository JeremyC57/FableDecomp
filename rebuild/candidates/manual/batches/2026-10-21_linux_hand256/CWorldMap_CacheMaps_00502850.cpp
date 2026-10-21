// CWorldMap::CacheMaps(ELoadType loadType) -- proximity map caching: for every used map that loads on player
// proximity, unload it (posting message 0x25) once the player is 64+ cells away, or start loading it when the player
// comes within 48 cells (draining the level loader immediately for load type 0); disabled while region loading runs
// Leaf: CacheMaps
// Retail: Fable.exe (TLC, VC7.1) @ 0x00502850, 330 bytes, `ret 4`. PDB ?CacheMaps@CWorldMap@@IAEXW4ELoadType@@@Z.
// Callees: CWorldMap::GetMinPlayerDistanceFromMap(long) 0x004FCB30, CWorldMap::IsMapLoaded(long) const virtual slot 10
//   (+0x28, vtable 0x01244AEC -> 0x004FC050), CMessageEventManager::AddMessage 0x00687540 on the world's +0x60,
//   CWorldMap::UnloadLevel(long) 0x004FEDE0, CWorldMap::LoadLevel(long, ELoadType) 0x00502620, CLevelLoader
//   IsActionPointFree-labelled busy test 0x006C20A0 and its virtual slot 1 update.
// Layout: retail CWorldMap (local model, as the landed CWorldMap rows): World +0x08, Maps +0x20 (CMapInfo 0x48 bytes:
//   Used +0x24, Loading +0x27, LoadedOnPlayerProximity +0x28), PLevelLoader data +0xbc; region-loading flag byte
//   0x013756F6 (landed name g_bRegionLoadingEnabled).
// Note: the "unload" branch tests Maps[loadType].Loading -- retail indexes the map array with the load type there.
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
struct CThing;
struct CMessageEventManager {
    void* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1,
                     float value2, long data);          // 0x00687540
};
struct CWorld {
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;         // +0x60
};
struct CLevelLoader {
    virtual void v00();
    virtual void Update();                              // slot 1
    bool IsBusy();                                      // 0x006C20A0
};
enum ELoadType { LOAD_TYPE_IMMEDIATE = 0, LOAD_TYPE_BACKGROUND = 1 };
struct CMapInfo {                                       // 0x48 bytes
    unsigned char _pad00[0x24];
    bool          Used;                                 // +0x24
    bool          Placed;                               // +0x25
    bool          Activated;                            // +0x26
    bool          Loading;                              // +0x27
    bool          LoadedOnPlayerProximity;              // +0x28
    unsigned char _pad29[0x48 - 0x29];
};
struct CMapInfoArray {
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    CMapInfo& operator[](long n) { return _M_start[n]; }
};
extern bool g_bRegionLoadingEnabled;                    // 0x013756F6
struct CWorldMap {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;           // slot 10 (+0x28)
    long          f4;                                   // +0x04
    CWorld*       World;                                // +0x08
    unsigned char _pad0c[0x20 - 0x0c];
    CMapInfoArray Maps;                                 // +0x20
    unsigned char _pad2c[0xbc - 0x2c];
    CLevelLoader* PLevelLoader;                         // +0xbc CCountedPointer<CLevelLoader>::Data
    long GetMinPlayerDistanceFromMap(long map);         // 0x004FCB30
    void UnloadLevel(long map);                         // 0x004FEDE0
    void LoadLevel(long map, ELoadType loadType);       // 0x00502620
    void CacheMaps(ELoadType loadType);
};
void CWorldMap::CacheMaps(ELoadType loadType)
{
    if (g_bRegionLoadingEnabled)
        return;
    for (long i = 0; i < Maps.size(); ++i) {
        if (Maps[i].Used && Maps[i].LoadedOnPlayerProximity) {
            long distance = GetMinPlayerDistanceFromMap(i);
            if (IsMapLoaded(i)) {
                if (distance >= 0x40 && !Maps[loadType].Loading) {
                    World->PMessageEventManager->AddMessage(0x25, 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, i);
                    UnloadLevel(i);
                }
            }
            else if (!Maps[i].Loading && distance < 0x30) {
                LoadLevel(i, LOAD_TYPE_BACKGROUND);
                if (loadType == LOAD_TYPE_IMMEDIATE) {
                    while (PLevelLoader->IsBusy())
                        PLevelLoader->Update();
                }
            }
        }
    }
}
