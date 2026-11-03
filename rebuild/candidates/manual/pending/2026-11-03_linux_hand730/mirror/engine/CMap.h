#ifndef FABLE_ENGINE_CMAP_H
#define FABLE_ENGINE_CMAP_H
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DCoordI.h"
#ifndef FABLE_RETAIL_CIDRAWMAP
#define FABLE_RETAIL_CIDRAWMAP
#endif
#include "engine/CIDrawMap.h"
class CGameDefinitionManager;class CWorld;
class CAFile;class CScriptedMapBrush;class CScriptedMapBrushInit;
// Retail class (header wave 2026-11-03, Codex pass 12 row 0x00821010): RTTI .?AVCMap@@ : CIDrawMap@0, CBaseClassNonCopyable@4
// (ghidra_out/rtti_bases.tsv); every Ego_r.pdb data member in order (struct_layouts_egor.tsv), composites without a shared header
// opaque at retail STLport sizes; sizeof 0x1D40.
class CMap:public CIDrawMap,public CBaseClassNonCopyable {
protected:
    unsigned char CachedVolumes[4],ScriptedMapBrushes[4],ScriptedMapBrushUpdateAreas[12];
    const CGameDefinitionManager& DefinitionManager;
    CWorld& World;
    unsigned long* WorldSeed;
    float RippleBuffer[41][41];
    unsigned char ThemePaletteEntryList[12],GameMap[4],HeightMap[4];
    C2DBoxI ChangedHeightAreas[20],ChangedGameAreas[20];
    long NoChangedHeightAreas,NoChangedGameAreas;
    float SeaLevel;
    long WorldSizeX,WorldSizeY,HeightMapSizeX,HeightMapSizeY,GameMapSizeX,GameMapSizeY;
    C2DCoordI WorldPos;
    unsigned long MapUID;
    unsigned __int64 ThingUIDCount;
    bool InitialisePassabilityFromThemes;
public:
    void LoadBrushesFromFile(CAFile&);
    CScriptedMapBrush& CreateScriptedMapBrush(const CScriptedMapBrushInit&);
    bool IsPassableAt(const C2DCoordI&)const;
    bool IsPrefNavAt(const C2DCoordI&)const;
};
FABLE_STATIC_ASSERT(sizeof(CMap)==0x1d40);
#endif
