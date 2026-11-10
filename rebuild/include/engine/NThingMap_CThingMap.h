#ifndef FABLE_ENGINE_NTHINGMAP_CTHINGMAP_H
#define FABLE_ENGINE_NTHINGMAP_CTHINGMAP_H
// NThingMap::CThingMap (chunk 768, for the CThingSearchTools rough-area body FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY).
// Ego_r.pdb members (0x3C with the debug 16-byte vectors; 0x34 in retail with STLport 4.6 vectors): the +0x00 word is the vfptr
// (RTTI .?AVCThingMap@NThingMap@@, vtable 0x01276B00 with one slot, the scalar deleting dtor 0x0085ECF0), MapwhoLevelInfo +0x04,
// Manager +0x10, Cells +0x14, Area +0x20, NoThings +0x30 (the offsets the landed rough-area rows 0x0075BC40 / 0x00969FB0 read).
// NThingMap::CMapwhoLevelInfo (0x14) and CThingMapCell (4) are Ego_r.pdb records. The level count and the layer table keep their
// landed spellings (NThingMap::g_NoMapwhoLevels 0x013BA848; LayerInfo_0137dad8, element NThingMap::CLayerInfo in Ego_r.pdb, kept
// as the landed global struct). Needs FABLE_RETAIL_CTCMAPWHO (NThingMap::CMapwhoNode) for the cell's node list.
#include "rebuild_abi.h"
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA)
#include "engine/CRGBColour.h"
#endif
#include "stlport/fable_containers.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DExtentsI.h"
#ifdef FABLE_RETAIL_CTHINGMAPMANAGER_DEBUGDRAW
class C2DVector;
#endif
namespace NThingMap {
class CThingMapManager;
class CMapwhoNode;
class CMapwhoLevelInfo {
public:
    long        Size;                                        // +0x00
    C2DExtentsI Extents;                                     // +0x04
    long        ScaleShift;                                  // +0x0c
    long        NoThings;                                    // +0x10
};
class CThingMapCell {
public:
    CMapwhoNode* PFirstNode;                                 // +0x00
};
class CThingMap {
public:
#ifdef FABLE_RETAIL_CTHINGMAPMANAGER_DEBUGDRAW
 // Retail opt-in (Codex pass18): FableWin029d90d0, native0085cbd0.
 void DebugDraw(const class C2DVector&,long);
#endif
#ifdef FABLE_RETAIL_CTHINGMAP_GETAREA
    // Retail opt-in (Codex pass 17): FableWin GetArea and the original debug caller return Area by const reference.
    const C2DBoxI& GetArea() const { return Area; }
#endif
    virtual ~CThingMap();                                    // slot 0 (scalar deleting dtor 0x0085ECF0)
    std::vector<CMapwhoLevelInfo>              MapwhoLevelInfo;   // +0x04
    CThingMapManager*                          Manager;           // +0x10
    std::vector<std::vector<CThingMapCell> >   Cells;             // +0x14
    C2DBoxI                                    Area;              // +0x20
    long                                       NoThings;          // +0x30
};
FABLE_STATIC_ASSERT(sizeof(CThingMap) == 0x34);
#if defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY) || defined(FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA)
extern long MaxLayers;
class CLayerInfo {public:long Size,ScaleShift;CRGBColour UsedColour,UnusedColour;};
extern const CLayerInfo LayerInfo[];
}
#else
extern long g_NoMapwhoLevels;                                // 0x013BA848
}
struct CLayerInfo { long Size; long ScaleShift; unsigned long UsedColour, UnusedColour; };   // NThingMap::CLayerInfo (0x10)
extern CLayerInfo LayerInfo_0137dad8[];                      // 0x0137DAD8 (landed spelling)
extern long SearchCount_013b89a8;                            // 0x013B89A8 the rough-area search count (landed spelling)
extern long SearchWork_013b89ac;                             // 0x013B89AC (landed spelling)
extern unsigned long SearchID_013b89b0;                      // 0x013B89B0 the search stamp (landed spelling)
#endif
#endif
