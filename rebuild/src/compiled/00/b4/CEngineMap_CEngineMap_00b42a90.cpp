// CEngineMap::~CEngineMap() -- close the static map, take the map out of the landscape renderer and the local-detail generator (each when this map has the matching sub-map), reset the file-block manager; then the members' destructors (file-block manager, async file entry, static map name)
// Leaf: ??1CEngineMap@@QAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Layout: engine/CEngineLandscapeRenderer.h is a generated flat 0xF5C-byte record with no member functions; only DeleteMap is
//   needed, so the renderer is a local method-only class.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B42A90, 121 bytes, thiscall, tail `jmp ??1CCharString@@QAE@XZ` (0x0099EAE0).
// Identity: the callees are CEngineMap's own (?CloseStaticMap@CEngineMap@@QAEXXZ 0x00B3EF40, ?DeleteMap@CEngineLandscapeRenderer@@
//   QAEXPAVCEngineMap@@@Z 0x00B67180, ?DeleteMap@CEngineLocalDetailGenerator@@QAEXPAVCEngineMap@@@Z 0x00B59840, all with `this`);
//   the callers are the scalar deleting destructor 0x00B44420 (FableWin.pdb ??_GCEngineMap@@QAEPAXI@Z, non-virtual: no vtable)
//   and the landed CEngineWorldMap::DeleteMap 0x00B42B10 (`delete MapList[index]`, which already names this row ~CEngineMap).
//   Ego_r.pdb / FableWin.pdb ??1CEngineMap@@QAE@XZ. The catalog's "CBankFileEntryUpdateData" label is propagated.
// Callees (also): ?Reset@CEngineStaticMapFileBlockManager@@QAEXXZ 0x00BE05D0 (Ego_r + FableWin), the manager's implicit destructor
//   0x00B443B0 (out of line in retail, landed), operator delete 0x00BFE9BC (count block). Globals: the landscape renderer pointer
//   0x01436EA8 and the local-detail generator pointer 0x01436E88, spelt as the landed caller CEngineWorldMap::DeleteMap spells them
//   (no PDB data-symbol dump exists; see NOTES "Callee names").
// Layout: Ego_r.pdb CEngineMap, which agrees with retail through +0x58 (no STL container before FileBlockManager): LandscapeMap
//   +0x00, LocalDetailCacheMap +0x04, GameMap +0x08, MapWorldPosition +0x0C, MapUID +0x18, StaticMapName +0x1C, StaticMapOpenState
//   +0x20, CameraMapBounds +0x24, AsyncFileEntry (CCountedPointer<CBankFileAsyncEntry>) +0x3C, FileBlockManager +0x44 (retail 0x14:
//   CCountedPointer + STLport map); then Ego_r less 4 (VisibleMapListIterator +0x58 .. IsInVisibleMapList +0x64, as the landed
//   DeleteMap). Retail destroys nothing after FileBlockManager, so the Ego_r CArray members (ShorePoints, ...) are not modelled.
// Shape: the user body is four statements; the members' implicit destructors follow in reverse order -- the manager's (called),
//   engine/CCountedPointer.h's release with immediate zero stores, and CCharString's tail call.
#include "engine/CCountedPointer.h"
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
#include "engine/C3DVector.h"
class C3DBoundingBox { public: C3DVector Min, Max; };
class CEngineMap;
class CEngineLandscapeMap;
class CLocalDetailCacheMap;
class CIDrawMap;
class CBankFileAsyncEntry;
class CEngineLandscapeRenderer { public:
    void DeleteMap(CEngineMap* map);            // 0x00B67180
};
extern CEngineLandscapeRenderer* g_pLandscapeRenderer;          // 0x01436EA8
class CEngineLocalDetailGenerator { public:
    void DeleteMap(CEngineMap* map);            // 0x00B59840
};
extern CEngineLocalDetailGenerator* g_pLocalDetailGenerator;    // 0x01436E88
class CEngineStaticMapFileBlockManager { public:
    unsigned char _body[0x14];                  // AsyncBankEntry (CCountedPointer) +0x00, Blocks (STLport map) +0x08
    void Reset();                               // 0x00BE05D0
    ~CEngineStaticMapFileBlockManager();        // 0x00B443B0
};
class CEngineMap { public:
    enum STATIC_MAP_OPEN_STATE { };             // nested (Ego_r ?OpenDevelopmentStaticMap@CEngineMap@@..W4STATIC_MAP_OPEN_STATE@1@@Z); enumerators unproven
    CEngineLandscapeMap*  LandscapeMap;         // +0x00
    CLocalDetailCacheMap* LocalDetailCacheMap;  // +0x04
    CIDrawMap*            GameMap;              // +0x08
    C3DVector             MapWorldPosition;     // +0x0c
    long                  MapUID;               // +0x18
    CCharString           StaticMapName;        // +0x1c
    STATIC_MAP_OPEN_STATE StaticMapOpenState;   // +0x20
    C3DBoundingBox        CameraMapBounds;      // +0x24
    CCountedPointer<CBankFileAsyncEntry> AsyncFileEntry;   // +0x3c
    CEngineStaticMapFileBlockManager FileBlockManager;     // +0x44
    void*                 VisibleMapListIterator;   // +0x58 (STLport list<CEngineMap*>::iterator)
    unsigned short        MapWidth;             // +0x5c
    unsigned short        MapHeight;            // +0x5e
    unsigned short        WorldPosX;            // +0x60
    unsigned short        WorldPosY;            // +0x62
    bool                  IsInVisibleMapList;   // +0x64
    void CloseStaticMap();                      // 0x00B3EF40
    ~CEngineMap();
};
CEngineMap::~CEngineMap()
{
    CloseStaticMap();
    if (LandscapeMap)
        g_pLandscapeRenderer->DeleteMap(this);
    if (LocalDetailCacheMap)
        g_pLocalDetailGenerator->DeleteMap(this);
    FileBlockManager.Reset();
}