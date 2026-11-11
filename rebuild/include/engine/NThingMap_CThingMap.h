// Retail opt-in (Codex pass21): FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST exposes original targeting helpers; row00804020 retains both-PDB/donor/native proof.
#ifdef FABLE_RETAIL_MAP_VALIDATE
#ifndef FABLE_ENGINE_NTHINGMAP_CTHINGMAP_H
#define FABLE_ENGINE_NTHINGMAP_CTHINGMAP_H
#include "engine/CBaseClassNonCopyable.h"
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#include "engine/CRGBColour.h"
#endif
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/C2DBoxI.h"
#include "engine/C2DExtentsI.h"
class CTCMapwho;class C2DVector;
namespace NThingMap {
class CThingMapManager;class CThingMapCell;
// Layout: original 0x2423e, complete record.
class CMapwhoNode{
public:
 CTCMapwho * PMapwho;
 NThingMap::CMapwhoNode * PNext;
 NThingMap::CMapwhoNode * PPrevious;
 NThingMap::CThingMapCell * PCell;
 CMapwhoNode();
};
// Layout: original 0x24263, complete record.
class CThingMapCell{
public:
 CThingMapCell();
 void Clear();
 const NThingMap::CMapwhoNode * GetPFirstNode() const{return PFirstNode;}
 NThingMap::CMapwhoNode * PFirstNode;
};
// Layout: original 0x243aa, complete record.
class CMapwhoLevelInfo{
public:
 long Size;
 C2DExtentsI Extents;
 long ScaleShift;
 long NoThings;
 CMapwhoLevelInfo(const NThingMap::CMapwhoLevelInfo & p0);
 CMapwhoLevelInfo();
};
// Layout: complete original Ego24288/FW262d0; native52 after two vector reductions.
class CThingMap:public CBaseClassNonCopyable{
public:
 CThingMap(const NThingMap::CThingMap & p0);
 CThingMap(NThingMap::CThingMapManager & p0, const C2DBoxI & p1);
 virtual ~CThingMap();
 void GetAllThings(std::set<CTCMapwho *,std::less<CTCMapwho *>,std::allocator<CTCMapwho *> > & p0);
 long GetNoThingsInMapwhoLevel(long p0) const;
 void Validate();
 long GetMemoryFootprint() const;
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
 const NThingMap::CThingMapCell & PeekCell(unsigned char p0, unsigned long p1, unsigned long p2) const{return Cells[p0][MapwhoLevelInfo[p0].Extents.X*p2+p1];}
#else
 const NThingMap::CThingMapCell & PeekCell(unsigned char p0, unsigned long p1, unsigned long p2) const;
#endif
 NThingMap::CThingMapCell & GetCell(unsigned char p0,unsigned long p1,unsigned long p2){return Cells[p0][MapwhoLevelInfo[p0].Extents.X*p2+p1];}
 const C2DBoxI & GetArea() const;
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
 const C2DExtentsI & GetLayerExtents(long p0) const{return MapwhoLevelInfo[p0].Extents;}
#else
 const C2DExtentsI & GetLayerExtents(long p0) const;
#endif
 void Clear();
 void DebugDraw(const C2DVector & p0, long p1);
protected:
 std::vector<NThingMap::CMapwhoLevelInfo,std::allocator<NThingMap::CMapwhoLevelInfo> > MapwhoLevelInfo;
 NThingMap::CThingMapManager & Manager;
 std::vector<std::vector<NThingMap::CThingMapCell,std::allocator<NThingMap::CThingMapCell> >,std::allocator<std::vector<NThingMap::CThingMapCell,std::allocator<NThingMap::CThingMapCell> > > > Cells;
 C2DBoxI Area;
 long NoThings;
 void InitMapwhoLevels();
 void ValidateCell(const NThingMap::CThingMapCell & cell) const {
 for(const CMapwhoNode* ploop_node1=cell.GetPFirstNode();ploop_node1;ploop_node1=ploop_node1->PNext) {
  long c1=0;
  for(const CMapwhoNode* ploop_node2=ploop_node1->PNext;ploop_node2;ploop_node2=ploop_node2->PNext)++c1;
 }
 }
public:
 CThingMap & operator=(const NThingMap::CThingMap & p0);
};
FABLE_STATIC_ASSERT(sizeof(CThingMap)==52);
#ifdef FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
class CLayerInfo {public:long Size,ScaleShift;CRGBColour UsedColour,UnusedColour;};
extern const CLayerInfo LayerInfo[];
#endif
extern long MaxLayers;
}
#endif

#else
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

#endif
