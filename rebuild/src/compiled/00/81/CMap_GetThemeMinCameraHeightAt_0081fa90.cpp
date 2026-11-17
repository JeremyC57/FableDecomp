// CMap::GetThemeMinCameraHeightAt(const C2DCoordI& pos) -- the dominant engine theme at a map cell; when it covers the cell fully (blend >= 255) the theme definition's float at +0x84 (PDB CEngineThemeDef MinCameraHeight, one dword lower in retail), else 0
// Leaf: ?GetThemeMinCameraSizeZOffsetAt@CMap@@QAEMABVC2DCoordI@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081FA90, length 103 bytes (twin of 0x0081FA20 at the neighbouring float).
// Callees: CMap::GetDominantEngineThemeAt(const C2DCoordI&, long* blend) @ 0x0081F910 (the manifest's "CopyPos" label is wrong: it returns the
//   palette's theme def index and writes the blend), CDefinitionManager::GetDef<CEngineThemeDef> @ 0x0063F3A0 (manifest label GetDef<CThingPhysicalSwitchDef>,
//   a byte-twin), the def's virtual slot 1 (self-destroy) on the last reference. Floats: 0x01230014 = 255.0f, 0x0122DEDC = 0.0f.
// Layout: CMap DefinitionManager +0x1c (retail; PDB +0x28 less the debug list/vector words).
// Tried: an inline Release() member (load/dec/store of the count), the hand `if (--def->RefCount == 0)` (this source: `dec [eax+4]` then a
//   re-read), a copied register local for the def (worse), a CDefPointer object with an unchecked release destructor (same frame).
// next: lane gap: multi-local homing into a dead argument slot.
#include "fable_compiler.h"
// NF49 (round 7): the def classes and the manager come from the shared headers (were local CDefPointeeBase / CDefinitionManager
// models plus a same-TU stand-in GetDef body over a made-up FindDef_CDefinitionManager_GetDef_0 callee): the lookups at 0x0063F3A0 are
// the member template ??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z (FableWin.pdb;
// the landed 0x0063DD70 / 0x0066CF00 include chain), with the header's same-TU body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY,
// the stand-in's role); each raw def pointer released by hand is a CDefPointer<const CEngineThemeDef> released by its scope-end
// destructor. CEngineThemeDef has no shared header (engine/_quarantine only), so it stays modelled on the shared CDefClassBase.
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
class CWorld;
class C2DCoordI { public:
    long X, Y;
};
class CHeightMapCell { public:                            // PDB, 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char Flags;                           // +0x09 Changed / Passable / ShorePoint / CameraPassable / NavPreferred bits
    unsigned char _pad0a[2];
};
template <class T> struct CRetailVector {          // STLport vector: begin / end / end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
class CHeightMapInit { public:                            // PDB, 12 bytes
    long           SizeX;
    long           SizeY;
    unsigned long* WorldSeed;
};
class CEngineThemeDef : public CDefClassBase { public:            // Ego_r.pdb CEngineThemeDef (no shared header); retail offsets
    unsigned char _pad_0x25[0x80 - 0x25];
    float Height;                                  // +0x80 (retail)
    float MinCameraHeight;                         // +0x84 (retail)
};
class CMap;
class CHeightMap { public:                                // retail layout (PDB CHeightMap with the 12-byte release vector)
    virtual ~CHeightMap();                         // slot 0: the scalar deleting destructor
    CWorld*                        World;          // +0x04
    CMap*                          Map;            // +0x08
    unsigned long*                 WorldSeed;      // +0x0c
    CRetailVector<CHeightMapCell>  Cells;          // +0x10
    unsigned char                  _MapUpdateAreas[0x1e8];   // +0x1c C2DBoxI[30] MapUpdateAreas, +0x1fc NoMapUpdateAreas, +0x200 Changed
    long                           SizeX;          // +0x204
    long                           SizeY;          // +0x208
    long                           SizeXIncludingExtraCells;   // +0x20c
    long                           SizeYIncludingExtraCells;   // +0x210
    CHeightMap(CWorld* world, CMap* map, const CHeightMapInit& init);   // 0x008F63C0
    FABLE_FORCEINLINE CHeightMapCell& GetCellIncludingExtraCells(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};
template <class T> struct scoped_ptr {
    T* ptr;
    T* operator->() const { return ptr; }
    void reset(T* p) { T* old = ptr; ptr = p; delete old; }
};
class CMap { public:
    unsigned char       _pad00[0x1c];
    CDefinitionManager* DefinitionManager;           // +0x1c
    long GetDominantEngineThemeAt(const C2DCoordI& pos, long* blend);   // 0x0081F910
    float GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos);
};
void* FindRef_CMap_GetDominantEngineThemeAt_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it only writes a word through the reference parameter inline, so
// VC7.1 knows the caller's address-taken local does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) long CMap::GetDominantEngineThemeAt(const C2DCoordI& pos, long* blend)
{
    void* p = FindRef_CMap_GetDominantEngineThemeAt_0();
    if (!p)
        return 0;
    *(void**)&pos = p;
    return 0;
}
#pragma auto_inline(on)
float CMap::GetThemeMinCameraSizeZOffsetAt(const C2DCoordI& pos)
{
    long blend;
    long themeIndex = GetDominantEngineThemeAt(pos, &blend);
    if (blend >= 255.0f) {
        CDefPointer<const CEngineThemeDef> def;
        DefinitionManager->GetDef(themeIndex, &def);
        return def->MinCameraHeight;
    }
    return 0.0f;
}