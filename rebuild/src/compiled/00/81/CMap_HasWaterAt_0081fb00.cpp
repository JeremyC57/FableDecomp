// CMap::HasWaterAt(const C2DCoordI& pos) -- true if any of the (up to three) engine themes painted on the map cell is a water theme (WaterType not none/8); each theme def stays referenced until the function returns
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081FB00, length 448 bytes (`ret 4`).
// Callees: CDefinitionManager::GetDef<CEngineThemeDef> @ 0x0063F3A0 (manifest label GetDef<CThingPhysicalSwitchDef>, a byte-twin), the
//   def's virtual slot 1 (self-destroy) on the last reference.
// Layout: CMap DefinitionManager +0x1c, theme palette vector +0x1a6c (8-byte entries, def index first), height map +0x1a7c (cells
//   vector +0x10, SizeX +0x204, SizeY +0x208; 12-byte cells with the three palette indices at +4) -- as in the landed
//   CMap::GetThemeMinCameraSizeZOffsetAt 0x0081FA90 and CMap::GetEngineThemeAt 0x0081F090. CEngineThemeDef::WaterType is retail
//   +0x70 (PDB +0x74, one dword lower in retail like MinCameraHeight).
// Shape: nested scopes, one raw def pointer per theme layer released by hand (`--RefCount == 0` -> DestroySelf) on every exit;
//   GetDef is a same-TU stand-in that only writes its out slot.
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
class C2DCoordI { public:
    long X, Y;
};
class CHeightMapCell { public:                            // PDB, 12 bytes
    float         Height;                          // +0x00
    unsigned char EngineThemePaletteIndex[3];      // +0x04
    unsigned char EngineBlend[2];                  // +0x07
    unsigned char Flags;                           // +0x09
    unsigned char _pad0a[2];
};
template <class T> struct CRetailVector {          // STLport vector: begin / end / end of storage
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long n) { return *(_M_start + n); }
};
class CHeightMap { public:                                // retail layout
    unsigned char                  _pad00[0x10];
    CRetailVector<CHeightMapCell>  Cells;          // +0x10
    unsigned char                  _pad1c[0x1e8];
    long                           SizeX;          // +0x204
    long                           SizeY;          // +0x208
    FABLE_FORCEINLINE CHeightMapCell& GetCellIncludingExtraCells(long x, long y) { return Cells[(SizeY - y) * (SizeX + 1) + x]; }
};
struct CEngineThemePaletteEntry {                  // 8 bytes
    long DefIndex;
    long Count;
};
class CEngineThemeDef : public CDefClassBase { public:            // Ego_r.pdb CEngineThemeDef (no shared header); retail offsets
    unsigned char _pad_0x25[0x70 - 0x25];
    long          WaterType;                       // +0x70 (retail)
};
class CMap { public:
    unsigned char                            _pad00[0x1c];
    CDefinitionManager*                      DefinitionManager;   // +0x1c
    unsigned char                            _pad20[0x1a6c - 0x20];
    CRetailVector<CEngineThemePaletteEntry>  EngineThemePalette;  // +0x1a6c
    unsigned char                            _pad1a78[4];
    CHeightMap*                              HeightMap;           // +0x1a7c
    bool HasWaterAt(const C2DCoordI& pos);
};

bool CMap::HasWaterAt(const C2DCoordI& pos)
{
    long x = pos.X;
    long y = pos.Y;
    long index0 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[0]].DefIndex;
    if (index0 > 0) {
        CDefPointer<const CEngineThemeDef> def0;
        DefinitionManager->GetDef(index0, &def0);
        if (def0->WaterType != 0 && def0->WaterType != 8)
            return true;
        long index1 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[1]].DefIndex;
        if (index1 > 0) {
            CDefPointer<const CEngineThemeDef> def1;
            DefinitionManager->GetDef(index1, &def1);
            if (def1->WaterType != 0 && def1->WaterType != 8)
                return true;
            long index2 = EngineThemePalette[HeightMap->GetCellIncludingExtraCells(x, y).EngineThemePaletteIndex[2]].DefIndex;
            if (index2 > 0) {
                CDefPointer<const CEngineThemeDef> def2;
                DefinitionManager->GetDef(index2, &def2);
                return def2->WaterType != 0 && def2->WaterType != 8;
            }
        }
    }
    return false;
}
