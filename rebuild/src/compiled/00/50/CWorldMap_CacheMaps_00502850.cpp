// CWorldMap::CacheMaps(ELoadType loadType) -- proximity map caching: for every used map that loads on player
// proximity, unload it (posting message 0x25) once the player is 64+ cells away, or start loading it when the player
// comes within 48 cells (draining the level loader immediately for load type 0); disabled while region loading runs
// Leaf: CacheMaps
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00502850, 330 bytes, `ret 4`. PDB ?CacheMaps@CWorldMap@@IAEXW4ELoadType@1@@Z.
// Callees: CWorldMap::GetMinPlayerDistanceFromMap(long) 0x004FCB30, CWorldMap::IsMapLoaded(long) const virtual slot 10
//   (+0x28, vtable 0x01244AEC -> 0x004FC050), CMessageEventManager::AddMessage 0x00687540 on the world's +0x60,
//   CWorldMap::UnloadLevel(long) 0x004FEDE0, CWorldMap::LoadLevel(long, ELoadType) 0x00502620, CLevelLoader
//   IsActionPointFree-labelled busy test 0x006C20A0 and its virtual slot 1 update.
// Layout: retail CWorldMap (local model, as the landed CWorldMap rows): World +0x08, Maps +0x20 (CMapInfo 0x48 bytes:
//   Used +0x24, Loading +0x27, LoadedOnPlayerProximity +0x28), PLevelLoader data +0xbc; region-loading flag byte
//   0x013756F6 (landed name g_bRegionLoadingEnabled).
// Note: the "unload" branch tests Maps[loadType].Loading -- retail indexes the map array with the load type there.
// Name fix (agentNF5 2026-11-10): CMessageEventManager::AddMessage 0x00687540 is ?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z (Ego_r.pdb / FableWin.pdb): EMessageEventType, a const C3DVector reference, CMessageEvent* result.
enum EMessageEventType;                            // MS forward enum (CMessageEventManager::AddMessage 0x00687540 takes it)
class CMessageEvent;
#include "engine/C3DVector.h"
class CThing;
class CMessageEventManager { public:
    CMessageEvent* AddMessage(EMessageEventType type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);          // 0x00687540
};
class CWorld { public:
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;         // +0x60
};
class CLevelLoader { public:
    virtual void v00();
    virtual void Update();                              // slot 1
    bool IsBusy() const;                                      // 0x006C20A0
};
class CMapInfo { public:                                       // 0x48 bytes
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
class CWorldMap { public:
    // NF108 (2026-11-20, q4648 follow-up): ELoadType is nested in CWorldMap (Ego_r.pdb / FableWin.pdb LF_ENUMERATE WAIT_FOR_LOAD 0,
    //   DONT_WAIT_FOR_LOAD 1; engine/CWorldMap.h) and LoadLevel / CacheMaps are protected in both PDBs: this row's own leaf is
    //   ?CacheMaps@CWorldMap@@IAEXW4ELoadType@1@@Z (as 0x005064B0 / 0x005063E0 bind it) and LoadLevel 0x00502620 is
    //   ?LoadLevel@CWorldMap@@IAEXJW4ELoadType@1@@Z (its self row's Leaf). Was a file-scope stand-in enum, both public.
    enum ELoadType { WAIT_FOR_LOAD = 0, DONT_WAIT_FOR_LOAD = 1 };
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual bool IsMapLoaded(long map) const;           // slot 10 (+0x28)
    long          f4;                                   // +0x04
    CWorld*       World;                                // +0x08
    unsigned char _pad0c[0x20 - 0x0c];
    CMapInfoArray Maps;                                 // +0x20
    unsigned char _pad2c[0xbc - 0x2c];
    CLevelLoader* PLevelLoader;                         // +0xbc CCountedPointer<CLevelLoader>::Data
    protected:
    long GetMinPlayerDistanceFromMap(long map) const;
    public:
         // 0x004FCB30
    protected:
    void UnloadLevel(long map);
    public:
                         // 0x004FEDE0
protected:
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
                    World->PMessageEventManager->AddMessage((EMessageEventType)(0x25), 0x32, C3DVector(0.0f, 0.0f, 0.0f), 0, 0, 0.0f, 0.0f, i);
                    UnloadLevel(i);
                }
            }
            else if (!Maps[i].Loading && distance < 0x30) {
                LoadLevel(i, DONT_WAIT_FOR_LOAD);
                if (loadType == WAIT_FOR_LOAD) {
                    while (PLevelLoader->IsBusy())
                        PLevelLoader->Update();
                }
            }
        }
    }
}