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
#ifdef FABLE_RETAIL_CMAP_THEMEPALETTE
// Nested opt-in (chunk 760, row 0x008F70F0 CMap::AddThemeDefIndexToPalette): the theme palette members typed. Ego_r.pdb
// CThemePaletteEntry {ThemeDefIndex ulong +0, ThemePaletteIndexUsed bool +4} (8 bytes); CMap ThemePaletteEntryList
// vector<CThemePaletteEntry>, GameMap scoped_ptr<CGameMap>, HeightMap scoped_ptr<CHeightMap> (Ego_r +0x1A78/+0x1A88/+0x1A8C,
// retail +0x1A6C/+0x1A78/+0x1A7C: the 12-byte STLport vector and the three 4-byte containers before it). The (index, used)
// constructor is the landed ZeroAllUnusedThemePaletteEntries 0x008F6FC0 model's entry-0 reset.
#include "stlport/fable_containers.h"
#include "boost/scoped_ptr.h"
class CGameMap;class CHeightMap;
class CThemePaletteEntry {
public:
    CThemePaletteEntry(unsigned long themeDefIndex, bool used) : ThemeDefIndex(themeDefIndex), ThemePaletteIndexUsed(used) {}
    unsigned long ThemeDefIndex;                                         // +0x00
    bool ThemePaletteIndexUsed;                                          // +0x04
};
#endif
class CAFile;class CScriptedMapBrush;class CScriptedMapBrushInit;
#ifdef FABLE_RETAIL_CMAP_NAVIGATIONLAYER
class C3DVector;
#endif
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
#ifdef FABLE_RETAIL_CMAP_THEMEPALETTE
    std::vector<CThemePaletteEntry> ThemePaletteEntryList;
    boost::scoped_ptr<CGameMap> GameMap;
    boost::scoped_ptr<CHeightMap> HeightMap;
#else
    unsigned char ThemePaletteEntryList[12],GameMap[4],HeightMap[4];
#endif
    C2DBoxI ChangedHeightAreas[20],ChangedGameAreas[20];
    long NoChangedHeightAreas,NoChangedGameAreas;
    float SeaLevel;
    long WorldSizeX,WorldSizeY,HeightMapSizeX,HeightMapSizeY,GameMapSizeX,GameMapSizeY;
    C2DCoordI WorldPos;
    unsigned long MapUID;
    unsigned __int64 ThingUIDCount;
    bool InitialisePassabilityFromThemes;
#ifdef FABLE_RETAIL_CMAP_THEMEPALETTE
    // Ego_r.pdb donor order 0x008545D0 / 0x00854600 / 0x00854630 / 0x00854760 = retail (inline) / 0x008F6F90 / 0x008F6FC0 /
    // 0x008F70F0. GetThemePaletteIndexFromThemeDefIndex is inlined at its only retail call (no out-of-line copy is bound);
    // FindUnusedThemePaletteEntryIndex is inline at its first call there and called out of line (0x008F6F90) at its second.
    unsigned char GetThemePaletteIndexFromThemeDefIndex(unsigned long themeDefIndex) const
    {
        unsigned long index = 0;
        for (std::vector<CThemePaletteEntry>::const_iterator it = ThemePaletteEntryList.begin(); it != ThemePaletteEntryList.end(); ++it, ++index)
            if (index > 0 && it->ThemeDefIndex == themeDefIndex)
                return (unsigned char)index;
        return 0;
    }
    unsigned char FindUnusedThemePaletteEntryIndex()                     // 0x008F6F90 ?FindUnusedThemePaletteEntryIndex@CMap@@IAEEXZ
    {
        unsigned long index = 0;
        for (std::vector<CThemePaletteEntry>::iterator it = ThemePaletteEntryList.begin(); it != ThemePaletteEntryList.end(); ++it, ++index)
            if (index > 0 && !it->ThemePaletteIndexUsed)
                return (unsigned char)index;
        return 0;
    }
    void ZeroAllUnusedThemePaletteEntries();                             // 0x008F6FC0
public:
    unsigned char AddThemeDefIndexToPalette(unsigned long themeDefIndex); // 0x008F70F0
#endif
public:
    void LoadBrushesFromFile(CAFile&);
    CScriptedMapBrush& CreateScriptedMapBrush(const CScriptedMapBrushInit&);
    bool IsPassableAt(const C2DCoordI&)const;
    bool IsPrefNavAt(const C2DCoordI&)const;
#ifdef FABLE_RETAIL_CMAP_NAVIGATIONLAYER
    // Chunk 847 (row 0x0063E590 CWorldMap::GetNavigationLayerAt): Ego_r.pdb ?GetNavigationLayerAt@CMap@@QBEJABVC3DVector@@@Z, out of
    // line in retail at 0x0081E920 (catalog CMap::GetNavigationLayerAt). Declaration only: no layout change.
    long GetNavigationLayerAt(const C3DVector&)const;
#endif
};
FABLE_STATIC_ASSERT(sizeof(CMap)==0x1d40);
#endif
