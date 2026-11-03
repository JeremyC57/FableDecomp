#ifndef FABLE_ENGINE_CMAP_H
#define FABLE_ENGINE_CMAP_H
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DCoordI.h"
// Declaration projection of original draw interface: no virtual call through it in this TU.
class CIDrawMap {public:virtual ~CIDrawMap();};
class CGameDefinitionManager;class CWorld;
#ifdef FABLE_RETAIL_MAP_BRUSHES
class CAFile;class CScriptedMapBrush;class CScriptedMapBrushInit;
#endif
// Original two bases and every named data field; unused composite objects retain full native storage.
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
#ifdef FABLE_RETAIL_MAP_BRUSHES
    void LoadBrushesFromFile(CAFile&);
    CScriptedMapBrush& CreateScriptedMapBrush(const CScriptedMapBrushInit&);
#endif
    bool IsPassableAt(const C2DCoordI&)const;
    bool IsPrefNavAt(const C2DCoordI&)const;
};
FABLE_STATIC_ASSERT(sizeof(CMap)==0x1d40);
#endif
